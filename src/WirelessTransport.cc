//
// A wireless transport that implements the ITransport
// interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#include "WirelessTransport.h"

Define_Module(WirelessTransport);

void WirelessTransport::initialize(int stage)
{
    if (stage == 0) {
        // get all parameters
        wirelessTechnology = par("wirelessTechnology").stringValue();
        operationMode = par("operationMode").stringValue();
        connectString = par("connectString").stringValue();
        wirelessRange = par("wirelessRange");
        dataRate = par("dataRate");
        packetErrorRate = par("packetErrorRate");
        scanInterval = par("scanInterval");
        headerSize = par("headerSize");

        // validate parameters
        if (!(operationMode == "ap" || operationMode == "client" || operationMode == "direct")) {
            EV_INFO << WIRELESSTRANSPORT_SIMMODULEINFO << "operationMode must be - ap, client or direct" << "\n";
            throw cRuntimeError("Check log for details");
        }

    } else if (stage == 1) {

        // initialize variables
        buildMACLikeAddress();
        lastConnectedAP = NULL;

        // get references to models
        getDeusModel();
        getAllOtherModels();

        // register the transport with Deus
        registerWirelessTransportWithDeus();

    } else if (stage == 2) {

        // reminder to generate transport registration event
        cMessage *transportRegReminderEvent = new cMessage("Transport Registration Reminder Event");
        transportRegReminderEvent->setKind(WIRELESSTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), transportRegReminderEvent);

//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "macAddress: " << macAddress << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "wirelessTechnology: " << wirelessTechnology << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "operationMode: " << operationMode << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "connectString: " << connectString << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "wirelessRange: " << wirelessRange << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "dataRate: " << dataRate << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "packetErrorRate: " << packetErrorRate << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "scanInterval: " << scanInterval << "\n";
//        cout << WIRELESSTRANSPORT_SIMMODULEINFO << "headerSize: " << headerSize << "\n";


    } else {
        EV_INFO << WIRELESSTRANSPORT_SIMMODULEINFO << "unknown initialize() stage" << "\n";
        throw cRuntimeError("Check log for details");

    }
}

void WirelessTransport::handleMessage(cMessage *msg)
{
    // register transport with upper layer (forwarder)
    if (msg->isSelfMessage()) {
        if (msg->getKind() == WIRELESSTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE) {

            TransportRegistrationMsg *transportRegMsg = new TransportRegistrationMsg();
            transportRegMsg->setTransportID(getId());
            transportRegMsg->setTransportDescription("Wireless Transport");
            transportRegMsg->setTransportAddress(macAddress.c_str());

            send(transportRegMsg, "forwarderInOut$o");

        }

        delete msg;

    } else {
        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        // process message
        if (strstr(gateName, "forwarderInOut") != NULL) {

            processOutgoingMessage(msg);

        } else if (strstr(gateName, "radioIn") != NULL) {

            processIncomingMessage(msg);

        }
    }
}

void WirelessTransport::processOutgoingMessage(cMessage *msg)
{

    // build the list of potential nodes to send the message
    // when node is an AP, look for all the nodes connected to AP
    if (operationMode == "ap") {
        processOutgoingOnAPNode(msg);


    // when node is a client, look for the AP connected to
    } else if (operationMode == "client") {
        processOutgoingOnClientNode(msg);


    // when node is a configured to communicate directly with other
    // nodes, then find other nodes in range
    } else if (operationMode == "direct") {
        processOutgoingOnDirectNode(msg);

    }
}

void WirelessTransport::processIncomingMessage(cMessage *msg)
{
    // check message, must be a transport message
    TransportMsg *transportMsg;
    if ((transportMsg = dynamic_cast<TransportMsg*>(msg)) == NULL) {
        delete msg;
        return;
    }

    // get source of the msg
    ExchangedTransportInfo *arrivalTransportInfo = new ExchangedTransportInfo("ExchangedTransportInfo");
    arrivalTransportInfo->transportAddress = transportMsg->getSourceAddress();

    // decapsulate and get original packet
    cMessage *decapsulatedMsg = transportMsg->decapsulate();

    // attach source info
    decapsulatedMsg->addObject(arrivalTransportInfo);

    // send decapsulated msg to forwarding
    send(decapsulatedMsg, "forwarderInOut$o");

    delete msg;
}

