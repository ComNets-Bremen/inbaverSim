//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "IoTGatewayApp.h"

Define_Module(IoTGatewayApp);

void IoTGatewayApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        hostedPrefixName = par("hostedPrefixName").stringValue();
        hostedPrefixNameForSensorNet = par("hostedPrefixNameForSensorNet").stringValue();
        sensorPrefixName = par("sensorPrefixName").stringValue();
        interestLifetime = par("interestLifetime");
        maximumSensorReadingsToKeep = par("maximumSensorReadingsToKeep");

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

    } else if (stage == 2) {

        // reminder to generate IoT gateway app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(IOTGATEWAYAPP_APP_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // register stat signals
        totalInterestsBytesReceivedSignal = registerSignal("appTotalInterestsBytesReceived");
        totalContentObjsBytesSentSignal = registerSignal("appTotalContentObjsBytesSent");
        totalDataBytesSentSignal = registerSignal("appTotalDataBytesSent");

    } else {
        EV_FATAL << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");

    }
}

void IoTGatewayApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == IOTGATEWAYAPP_APP_REG_REM_EVENT_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setContentServerApp(true);
        appRegMsg->setHostedPrefixNamesArraySize(2);
        appRegMsg->setHostedPrefixNames(0, hostedPrefixName.c_str());
        appRegMsg->setHostedPrefixNames(1, hostedPrefixNameForSensorNet.c_str());
        EV_INFO << simTime() << " Registering prefix: " << hostedPrefixName << endl;
        EV_INFO << simTime() << " Registering prefix: " << hostedPrefixNameForSensorNet << endl;
        appRegMsg->setAppDescription("IoT Gateway Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    // process other messages (non-self message types)
    } else {

        cGate *arrivalGate;
        char gateName[32];

        // get message arrival gate name
        arrivalGate = msg->getArrivalGate();
        strcpy(gateName, arrivalGate->getName());

        // cast to message types for later use
        InterestMsg *interestMsg = dynamic_cast<InterestMsg*>(msg);
        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);

        // process Interests received
        if (interestMsg != NULL) {

//            cout << simTime() << " ==== " << getFullPath()
//                    << " IoTGatewayApp: received interest "
//                    << interestMsg->getPrefixName()
//                    << " " << interestMsg->getDataName()
//                    << endl;


            EV_INFO << simTime() << " Received Interest: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum() << endl;

            // Interest by a sensor indicating its presence and saying it woke up and wants to send values
            if (string(interestMsg->getPrefixName()).find(hostedPrefixNameForSensorNet) != string::npos) {

                // update the sensor name as a sensor in the network
                int index = -1;
                for (int i = 0; i < servedSensorList.size(); i++) {
                    if (string(interestMsg->getDataName()) == servedSensorList[i]->sensorName) {
                        index = i;
                        break;
                    }
                }
                if (index == -1) {
                    // generate the prefix that reaches the sensor
                    string completePrefixName = sensorPrefixName + string(interestMsg->getDataName());

                    SensorEntry* sensorEnry = new SensorEntry;
                    sensorEnry->sensorName = string(interestMsg->getDataName());
                    sensorEnry->sensorPrefixName = completePrefixName;
                    sensorEnry->faceID = interestMsg->getArrivalFaceID();
                    sensorEnry->faceType = interestMsg->getArrivalFaceType();
                    sensorEnry->prefixRegistered = false;
                    servedSensorList.push_back(sensorEnry);
                    index = servedSensorList.size() - 1;
                }

                // reply with a content obj
                ContentObjMsg* contentObjMsg = new ContentObjMsg("Content Object");
                contentObjMsg->setPrefixName(interestMsg->getPrefixName());
                contentObjMsg->setDataName(interestMsg->getDataName());
                contentObjMsg->setVersionName(interestMsg->getVersionName());
                contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
                contentObjMsg->setCachetime(interestLifetime);
                contentObjMsg->setExpirytime(simTime() + interestLifetime);
                contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                contentObjMsg->setPayloadSize(8);
                contentObjMsg->setTotalNumSegments(1);
                contentObjMsg->setPayloadAsString("OK");
                contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);

//                cout << simTime() << " ==== " << getFullPath()
//                        << " IoTGatewayApp: sending content obj "
//                        << contentObjMsg->getPrefixName()
//                        << " " << contentObjMsg->getDataName()
//                        << " payload: " << contentObjMsg->getPayloadAsString()
//                        << endl;


                EV_INFO << simTime() << " Sending ContentObj: "
                        << interestMsg->getPrefixName()
                        << " " << interestMsg->getDataName()
                        << " " << interestMsg->getVersionName()
                        << " " << interestMsg->getSegmentNum()
                        << " " << contentObjMsg->getExpirytime()
                        << " " << contentObjMsg->getPayloadSize()
                        << " " << contentObjMsg->getPayloadAsString()
                        << endl;

                // send msg to forwarding layer
                send(contentObjMsg, "forwarderInOut$o");

                // register the sensor's prefix in the FIB of the forwarding layer
                // so that interests to the sensor gets directed there
                if (!servedSensorList[index]->prefixRegistered) {

                    EV_INFO << simTime() << " Registering new prefix at forwarding (FIB): "
                            << servedSensorList[index]->sensorPrefixName
                            << " " << servedSensorList[index]->faceID
                            << " " << servedSensorList[index]->faceType
                            << endl;

                    PrefixRegistrationMsg *prefixRegMsg = new PrefixRegistrationMsg();
                    prefixRegMsg->setPrefixName(servedSensorList[index]->sensorPrefixName.c_str());
                    prefixRegMsg->setFaceID(servedSensorList[index]->faceID);

                    send(prefixRegMsg, "forwarderInOut$o");

                    servedSensorList[index]->prefixRegistered = true;
                }

                // generate a new Interest to retrieve the latest sensor values
                InterestMsg* interestMsgNew = new InterestMsg("Interest");
                interestMsgNew->setHopLimit(10);
                interestMsgNew->setLifetime(simTime() + interestLifetime);
                interestMsgNew->setPrefixName(servedSensorList[index]->sensorPrefixName.c_str());
                interestMsgNew->setDataName("data");
                interestMsgNew->setVersionName("v01");
                interestMsgNew->setSegmentNum(0);
                interestMsgNew->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                interestMsgNew->setPayloadSize(0);
                interestMsgNew->setHopsTravelled(0);
                interestMsgNew->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

//                cout << simTime() << " ==== " << getFullPath()
//                        << " IoTGatewayApp: sending interest "
//                        << interestMsgNew->getPrefixName()
//                        << " " << interestMsgNew->getDataName()
//                        << endl;


                EV_INFO << simTime() << " Sending Interest: " << interestMsgNew->getPrefixName()
                        << " " << interestMsgNew->getDataName()
                        << " " << interestMsgNew->getVersionName()
                        << " " << interestMsgNew->getSegmentNum()
                        << " " << interestMsgNew->getPayloadSize() << endl;

                // send new interest msg to forwarding layer
                send(interestMsgNew, "forwarderInOut$o");

            //  request for stored sensor data at gateway from outside (e.g. Internet)
            } else if (string(interestMsg->getPrefixName()).find(hostedPrefixName) != string::npos) {

                // find the sensor entry
                int index = -1;
                string sensedData = "";
                for (int i = 0; i < servedSensorList.size(); i++) {
                    stringstream stream(servedSensorList[i]->lastSensorReading);
                    while(getline(stream, sensedData, ';')) {
                        if (sensedData.find(string(interestMsg->getDataName())) != string::npos) {
                            index = i;
                            break;
                        }
                    }
                    if (index != -1) {
                        break;
                    }
                }

                // if requested sensor data is found, send Content Obj
                if (index != -1) {
                    // reply with a content obj
                    ContentObjMsg* contentObjMsg = new ContentObjMsg("Content Object");
                    contentObjMsg->setPrefixName(interestMsg->getPrefixName());
                    contentObjMsg->setDataName(interestMsg->getDataName());
                    contentObjMsg->setVersionName(interestMsg->getVersionName());
                    contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
                    contentObjMsg->setCachetime(servedSensorList[index]->dataLifetime);
                    contentObjMsg->setExpirytime(simTime() + servedSensorList[index]->dataLifetime);
                    contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                    contentObjMsg->setPayloadSize(servedSensorList[index]->dataSize);
                    contentObjMsg->setTotalNumSegments(1);
                    contentObjMsg->setPayloadAsString(sensedData.c_str());
                    contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);


//                    cout << simTime() << " ==== " << getFullPath()
//                            << " IoTGatewayApp: sending content obj "
//                            << contentObjMsg->getPrefixName()
//                            << " " << contentObjMsg->getDataName()
//                            << " payload: " << contentObjMsg->getPayloadAsString()
//                            << endl;


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

                // if not found, send an Interest Return
                } else {
                    EV_FATAL << "Requested sensor data not found - " << interestMsg->getDataName() << endl;
                    throw cRuntimeError("Check log for details");

                }
            } else {
                EV_INFO << " Ignoring Interest message - "
                        << interestMsg->getPrefixName()
                        << endl;

            }


        // process content obj received from sensor
        } if (contentObjMsg != NULL) {

//            cout << simTime() << " ==== " << getFullPath()
//                    << " IoTGatewayApp: received content obj "
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

            // find the sensor entry
            int index = -1;
            for (int i = 0; i < servedSensorList.size(); i++) {
                if(string(contentObjMsg->getPrefixName()).find(servedSensorList[i]->sensorName) != string::npos) {
                    index = i;
                    break;
                }
            }
            if (index != -1) {
                servedSensorList[index]->lastSensorReading = contentObjMsg->getPayloadAsString();
                servedSensorList[index]->dataLifetime = contentObjMsg->getCachetime().dbl();
                servedSensorList[index]->dataSize = contentObjMsg->getPayloadSize();
                servedSensorList[index]->lastSensorReadingsList.push_back(contentObjMsg->getPayloadAsString());
                if (servedSensorList[index]->lastSensorReadingsList.size() > maximumSensorReadingsToKeep) {
                    servedSensorList[index]->lastSensorReadingsList.erase(servedSensorList[index]->lastSensorReadingsList.begin());
                }
            } else {
                EV_FATAL << "Corresponding entry in sensor data not found for " << contentObjMsg->getPrefixName() << endl;
                throw cRuntimeError("Check log for details");

            }

        }

        delete msg;
    }
}

void IoTGatewayApp::finish()
{
    // remove sensor info from list
    for (int i = 0; i < servedSensorList.size(); i++) {
        servedSensorList[i]->lastSensorReadingsList.clear();
        delete servedSensorList[i];
    }
    servedSensorList.clear();

}
