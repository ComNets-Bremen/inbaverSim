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

        // build own (MAC-like) unique address - 0xBB is wireless interface
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

        // register with Numen
        long nodeID = nodeModel->getId();
        deusModel->registerWirelessTransport(nodeID, macAddress, nodeModel, mobilityModel, this, numenModel,
                        wirelessTechnology, operationMode, connectString);

    } else if (stage == 2) {

        // reminder to generate transport registration event
        cMessage *transportRegReminderEvent = new cMessage("Transport Registration Reminder Event");
        transportRegReminderEvent->setKind(WIRELESSTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), transportRegReminderEvent);

    } else {
        EV_INFO << WIRELESSTRANSPORT_SIMMODULEINFO << "unknown initialize() stage" << "\n";
        throw cRuntimeError("Check log for details");

    }
}

void WirelessTransport::handleMessage(cMessage *msg)
{
    // register interface with upper layer (forwarder)
    if (msg->isSelfMessage()) {
        if (msg->getKind() == WIRELESSTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE) {

            TransportRegistrationMsg *transportRegMsg = new TransportRegistrationMsg();
            transportRegMsg->setTransportID(getId());
            transportRegMsg->setTransportDescription("Wireless Transport");
            transportRegMsg->setTransportAddress(ownMACAddress.c_str());

            send(transportRegMsg, "forwarderInOut$o");

        }

        delete msg;

    } else {
        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        if (strstr(gateName, "forwarderInOut") != NULL) {

            processOutgoingMessage(msg);

        } else if (strstr(gateName, "radioIn") != NULL) {




        }
    }
}



void WirelessTransport::processOutgoingMessage(cMessage *msg)
{

    // encapsulate message


    // decide based on type of wireless settings
    if (operationMode == "ap") {

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
        if (!found) {
            delete msg;
            return;
        }

        // make list of nodes that are currently in wireless range of AP
        vector <WirelessTransportInfo*> selectedWirelessNodesList;
        list<WirelessTransportInfo*>::iterator iteratorWirelessTransportInfo = sameWirelessGroup->wirelessTransportInfoList.begin();
        while (iteratorWirelessTransportInfo != sameWirelessGroup->wirelessTransportInfoList.end()) {
            WirelessTransportInfo *wirelessTransportInfo = *iteratorWirelessTransportInfo;

            if (inWirelessRange(wirelessTransportInfo->mobilityModel, mobilityModel, wirelessRange)) {
                selectedWirelessNodesList.push_back(wirelessTransportInfo);
            }

            iteratorWirelessTransportInfo++;
        }

        // send the message to all the nodes














    } else if (operationMode == "client") {

    } else if (operationMode == "direct") {

    }
}

bool WirelessTransport::inWirelessRange(inet::IMobility *neighbourMobilityModel, inet::IMobility *ownMobilityModel, double radius)
{
    inet::Coord ownCoord, neighCoord;

    ownCoord = ownMobilityModel->getCurrentPosition();
    neighCoord = neighbourMobilityModel->getCurrentPosition();

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