void WirelessTransport::processOutgoingOnAPNode(cMessage *msg)
{
    // get CCNx message size
    long msgSize = ((cPacket *)msg)->getByteLength();

    // get destination address if given
    ExchangedTransportInfo *destinationTransportInfo = NULL;
    if (msg->hasObject("ExchangedTransportInfo")) {
        destinationTransportInfo = check_and_cast<ExchangedTransportInfo*>(msg->getObject("ExchangedTransportInfo"));
        msg->removeObject("ExchangedTransportInfo");
    }

    // vector to hold all the nodes to communicate with
    vector <WirelessTransportInfo*> potentialWirelessNodesList;

    // get the relevant client wireless group whose members can potentially be a node
    // connected to the AP
    bool found = false;
    SameWirelessGroup *sameWirelessGroup = NULL;
    list<SameWirelessGroup*>::iterator iteratorSameWirelessGroup = deusModel->sameWirelessGroupList.begin();
    while (iteratorSameWirelessGroup != deusModel->sameWirelessGroupList.end()) {
        sameWirelessGroup = *iteratorSameWirelessGroup;

        if (strcmp(sameWirelessGroup->wirelessTechnology.c_str(), wirelessTechnology.c_str()) == 0
                && strcmp(sameWirelessGroup->operationMode.c_str(), "client") == 0
                && strcmp(sameWirelessGroup->connectString.c_str(), connectString.c_str()) == 0) {
            found = true;
            break;
        }

        iteratorSameWirelessGroup++;
    }

    // when not found, that means no client nodes in network
    // so, discard message and return
    if (!found) {
        if (destinationTransportInfo) {
            delete destinationTransportInfo;
        }
        delete msg;
        return;
    }

    // make the list of clients connected to AP
    // based on wireless range
    // make list of nodes that are currently in wireless range of AP
    list<WirelessTransportInfo*>::iterator iteratorWirelessTransportInfo = sameWirelessGroup->wirelessTransportInfoList.begin();
    while (iteratorWirelessTransportInfo != sameWirelessGroup->wirelessTransportInfoList.end()) {
        WirelessTransportInfo *wirelessTransportInfo = *iteratorWirelessTransportInfo;

        // when in wireless range, select
        if (inWirelessRange(wirelessTransportInfo->mobilityModel, mobilityModel, wirelessRange)) {
            potentialWirelessNodesList.push_back(wirelessTransportInfo);
        }

        iteratorWirelessTransportInfo++;
    }

    // check whether a destination address is given
    // this means the message goes only to one destination
    if (destinationTransportInfo != NULL) {

        // when destination given, then send message to only given node
        // but still, only if in neighbourhood (i.e., connected)
        bool found = false;
        WirelessTransportInfo *wirelessTransportInfo = NULL;
        for (int i = 0; i < potentialWirelessNodesList.size(); i++) {
            wirelessTransportInfo = potentialWirelessNodesList[i];
            if (wirelessTransportInfo->macAddress == destinationTransportInfo->transportAddress) {
                found = true;
                break;
            }
        }

        // when node is not in list, then that node has moved away
        // so discard message
        if (!found) {
            if (destinationTransportInfo) {
                delete destinationTransportInfo;
            }
            delete msg;
            return;
        }

        // when node is in wireless range, create and send the message

        // make copy of original message
        cMessage *copyOfMsg = msg->dup();

        // create message
        TransportMsg *transportMsg = new TransportMsg();
        transportMsg->setSourceAddress(macAddress.c_str());
        transportMsg->setBroadcastMsg(false);
        transportMsg->setDestinationAddress(destinationTransportInfo->transportAddress.c_str());
        transportMsg->encapsulate((cPacket *) copyOfMsg);
        transportMsg->setHeaderSize(headerSize);
        transportMsg->setPayloadSize(msgSize);
        transportMsg->setByteLength(headerSize + msgSize);

        // send msg directly to node
        sendDirect(transportMsg, wirelessTransportInfo->wirelessTransportModel, "radioIn");

        // remove original msg and return
        if (destinationTransportInfo) {
            delete destinationTransportInfo;
        }
        delete msg;

        return;

    }

    // this means the message goes to all the nodes in wireless range
    if (destinationTransportInfo == NULL) {

        // loop around list and send to every one
        for (int i = 0; i < potentialWirelessNodesList.size(); i++) {
            WirelessTransportInfo *wirelessTransportInfo = potentialWirelessNodesList[i];

            // make copy of original message
            cMessage *copyOfMsg = msg->dup();

            // create message
            TransportMsg *transportMsg = new TransportMsg();
            transportMsg->setSourceAddress(macAddress.c_str());
            transportMsg->setBroadcastMsg(true);
            transportMsg->setDestinationAddress("");
            transportMsg->encapsulate((cPacket *)copyOfMsg);
            transportMsg->setHeaderSize(headerSize);
            transportMsg->setPayloadSize(msgSize);
            transportMsg->setByteLength(headerSize + msgSize);

            // send msg directly to node
            sendDirect(transportMsg, wirelessTransportInfo->wirelessTransportModel, "radioIn");
        }

        // remove original msg and return
        if (destinationTransportInfo) {
            delete destinationTransportInfo;
        }
        delete msg;

        return;
    }
}

