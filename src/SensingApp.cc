//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "SensingApp.h"

Define_Module(SensingApp);

void SensingApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        gwPrefixName = par("gwPrefixName").stringValue();
        interestLifetime = par("interestLifetime");
        sensorPrefixName = par("sensorPrefixName").stringValue();
        segmentSize = par("segmentSize");
        numSegmentsPerFile = par("numSegmentsPerFile");
        cacheTime = par("cacheTime");
        sensedDataTypes = par("sensedDataTypes").stringValue();
        wakeupInterval  = par("wakeupInterval");

    } else if (stage == 1) {

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
            EV_FATAL << "The single Demiurge model instance not found. Please define one at the network level.\n";
            throw cRuntimeError("Check log for details");
        }

        // make list of sensed data types
        string sensedDataType;
        stringstream stream(sensedDataTypes);
        while(getline(stream, sensedDataType, ';')) {
            sensedDataTypesList.push_back(sensedDataType);
        }
        if (sensedDataTypesList.size() <= 0) {
            EV_FATAL << "Sensed data type(s) not defined.\n";
            throw cRuntimeError("Check log for details");
        }

        // since water and electricity has a current meter reading
        waterCurrent = par("waterStart");
        electricCurrent = par("electricStart");

    } else if (stage == 2) {

        // reminder to generate content host app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(SENSINGAPP_APP_REG_REMINDER_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // reminder to wake up to sense data and send
        cMessage *wakeupReminderEvent = new cMessage("Wake Up Reminder Event");
        wakeupReminderEvent->setKind(SENSINGAPP_WAKEUP_REMINDER_CODE);
        scheduleAt(simTime() + wakeupInterval, wakeupReminderEvent);

        // register stat signals
        totalInterestsBytesReceivedSignal = registerSignal("appTotalInterestsBytesReceived");
        totalContentObjsBytesSentSignal = registerSignal("appTotalContentObjsBytesSent");
        totalDataBytesSentSignal = registerSignal("appTotalDataBytesSent");

        segmentDownloadDurationSignal = registerSignal("appSegmentDownloadDuration");
        totalInterestsBytesSentSignal = registerSignal("appTotalInterestsBytesSent");
        totalContentObjsBytesReceivedSignal = registerSignal("appTotalContentObjsBytesReceived");
        totalDataBytesReceivedSignal = registerSignal("appTotalDataBytesReceived");
        networkInterestInjectedCountSignal = registerSignal("appNetworkInterestInjectedCount");


    } else {
        EV_FATAL << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");

    }
}

void SensingApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == SENSINGAPP_APP_REG_REMINDER_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        // create prefix
        string servedPrefix = sensorPrefixName + string(getParentModule()->getName());

        // create message
        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setContentServerApp(true);
        appRegMsg->setHostedPrefixNamesArraySize(1);
        appRegMsg->setHostedPrefixNames(0, servedPrefix.c_str());
        appRegMsg->setAppDescription("Data Sensing Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    // generate interest to inform gateway of this sensors presence and saying that
    // new sensor values are ready to sent
    } else if (msg->isSelfMessage() && msg->getKind() == SENSINGAPP_WAKEUP_REMINDER_CODE) {

        // read sensor values
        lastSensorReading = getSensorValues();

        // generate interest to indicate the presence of the sensor and asking
        // the gateway to request for sensor data
        InterestMsg* interestMsg = new InterestMsg("Interest");
        interestMsg->setHopLimit(10);
        interestMsg->setLifetime(simTime() + interestLifetime);
        interestMsg->setPrefixName(gwPrefixName.c_str());
        interestMsg->setDataName(getParentModule()->getName());
        interestMsg->setVersionName("v01");
        interestMsg->setSegmentNum(0);
        interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
        interestMsg->setPayloadSize(0);
        interestMsg->setHopsTravelled(0);
        interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

//        cout << simTime() << " ==== " << getFullPath()
//                << " SensingApp: sending interest "
//                << interestMsg->getPrefixName()
//                << " " << interestMsg->getDataName()
//                << endl;

        EV_INFO << simTime() << " Sending Interest: " << interestMsg->getPrefixName()
                << " " << interestMsg->getDataName()
                << " " << interestMsg->getVersionName()
                << " " << interestMsg->getSegmentNum() << endl;

        // send msg to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // setup wakeup reminder again
        scheduleAt(simTime() + wakeupInterval, msg);

    // process other (non-self message type) messages
    } else {

        cGate *arrivalGate;
        char gateName[32];

       // get message arrival gate name
        arrivalGate = msg->getArrivalGate();
        strcpy(gateName, arrivalGate->getName());

        // cast the message to the valid message type
        InterestMsg* interestMsg = dynamic_cast<InterestMsg*>(msg);
        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);

        if (interestMsg != NULL) {

//            cout << simTime() << " ==== " << getFullPath()
//                    << " SensingApp: received interest "
//                    << interestMsg->getPrefixName()
//                    << " " << interestMsg->getDataName()
//                    << endl;



            EV_INFO << simTime() << " Received Interest: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum() << endl;

            // generate stat for received interest size
            emit(totalInterestsBytesReceivedSignal, (long) interestMsg->getByteLength());

            // create new content obj with the last read sensor values and
            // send to the gateway
            segmentSize = par("segmentSize");
            cacheTime = par("cacheTime");
            ContentObjMsg* contentObjMsg = new ContentObjMsg("Content Object");
            contentObjMsg->setPrefixName(interestMsg->getPrefixName());
            contentObjMsg->setDataName(interestMsg->getDataName());
            contentObjMsg->setVersionName(interestMsg->getVersionName());
            contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
            contentObjMsg->setCachetime(cacheTime);
            contentObjMsg->setExpirytime(simTime() + cacheTime);
            contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
            contentObjMsg->setPayloadSize(segmentSize);
            contentObjMsg->setTotalNumSegments(1);
            contentObjMsg->setPayloadAsString(lastSensorReading.c_str());
            contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + segmentSize);

