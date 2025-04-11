//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "WiredTransport.h"

Define_Module(WiredTransport);

void WiredTransport::initialize(int stage)
{
    if (stage == 0) {
        // get all parameters
        wiredTechnology = par("wiredTechnology").stringValue();
        dataRate = par("dataRate");
        packetErrorRate = par("packetErrorRate");
        headerSize = par("headerSize");


    } else if (stage == 1) {

        // initialize variables
        buildMACLikeAddress();

        // get references to models
        getDemiurgeModel();
        getAllOtherModels();

        // register the transport with Demiurge
        registerWiredTransportWithDemiurge();

    } else if (stage == 2) {

        // reminder to generate transport registration event
        cMessage *transportRegReminderEvent = new cMessage("Transport Registration Reminder Event");
        transportRegReminderEvent->setKind(WIREDTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), transportRegReminderEvent);

        // setup pkt send event message
        msgSendCompletedEvent = new cMessage("Message Send Completed Event");
        msgSendCompletedEvent->setKind(WIREDTRANSPORT_MSG_SEND_COMPLETED_EVENT_CODE);

        // init stat signals
        totalWiredBytesReceivedSignal = registerSignal("transportTotalWiredBytesReceived");
        totalWiredBytesSentSignal = registerSignal("transportTotalWiredBytesSent");

    } else {
        EV_FATAL << simTime() << "unknown initialize() stage"
                << "\n";
        throw cRuntimeError("Check log for details");

    }
}

void WiredTransport::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {

        // register transport with upper layer (forwarder)
        if (msg->getKind() == WIREDTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE) {

            TransportRegistrationMsg *transportRegMsg = new TransportRegistrationMsg();
            transportRegMsg->setTransportID(getId());
            transportRegMsg->setTransportDescription("Wired Transport");
            transportRegMsg->setTransportAddress(macAddress.c_str());

            EV_INFO << simTime() << " Registering transport with ID: " << getId() << endl;

            send(transportRegMsg, "forwarderInOut$o");

            delete msg;

        // last message sent, see if queued messages exist
        } else if (msg->getKind() == WIREDTRANSPORT_MSG_SEND_COMPLETED_EVENT_CODE) {

            // are there queued messages? if so, pop
            // and send message out
            if (!messageQueue.empty()) {

                // pop queued message
                cMessage *queuedMsg = messageQueue.front();
                messageQueue.pop();

                // send message immediately out
                sendOutgoingMessage(queuedMsg);
            }
        }

    } else {
        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        // process message
        if (strstr(gateName, "forwarderInOut") != NULL) {

            processOutgoingMessage(msg);

        } else if (strstr(gateName, "physicalInOut") != NULL) {

            processIncomingMessage(msg);

        }
    }
}

void WiredTransport::processOutgoingMessage(cMessage *msg)
{

    EV_INFO << simTime() << "Received outgoing message: "
            << msg->getName()
            << endl;

    // is there a message send in progress
    if (msgSendCompletedEvent->isScheduled()) {

        EV_INFO << simTime() << "Queuing outgoing message: "
                << msg->getName()
                << endl;

        // queue the message until current sent
        messageQueue.push(msg);

    } else {

        // send message immediately out
        sendOutgoingMessage(msg);

    }
}

void WiredTransport::sendOutgoingMessage(cMessage *msg)
{
    // get CCNx message size
    long msgSize = ((cPacket *)msg)->getByteLength();

    // get destination address if given
    ExchangedTransportInfo *destinationTransportInfo = NULL;
    if (msg->hasObject("ExchangedTransportInfo")) {
        destinationTransportInfo = check_and_cast<ExchangedTransportInfo*>(msg->getObject("ExchangedTransportInfo"));
        msg->removeObject("ExchangedTransportInfo");
    }

    // make copy of original message
    cMessage *copyOfMsg = msg->dup();

    // create message
    TransportMsg *transportMsg = new TransportMsg(copyOfMsg->getName());
    transportMsg->setSourceAddress(macAddress.c_str());
    if (destinationTransportInfo != NULL) {
        transportMsg->setBroadcastMsg(false);
        transportMsg->setDestinationAddress(destinationTransportInfo->transportAddress.c_str());
    } else {
        transportMsg->setBroadcastMsg(true);
        transportMsg->setDestinationAddress("");
    }
    transportMsg->encapsulate((cPacket *) copyOfMsg);
    transportMsg->setHeaderSize(headerSize);
    transportMsg->setPayloadSize(msgSize);
    transportMsg->setByteLength(headerSize + msgSize);

    EV_INFO << simTime() << "Sending outgoing message: "
            << msg->getName()
            << " " << macAddress
            << " " << transportMsg->getBroadcastMsg()
            << " " << transportMsg->getDestinationAddress()
            << endl;


    // send msg directly to node
    send(transportMsg, "physicalInOut$o");

    // generate stats
    emit(totalWiredBytesSentSignal, (long) transportMsg->getByteLength());

    // remove original msg and return
    if (destinationTransportInfo) {
        delete destinationTransportInfo;
    }

    // set send message completed timeout
    cChannel *txChannel = gate("physicalInOut$o")->getTransmissionChannel();
    simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
    scheduleAt(txFinishTime, msgSendCompletedEvent);

    delete msg;

}