void WirelessTransport::processOutgoingOnClientNode(cMessage *msg)
{
    // get CCNx message size
    long msgSize = ((cPacket *)msg)->getByteLength();

    // get destination address if given
    ExchangedTransportInfo *destinationTransportInfo = NULL;
    if (msg->hasObject("ExchangedTransportInfo")) {
        destinationTransportInfo = check_and_cast<ExchangedTransportInfo*>(msg->getObject("ExchangedTransportInfo"));
        msg->removeObject("ExchangedTransportInfo");
    }

    // get the ap wireless group to identify to which this client is
    // connected
    bool found = false;
    SameWirelessGroup *sameWirelessGroup = NULL;
    list<SameWirelessGroup*>::iterator iteratorSameWirelessGroup = deusModel->sameWirelessGroupList.begin();
    while (iteratorSameWirelessGroup != deusModel->sameWirelessGroupList.end()) {
        sameWirelessGroup = *iteratorSameWirelessGroup;

        if (strcmp(sameWirelessGroup->wirelessTechnology.c_str(), wirelessTechnology.c_str()) == 0
                && strcmp(sameWirelessGroup->operationMode.c_str(), "ap") == 0
                && strcmp(sameWirelessGroup->connectString.c_str(), connectString.c_str()) == 0) {
            found = true;
            break;
        }

        iteratorSameWirelessGroup++;
    }

    // when not found, that means no ap is active in network
    // so, discard message and return
    if (!found) {
        if (destinationTransportInfo != NULL) {
            delete destinationTransportInfo;
        }
        delete msg;
        return;
    }

    // identify the AP to connect to
    WirelessTransportInfo* currentConnectAP = NULL;
    list<WirelessTransportInfo*>::iterator iteratorWirelessTransportInfo = sameWirelessGroup->wirelessTransportInfoList.begin();
    while (iteratorWirelessTransportInfo != sameWirelessGroup->wirelessTransportInfoList.end()) {
        WirelessTransportInfo *wirelessTransportInfo = *iteratorWirelessTransportInfo;

        // when in wireless range select AP
        if (inWirelessRange(wirelessTransportInfo->mobilityModel, mobilityModel, wirelessRange)) {
            currentConnectAP = wirelessTransportInfo;

            // when there was no last connected AP,
            // select the first found as the AP to connect to
            if (lastConnectedAP == NULL) {
                break;
            }

            // when the selected AP is the same as last connected,
            // use the same - to avoid ping-pong effect of selecting APs
            if (currentConnectAP->macAddress == lastConnectedAP->macAddress) {
                break;
            }

        }

        iteratorWirelessTransportInfo++;
    }

    // the AP may not be in range
    if (currentConnectAP == NULL) {
        if (destinationTransportInfo != NULL) {
            delete destinationTransportInfo;
        }

        delete msg;
        return;
    }

    // use the AP
    lastConnectedAP = currentConnectAP;

    // the selected AP must also be the destination,
    // if not, discard the msg
    if (destinationTransportInfo != NULL && currentConnectAP->macAddress != destinationTransportInfo->transportAddress) {
        delete destinationTransportInfo;
        delete msg;
        return;
    }

    // make copy of original message
    cMessage *copyOfMsg = msg->dup();

    // create message
    TransportMsg *transportMsg = new TransportMsg();
    transportMsg->setSourceAddress(macAddress.c_str());
    transportMsg->setBroadcastMsg(false);
    transportMsg->setDestinationAddress(currentConnectAP->macAddress.c_str());
    transportMsg->encapsulate((cPacket *) copyOfMsg);
    transportMsg->setHeaderSize(headerSize);
    transportMsg->setPayloadSize(msgSize);
    transportMsg->setByteLength(headerSize + msgSize);

    // send msg directly to node
    sendDirect(transportMsg, currentConnectAP->wirelessTransportModel, "radioIn");

    // remove original msg and return
    if (destinationTransportInfo != NULL) {
        delete destinationTransportInfo;
    }
    delete msg;
    return;
}

