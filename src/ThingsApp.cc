//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "ThingsApp.h"

Define_Module(ThingsApp);

void ThingsApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters (other parameters are volatile and uses distributions therefore read when used)
        requestedSensorNetworkPrefixName = par("requestedSensorNetworkPrefixName").stringValue();
        sensorDataNames = par("sensorDataNames").stringValue();
        maxSensorDataReadingsToKeep = par("maxSensorDataReadingsToKeep");
        maxHopsAllowed = par("maxHopsAllowed");

    } else if (stage == 1) {

        // get Demiurge model instance
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

        // get Numen model instance
        numenModel = NULL;
        for (cModule::SubmoduleIterator it(getParentModule()); !it.end(); it++) {
            cModule *unknownModel = *it;
            if (unknownModel == NULL) {
                continue;
            }
            if (dynamic_cast<Numen*>(unknownModel) != NULL) {
                numenModel = dynamic_cast<Numen*>(unknownModel);
                break;
            }
        }

        // when Demiurge or Numen not found, terminate
        if (demiurgeModel == NULL || numenModel == NULL) {
            EV_FATAL << "The global Demiurge instance and/or node specific Numen instance not found.\n";
            throw cRuntimeError("Check log for details");
        }


        // make list of requested prefixes
        string sensorDataName;
        stringstream stream(sensorDataNames);
        while(getline(stream, sensorDataName, ';')) {
            SensorDataEntry *sensorDataEntry = new SensorDataEntry;
            sensorDataEntry->sensorDataName = sensorDataName;
            servedSensorDataList.push_back(sensorDataEntry);
        }

    } else if (stage == 2) {

        // reminder to generate content host app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(THINGSAPP_APP_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // setup first sensor data download
        cMessage * firstSensorDataDownloadReminderEvent = new cMessage("Sensor Data Download Start Event");
        firstSensorDataDownloadReminderEvent->setKind(THINGSAPP_NEXT_DATA_DOWNLOAD_EVENT_CODE);
        sensorDataRetrievalStartTime = par("sensorDataRetrievalStartTime");
        scheduleAt(simTime() + sensorDataRetrievalStartTime, firstSensorDataDownloadReminderEvent);

        // register stat signals
        totalInterestsBytesSentSignal = registerSignal("appTotalInterestsBytesSent");
        totalContentObjsBytesReceivedSignal = registerSignal("appTotalContentObjsBytesReceived");
        totalDataBytesReceivedSignal = registerSignal("appTotalDataBytesReceived");
        networkInterestRetransmissionCountSignal = registerSignal("appNetworkInterestRetransmissionCount");
        networkInterestInjectedCountSignal = registerSignal("appNetworkInterestInjectedCount");

    } else {
        EV_FATAL << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");
    }
}

void ThingsApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == THINGSAPP_APP_REG_REM_EVENT_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setAppDescription("Sensor Data Retrieval (Things) Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    // request for a single sensor value download
    } else if (msg->isSelfMessage() && msg->getKind() == THINGSAPP_NEXT_DATA_DOWNLOAD_EVENT_CODE) {

        // identify what type of data to download
        int index = par("nextIndexOfSensorDataToRetrieve");
        SensorDataEntry *sensorDataEntry = servedSensorDataList[index];

        InterestMsg* interestMsg = new InterestMsg("Interest");
        interestMsg->setHopLimit(maxHopsAllowed);
        interestMsg->setLifetime(simTime() + par("interestLifetime"));
        interestMsg->setPrefixName(requestedSensorNetworkPrefixName.c_str());
        interestMsg->setDataName(sensorDataEntry->sensorDataName.c_str());
        interestMsg->setVersionName("v01");
        interestMsg->setSegmentNum(0);
        interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
        interestMsg->setPayloadSize(0);
        interestMsg->setHopsTravelled(0);
        interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

//        cout << simTime() << " ==== " << getFullPath()
//                << " ThingsApp: sending interest "
//                << interestMsg->getPrefixName()
//                << " " << interestMsg->getDataName()
//                << endl;

        EV_INFO << simTime() << " Sending Interest: " << interestMsg->getPrefixName()
                << " " << interestMsg->getDataName() << " " << interestMsg->getVersionName()
                << " " << interestMsg->getSegmentNum() << endl;

        // send interest to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // update stats
        demiurgeModel->incrementNetworkInterestInjectedCount();

        // write stats
        emit(totalInterestsBytesSentSignal, (long) interestMsg->getByteLength());
        emit(networkInterestInjectedCountSignal, demiurgeModel->getNetworkInterestInjectedCount());

        scheduleAt(simTime() + par("interDataDownloadInterval"), msg);

    // process the sensor data received
    } else {
        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);


//        cout << simTime() << " ==== " << getFullPath()
//                << " ThingsApp: received content obj "
//                << contentObjMsg->getPrefixName()
//                << " " << contentObjMsg->getDataName()
//                << " payload: " << contentObjMsg->getPayloadAsString()
//                << endl;


        EV_INFO << simTime() << " Received ContentObj: "
                << contentObjMsg->getPrefixName()
                << " " << contentObjMsg->getDataName()
                << " " << contentObjMsg->getVersionName()
                << " " << contentObjMsg->getSegmentNum()
                << " " << contentObjMsg->getTotalNumSegments()
                << " " << contentObjMsg->getExpirytime()
                << " " << contentObjMsg->getPayloadSize()
                << " " << contentObjMsg->getPayloadAsString()
                << endl;

        emit(totalContentObjsBytesReceivedSignal, (long) contentObjMsg->getByteLength());
        emit(totalDataBytesReceivedSignal, contentObjMsg->getPayloadSize());

        EV_INFO << simTime() << " Received sensor data in ContentObj: "
                << contentObjMsg->getPayloadAsString()
                << endl;

        for (int i = 0; i < servedSensorDataList.size(); i++) {

            if (string(contentObjMsg->getDataName()) == servedSensorDataList[i]->sensorDataName) {

                servedSensorDataList[i]->lastDataReading = contentObjMsg->getPayloadAsString();
                servedSensorDataList[i]->lastDataReadingTime = simTime().dbl();
                servedSensorDataList[i]->dataLifetime = contentObjMsg->getCachetime().dbl();
                servedSensorDataList[i]->lastDataReadingsList.push_back(contentObjMsg->getPayloadAsString());
                servedSensorDataList[i]->lastDataReadingTimesList.push_back(simTime().dbl());
                if (servedSensorDataList[i]->lastDataReadingsList.size() > maxSensorDataReadingsToKeep) {
                    servedSensorDataList[i]->lastDataReadingsList.erase(servedSensorDataList[i]->lastDataReadingsList.begin());
                    servedSensorDataList[i]->lastDataReadingTimesList.erase(servedSensorDataList[i]->lastDataReadingTimesList.begin());
                }
                break;
            }
        }


        delete msg;
    }
}

void ThingsApp::finish()
{
}