void WiredTransport::processIncomingMessage(cMessage *msg)
{
    // check message, must be a transport message
    TransportMsg *transportMsg;
    if ((transportMsg = dynamic_cast<TransportMsg*>(msg)) == NULL) {
        delete msg;
        return;
    }

    // generate stats
    emit(totalWiredBytesReceivedSignal, (long) transportMsg->getByteLength());

    // get source of the msg
    ExchangedTransportInfo *arrivalTransportInfo = new ExchangedTransportInfo("ExchangedTransportInfo");
    arrivalTransportInfo->transportAddress = transportMsg->getSourceAddress();

    // decapsulate and get original packet
    cMessage *decapsulatedMsg = transportMsg->decapsulate();

    EV_INFO << simTime() << "Received incoming message: "
            << decapsulatedMsg->getName()
            << " " << transportMsg->getSourceAddress()
            << " " << transportMsg->getBroadcastMsg()
            << " " << transportMsg->getDestinationAddress()
            << endl;

    // attach source info
    decapsulatedMsg->addObject(arrivalTransportInfo);

    // send decapsulated msg to forwarding
    send(decapsulatedMsg, "forwarderInOut$o");

    delete msg;
}

void WiredTransport::buildMACLikeAddress()
{
    // build own (MAC-like) unique address - 0xAA is for wired transports
    char str[24];
    long ifcID = getId();
    int len = sizeof(ifcID) - 1;
    unsigned char numList[] = {0xAA, 0, 0, 0, 0, 0};
    for (int i = 5; i >= 1 && len >= 0; i--) {
        unsigned char num = ifcID & 0xFF;
        numList[i] = num;
        ifcID = ifcID >> 8;
        len--;
    }
    snprintf(str, sizeof(str), "%02x:%02x:%02x:%02x:%02x:%02x", numList[0], numList[1],
                                               numList[2],numList[3],
                                               numList[4], numList[5]);
    macAddress = str;
}

void WiredTransport::getDemiurgeModel()
{
    // get Demiurge
    demiurgeModel = NULL;
    for (int id = 0; id <= getSimulation()->getLastComponentId(); id++) {
        cModule *unknownModel = getSimulation()->getModule(id);
        if (unknownModel == NULL) {
            continue;
        }
        if (dynamic_cast<Demiurge*>(unknownModel) != NULL) {
            demiurgeModel = dynamic_cast<Demiurge*>(unknownModel);
            break;
        }
    }
    if (demiurgeModel == NULL) {
        EV_FATAL << simTime() << "The single Demiurge model instance not found. Please define one at the network level." << "\n";
        throw cRuntimeError("Check log for details");
    }
}

void WiredTransport::getAllOtherModels()
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
        EV_FATAL << simTime() << "The Numen and/or Mobility model instances not found. They are part of every node model." << "\n";
        throw cRuntimeError("Check log for details");

    }
}


void WiredTransport::registerWiredTransportWithDemiurge()
{

    nodeID = nodeModel->getId();

    // check if node is already registered
    bool found = false;
    NodeInfo *nodeInfo;
    list<NodeInfo*>::iterator iteratorAllNodesList = demiurgeModel->allNodesList.begin();
    while (iteratorAllNodesList != demiurgeModel->allNodesList.end()) {
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
        demiurgeModel->allNodesList.push_back(nodeInfo);
    }

    WiredTransportInfo *wiredTransportInfo = new WiredTransportInfo;
    wiredTransportInfo->macAddress = macAddress;
    wiredTransportInfo->wiredTransportModel = this;
    wiredTransportInfo->mobilityModel = mobilityModel;
    wiredTransportInfo->nodeModel = nodeModel;
    wiredTransportInfo->numenModel = numenModel;
    nodeInfo->wiredTransportInfoList.push_back(wiredTransportInfo);
}

void WiredTransport::finish()
{
    // stop msg send trigger, if active
    if (msgSendCompletedEvent->isScheduled()) {
        cancelEvent(msgSendCompletedEvent);
    }

    // remove msg send trigger
    delete msgSendCompletedEvent;

    // remove queued messages
    while (!messageQueue.empty()) {

        // pop queued message
        cMessage *queuedMsg = messageQueue.front();
        messageQueue.pop();

        if (queuedMsg->hasObject("ExchangedTransportInfo")) {
            ExchangedTransportInfo *destinationTransportInfo = check_and_cast<ExchangedTransportInfo*>(queuedMsg->getObject("ExchangedTransportInfo"));
            queuedMsg->removeObject("ExchangedTransportInfo");
            delete destinationTransportInfo;
        }

        delete queuedMsg;

    }
}