void WirelessTransport::processOutgoingOnDirectNode(cMessage *msg)
{
    // get CCNx message size
    long msgSize = ((cPacket *)msg)->getByteLength();

    // get destination address if given
    ExchangedTransportInfo *destinationTransportInfo = NULL;
    if (msg->hasObject("ExchangedTransportInfo")) {
        destinationTransportInfo = check_and_cast<ExchangedTransportInfo*>(msg->getObject("ExchangedTransportInfo"));
        msg->removeObject("ExchangedTransportInfo");
    }

    // vector to hold all the nodes to communicate with
    vector <WirelessTransportInfo*> potentialWirelessNodesList;

    // get the relevant client wireless group whose members can potentially be a node
    // to communicate with
    bool found = false;
    SameWirelessGroup *sameWirelessGroup = NULL;
    list<SameWirelessGroup*>::iterator iteratorSameWirelessGroup = deusModel->sameWirelessGroupList.begin();
    while (iteratorSameWirelessGroup != deusModel->sameWirelessGroupList.end()) {
        sameWirelessGroup = *iteratorSameWirelessGroup;

        if (strcmp(sameWirelessGroup->wirelessTechnology.c_str(), wirelessTechnology.c_str()) == 0
                && strcmp(sameWirelessGroup->operationMode.c_str(), "direct") == 0
                && strcmp(sameWirelessGroup->connectString.c_str(), connectString.c_str()) == 0) {
            found = true;
            break;
        }

        iteratorSameWirelessGroup++;
    }

    // when not found, that means no client nodes in direct mode to communicate with
    // so, discard message and return
    if (!found) {
        if (destinationTransportInfo != NULL) {
            delete destinationTransportInfo;
        }
        delete msg;
        return;
    }

    // make the list of clients with whom this node can potentially communicate
    // based on wireless range
    list<WirelessTransportInfo*>::iterator iteratorWirelessTransportInfo = sameWirelessGroup->wirelessTransportInfoList.begin();
    while (iteratorWirelessTransportInfo != sameWirelessGroup->wirelessTransportInfoList.end()) {
        WirelessTransportInfo *wirelessTransportInfo = *iteratorWirelessTransportInfo;

        // leave out yourself from the list
        if (wirelessTransportInfo->macAddress == macAddress) {
            iteratorWirelessTransportInfo++;
            continue;
        }

        // when in wireless range, select
        if (inWirelessRange(wirelessTransportInfo->mobilityModel, mobilityModel, wirelessRange)) {
            potentialWirelessNodesList.push_back(wirelessTransportInfo);
        }

        iteratorWirelessTransportInfo++;
    }

    // check whether a destination address is given
    // this means the message goes only to one destination
    if (destinationTransportInfo != NULL) {

        // when destination given, then send message to only given node
        // but still, only if  that node is in neighbourhood (i.e., connected)
        bool found = false;
        WirelessTransportInfo *wirelessTransportInfo = NULL;
        for (int i = 0; i < potentialWirelessNodesList.size(); i++) {

            wirelessTransportInfo = potentialWirelessNodesList[i];
            if (wirelessTransportInfo->macAddress == destinationTransportInfo->transportAddress) {
                found = true;
                break;
            }

        }

        // when node is not in list (i.e., neighbourhood), then that node has moved away
        // so discard message
        if (!found) {
            if (destinationTransportInfo != NULL) {
                delete destinationTransportInfo;
            }
            delete msg;
            return;
        }

        // when node is in wireless range, create and send the message

        // make copy of original message
        cMessage *copyOfMsg = msg->dup();

        // create message
        TransportMsg *transportMsg = new TransportMsg();
        transportMsg->setSourceAddress(macAddress.c_str());
        transportMsg->setBroadcastMsg(false);
        transportMsg->setDestinationAddress(destinationTransportInfo->transportAddress.c_str());
        transportMsg->encapsulate((cPacket *) copyOfMsg);
        transportMsg->setHeaderSize(headerSize);
        transportMsg->setPayloadSize(msgSize);
        transportMsg->setByteLength(headerSize + msgSize);

        // send msg directly to node
        sendDirect(transportMsg, wirelessTransportInfo->wirelessTransportModel, "radioIn");

        // remove original msg and return
        if (destinationTransportInfo != NULL) {
            delete destinationTransportInfo;
        }
        delete msg;
        return;

    }

    // this means the message goes to all the direct nodes in wireless range
    if (destinationTransportInfo == NULL) {

        // loop around list and send to every one
        for (int i = 0; i < potentialWirelessNodesList.size(); i++) {
            WirelessTransportInfo *wirelessTransportInfo = potentialWirelessNodesList[i];

            // make copy of original message
            cMessage *copyOfMsg = msg->dup();

            // create message
            TransportMsg *transportMsg = new TransportMsg();
            transportMsg->setSourceAddress(macAddress.c_str());
            transportMsg->setBroadcastMsg(true);
            transportMsg->setDestinationAddress("");
            transportMsg->encapsulate((cPacket *)copyOfMsg);
            transportMsg->setHeaderSize(headerSize);
            transportMsg->setPayloadSize(msgSize);
            transportMsg->setByteLength(headerSize + msgSize);

            // send msg directly to node
            sendDirect(transportMsg, wirelessTransportInfo->wirelessTransportModel, "radioIn");

        }

        // remove original msg and return
        if (destinationTransportInfo) {
            delete destinationTransportInfo;
        }
        delete msg;
        return;
    }

}