//            cout << simTime() << " ==== " << getFullPath()
//                    << " SendingApp: sending content obj "
//                    << contentObjMsg->getPrefixName()
//                    << " " << contentObjMsg->getDataName()
//                    << " payload: " << contentObjMsg->getPayloadAsString()
//                    << endl;



            EV_INFO << simTime() << " Sending ContentObj: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum()
                    << " " << contentObjMsg->getTotalNumSegments()
                    << " " << contentObjMsg->getExpirytime()
                    << " " << contentObjMsg->getPayloadSize()
                    << " " << contentObjMsg->getPayloadAsString()
                    << endl;

            // send msg to forwarding layer
            send(contentObjMsg, "forwarderInOut$o");

            // generate stat for sent content obj
            emit(totalContentObjsBytesSentSignal, (long) contentObjMsg->getByteLength());
            emit(totalDataBytesSentSignal, (long) contentObjMsg->getPayloadSize());

        } else if (contentObjMsg != NULL) {

            // write stats
            emit(segmentDownloadDurationSignal, (simTime() - contentObjMsg->getSendingTime()));
            emit(totalContentObjsBytesReceivedSignal, (long) contentObjMsg->getByteLength());
            emit(totalDataBytesReceivedSignal, contentObjMsg->getPayloadSize());

//            cout << simTime() << " ==== " << getFullPath()
//                    << " SendingApp: received content obj "
//                    << contentObjMsg->getPrefixName()
//                    << " " << contentObjMsg->getDataName()
//                    << " payload: " << contentObjMsg->getPayloadAsString()
//                    << endl;


            EV_INFO << simTime() << " Received ContentObj: "
                    << contentObjMsg->getPrefixName()
                    << " " << contentObjMsg->getDataName()
                    << " " << contentObjMsg->getVersionName()
                    << " " << contentObjMsg->getSegmentNum()
                    << " " << contentObjMsg->getTotalNumSegments()
                    << " " << contentObjMsg->getExpirytime()
                    << " " << contentObjMsg->getPayloadSize()
                    << " " << contentObjMsg->getTotalNumSegments()
                    << " " << contentObjMsg->getPayloadAsString()
                    << endl;

        } else {
            EV_INFO << " Ignoring message - " << msg->getName() << "\n";

        }
        delete msg;
    }
}

void SensingApp::finish()
{
    // remove sensed data types list
    sensedDataTypesList.clear();
}


// utility functions
string SensingApp::getSensorValues()
{
    string sensorValuesStr = "";
    double valdouble;
    int valint;
    char buffer[64];
    for (int i = 0; i < sensedDataTypesList.size(); i++) {
        if (i > 0) {
            sensorValuesStr = sensorValuesStr + string(";");
        }
        if (sensedDataTypesList[i] == "temperature") {
            valdouble = par("temperature");
            snprintf(buffer, sizeof(buffer), "%.2f", valdouble);
            sensorValuesStr = sensorValuesStr + string("temperature:") + string(buffer) + string(":celsius");

        } else if (sensedDataTypesList[i] == "humidity") {
            valdouble = par("humidity");
            snprintf(buffer, sizeof(buffer), "%.2f", valdouble);
            sensorValuesStr = sensorValuesStr + string("humidity:") + string(buffer) + string(":percentage");

        } else if (sensedDataTypesList[i] == "vwc") {
            valdouble = par("vwc");
            snprintf(buffer, sizeof(buffer), "%.2f", valdouble);
            sensorValuesStr = sensorValuesStr + string("vwc:") + string(buffer) + string(":percentage");

        } else if (sensedDataTypesList[i] == "salinity") {
            valint = par("salinity");
            snprintf(buffer, sizeof(buffer), "%d", valint);
            sensorValuesStr = sensorValuesStr + string("salinity:") + string(buffer) + string(":uS/cm");

        } else if (sensedDataTypesList[i] == "water") {
            valdouble = par("waterIncrease");
            waterCurrent += valdouble;
            snprintf(buffer, sizeof(buffer), "%.3f", waterCurrent);
            sensorValuesStr = sensorValuesStr + string("water:") + string(buffer) + string(":liters");

        } else if (sensedDataTypesList[i] == "electricity") {
            valdouble = par("electricIncrease");
            electricCurrent += valdouble;
            snprintf(buffer, sizeof(buffer), "%.4f", electricCurrent);
            sensorValuesStr = sensorValuesStr + string("electricity:") + string(buffer) + string(":kWh");

        }
    }

    return sensorValuesStr;
}