void WirelessTransport::buildMACLikeAddress()
{
    // build own (MAC-like) unique address - 0xBB is for wireless transports
    char str[24];
    long ifcID = getId();
    int len = sizeof(ifcID) - 1;
    unsigned char numList[] = {0xBB, 0, 0, 0, 0, 0};
    for (int i = 5; i >= 1 && len >= 0; i--) {
        unsigned char num = ifcID & 0xFF;
        numList[i] = num;
        ifcID = ifcID >> 8;
        len--;
    }
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", numList[0], numList[1],
                                               numList[2],numList[3],
                                               numList[4], numList[5]);
    macAddress = str;
}

void WirelessTransport::getDeusModel()
{
    // get Deus
    deusModel = NULL;
    for (int id = 0; id <= getSimulation()->getLastComponentId(); id++) {
        cModule *unknownModel = getSimulation()->getModule(id);
        if (unknownModel == NULL) {
            continue;
        }
        if (dynamic_cast<Deus*>(unknownModel) != NULL) {
            deusModel = dynamic_cast<Deus*>(unknownModel);
            break;
        }
    }
    if (deusModel == NULL) {
        EV_INFO << WIRELESSTRANSPORT_SIMMODULEINFO << "The single Deus model instance not found. Please define one at the network level." << "\n";
        throw cRuntimeError("Check log for details");
    }
}

void WirelessTransport::getAllOtherModels()
{
    // get all own models
    nodeModel = getParentModule();
    numenModel = NULL;
    mobilityModel = NULL;
    for (cModule::SubmoduleIterator it(getParentModule()); !it.end(); it++) {
        cModule *unknownModel = *it;
        if (unknownModel == NULL) {
            continue;
        }
        if (dynamic_cast<Numen*>(unknownModel) != NULL) {
            numenModel = dynamic_cast<Numen*>(unknownModel);
        }
        if (dynamic_cast<inet::IMobility*>(unknownModel) != NULL) {
            mobilityModel = dynamic_cast<inet::IMobility*>(unknownModel);
        }
    }
    if (numenModel == NULL || mobilityModel == NULL) {
        EV_INFO << WIRELESSTRANSPORT_SIMMODULEINFO << "The Numen and/or Mobility model instances not found. They are part of every node model." << "\n";
        throw cRuntimeError("Check log for details");

    }
}

void WirelessTransport::registerWirelessTransportWithDeus()
{

    nodeID = nodeModel->getId();

    // check if node is already registered
    bool found = false;
    NodeInfo *nodeInfo;
    list<NodeInfo*>::iterator iteratorAllNodesList = deusModel->allNodesList.begin();
    while (iteratorAllNodesList != deusModel->allNodesList.end()) {
        nodeInfo = *iteratorAllNodesList;
        if (nodeInfo->nodeID == nodeID) {
            found = true;
            break;
        }

        iteratorAllNodesList++;
    }

    if(!found) {
        nodeInfo = new NodeInfo;
        nodeInfo->nodeID = nodeID;
        nodeInfo->nodeModel = nodeModel;
        nodeInfo->mobilityModel = mobilityModel;
        nodeInfo->numenModel = numenModel;
        deusModel->allNodesList.push_back(nodeInfo);
    }

    WirelessTransportInfo *wirelessTransportInfo = new WirelessTransportInfo;
    wirelessTransportInfo->macAddress = macAddress;
    wirelessTransportInfo->wirelessTransportModel = this;
    wirelessTransportInfo->mobilityModel = mobilityModel;
    wirelessTransportInfo->nodeModel = nodeModel;
    wirelessTransportInfo->numenModel = numenModel;
    wirelessTransportInfo->wirelessTechnology = wirelessTechnology;
    wirelessTransportInfo->operationMode = operationMode;
    wirelessTransportInfo->connectString = connectString;
    nodeInfo->wirelessTransportInfoList.push_back(wirelessTransportInfo);

     // check and update transport belonging to the same wireless group
    found = false;
    SameWirelessGroup *sameWirelessGroup;
    list<SameWirelessGroup*>::iterator iteratorSameWirelessGroupList = deusModel->sameWirelessGroupList.begin();
    while (iteratorSameWirelessGroupList != deusModel->sameWirelessGroupList.end()) {
        sameWirelessGroup = *iteratorSameWirelessGroupList;
        if (sameWirelessGroup->wirelessTechnology ==  wirelessTechnology
                && sameWirelessGroup->operationMode == operationMode
                && sameWirelessGroup->connectString == connectString) {
            found = true;
            break;
        }

        iteratorSameWirelessGroupList++;
    }

    if (!found) {
        sameWirelessGroup = new SameWirelessGroup;
        sameWirelessGroup->wirelessTechnology = wirelessTechnology;
        sameWirelessGroup->operationMode = operationMode;
        sameWirelessGroup->connectString = connectString;
        deusModel->sameWirelessGroupList.push_back(sameWirelessGroup);
    }

    sameWirelessGroup->wirelessTransportInfoList.push_back(wirelessTransportInfo);
}

bool WirelessTransport::inWirelessRange(inet::IMobility *neighbourMobilityModel, inet::IMobility *ownMobilityModel, double radius)
{
    inet::Coord ownCoord, neighCoord;

    ownCoord = ownMobilityModel->getCurrentPosition();
    neighCoord = neighbourMobilityModel->getCurrentPosition();

//    cout << WIRELESSTRANSPORT_SIMMODULEINFO
//            << " neighbour coord (x, y): " << neighCoord.x << ", " << neighCoord.y
//            << " own coord (x, y): " << ownCoord.x << ", " << ownCoord.y
//            << " range: " << radius
//            << "\n";


    double l = ((neighCoord.x - ownCoord.x) * (neighCoord.x - ownCoord.x))
        + ((neighCoord.y - ownCoord.y) * (neighCoord.y - ownCoord.y));
    double r = radius * radius;

    if (l <= r) {
        return true;
    }

    return false;
}

void WirelessTransport::finish()
{

}
