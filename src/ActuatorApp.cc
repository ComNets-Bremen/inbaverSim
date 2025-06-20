//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include "ActuatorApp.h"

Define_Module(ActuatorApp);

void ActuatorApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        gwPrefixName = par("gwPrefixName").stringValue();
        interestLifetime = par("interestLifetime");
        actuatorPrefixName = par("actuatorPrefixName").stringValue();
        segmentSize = par("segmentSize");
        numSegmentsPerFile = par("numSegmentsPerFile");
        cacheTime = par("cacheTime");
        actuatorDataTypes = par("actuatorDataTypes").stringValue();
        maxActuatorDataReadingsToKeep = par("maxActuatorDataReadingsToKeep");
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

       // make list of actuator data types
        string actuatorDataType;
        stringstream stream(actuatorDataTypes);
        while(getline(stream, actuatorDataType, ';')) {
            actuatorDataTypesList.push_back(actuatorDataType);
        }
        if (actuatorDataTypesList.size() <= 0) {
            EV_FATAL << "Actuator data type(s) not defined.\n";
            throw cRuntimeError("Check log for details");

        }

        // since water and electricity has a current meter reading
      //  waterCurrent = par("waterStart");
      //  electricCurrent = par("electricStart");

    } else if (stage == 2) {

        // reminder to generate content host app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(ACTUATORAPP_APP_REG_REMINDER_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // reminder to generate Actuator registration event
        cMessage *actuatorRegReminderEvent = new cMessage("Actuator Registration Reminder Event");
        actuatorRegReminderEvent->setKind(ACTUATOR_REG_REMINDER_CODE);
        scheduleAt(simTime()+1 , actuatorRegReminderEvent);

        // reminder to generate Actuator subscription event
        cMessage *actuatorSubReminderEvent = new cMessage("Actuator Subscription Reminder Event");
        actuatorSubReminderEvent->setKind(ACTUATOR_SUB_REMINDER_CODE);
        scheduleAt(simTime()+2 , actuatorSubReminderEvent);

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

void ActuatorApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == ACTUATORAPP_APP_REG_REMINDER_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        // create prefix
        string servedPrefix = string(getParentModule()->getName());

        // create message
        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setContentServerApp(true);
        appRegMsg->setHostedPrefixNamesArraySize(1);
        appRegMsg->setHostedPrefixNames(0, servedPrefix.c_str());
        appRegMsg->setAppDescription("Actuator Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    }
    else if (msg->isSelfMessage() && msg->getKind() == ACTUATOR_REG_REMINDER_CODE) {

           char completeDataName[64];
           strcpy(completeDataName,"ActuatorRegRequest:");
           strcat(completeDataName,getParentModule()->getName());
           strcat(completeDataName,"-");
           strcat(completeDataName,actuatorDataTypes.c_str());

           //Registration Interest to be sent to the Gateway
           InterestMsg* interestMsg = new InterestMsg("Registration Interest");
           interestMsg->setHopLimit(10);
           interestMsg->setLifetime(simTime() + interestLifetime);
           interestMsg->setPrefixName(gwPrefixName.c_str());
           interestMsg->setDataName(completeDataName);
           interestMsg->setVersionName("v01");
           interestMsg->setSegmentNum(0);
           interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
           interestMsg->setPayloadSize(0);
           interestMsg->setHopsTravelled(0);
           interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
           interestMsg->setReflexiveNamePrefix("0");

           //Store the RNP details
       //    reflexiveNamePrefix = interestMsg->getReflexiveNamePrefix();

           EV_INFO << simTime() << " Sending Registration Interest to the gateway: " << interestMsg->getPrefixName()
                           << " " << interestMsg->getDataName()
                           << " " << interestMsg->getVersionName()
                           << " " << interestMsg->getSegmentNum()
                           << " RNP:" << interestMsg->getReflexiveNamePrefix()
                           << " FPT:" << interestMsg->getForwardPendingToken()
                           << " RPT:" << interestMsg->getReversePendingToken()
                           << endl;

           // send msg to forwarding layer
           send(interestMsg, "forwarderInOut$o");

           delete msg;

    //Send Subscription Interest to Gateway
   } else if (msg->isSelfMessage() && msg->getKind() == ACTUATOR_SUB_REMINDER_CODE) {

       char completeDataName[64];
       strcpy(completeDataName,"ActuatorSubRequest:");
       strcat(completeDataName,getParentModule()->getName());

       //Subscription Interest to be sent to the Gateway
       InterestMsg* interestMsg = new InterestMsg("Subscription Interest");
       interestMsg->setHopLimit(10);
       interestMsg->setLifetime(SIMTIME_MAX);
    // interestMsg->setLifetime(simTime() + interestLifetime);
       interestMsg->setPrefixName(gwPrefixName.c_str());
       interestMsg->setDataName(completeDataName);
       interestMsg->setVersionName("v01");
       interestMsg->setSegmentNum(0);
       interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
       interestMsg->setPayloadSize(0);
       interestMsg->setHopsTravelled(0);
       interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

       //Generate RNP
       string s = to_string(intuniform(1, 16777217));
       char const *random_cstring = s.c_str();

       //Assign RNP
       interestMsg->setReflexiveNamePrefix(random_cstring);

       //Store the RNP details
       reflexiveNamePrefix = interestMsg->getReflexiveNamePrefix();

       EV_INFO << simTime() << " Sending Subscription Interest to the gateway: " << interestMsg->getPrefixName()
                       << " " << interestMsg->getDataName()
                       << " " << interestMsg->getVersionName()
                       << " " << interestMsg->getSegmentNum()
                       << " RNP:" << interestMsg->getReflexiveNamePrefix()
                       << " FPT:" << interestMsg->getForwardPendingToken()
                       << " RPT:" << interestMsg->getReversePendingToken()
                       << endl;

       // send msg to forwarding layer
       send(interestMsg, "forwarderInOut$o");

       delete msg;

    // process other (non-self message type) messages
    } else {

        cGate *arrivalGate;
        char gateName[32];

       // get message arrival gate name
        arrivalGate = msg->getArrivalGate();
        strcpy(gateName, arrivalGate->getName());

        // cast  messages to valid message types
        InterestMsg* interestMsg = dynamic_cast<InterestMsg*>(msg);
        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);

        //If an Interest is received
        if (interestMsg != NULL) {

            EV_INFO << simTime() << " Received Interest: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum() << endl;

            //Check if Prefix name of Reflexive Interest matches the RNP sent
            if(interestMsg->getPrefixName()== reflexiveNamePrefix){

                EV_INFO << simTime() << " RNP and received Interest's Prefix name match!: " << reflexiveNamePrefix <<endl;

                //If it is a Reflexive Interest asking to fetch configuration details
                if (string(interestMsg->getDataName()).find("FetchConfigurations") != string::npos){

                    segmentSize = par("segmentSize");
                    cacheTime = par("cacheTime");

                    //Acknowledge the Reflexive Interest
                    ContentObjMsg* contentObjMsg = new ContentObjMsg("Acknowledgement");
                    contentObjMsg->setPrefixName(interestMsg->getPrefixName());
                    contentObjMsg->setDataName(interestMsg->getDataName());
                    contentObjMsg->setVersionName(interestMsg->getVersionName());
                    contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
                    contentObjMsg->setCachetime(0);
                    contentObjMsg->setExpirytime(0);
            //        contentObjMsg->setCachetime(cacheTime);
            //        contentObjMsg->setExpirytime(simTime() + cacheTime);
                    contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                    contentObjMsg->setPayloadSize(segmentSize);
                    contentObjMsg->setTotalNumSegments(1);
                    contentObjMsg->setPayloadAsString("OK");
                    contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + segmentSize);
                    contentObjMsg->setReversePendingToken(interestMsg->getForwardPendingToken());

                    EV_INFO << simTime() << " Sending ContentObj: "
                                << contentObjMsg->getPrefixName()
                                << " " << contentObjMsg->getDataName()
                                << " " << contentObjMsg->getVersionName()
                                << " " << contentObjMsg->getSegmentNum()
                                << " " << contentObjMsg->getTotalNumSegments()
                                << " " << contentObjMsg->getExpirytime()
                                << " " << contentObjMsg->getPayloadSize()
                                << " RPT: " << contentObjMsg->getReversePendingToken()
                                << " Payload: " << contentObjMsg->getPayloadAsString()
                                << endl;

                    // send msg to forwarding layer
                    send(contentObjMsg, "forwarderInOut$o");

                    //Send Query Interest to fetch the Configuration Data from gateway
                    char completeDataName[64];
                    strcpy(completeDataName,"QueryActuatorCurrentConfig:");
                    strcat(completeDataName,getParentModule()->getName());

                    InterestMsg* interestMsg = new InterestMsg("Query Interest to fetch Configuration Data");
                    interestMsg->setHopLimit(10);
                    interestMsg->setLifetime(simTime() + interestLifetime);
                    interestMsg->setPrefixName(gwPrefixName.c_str());
                    interestMsg->setDataName(completeDataName);
                    interestMsg->setVersionName("v01");
                    interestMsg->setSegmentNum(0);
                    interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                    interestMsg->setPayloadSize(0);
                    interestMsg->setHopsTravelled(0);
                    interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                    interestMsg->setReflexiveNamePrefix("0");

                    //Store the RNP details
                  //  reflexiveNamePrefix = interestMsg->getReflexiveNamePrefix();

                    EV_INFO << simTime() << " Query Interest to fetch Configuration Data: " << interestMsg->getPrefixName()
                                    << " " << interestMsg->getDataName()
                                    << " " << interestMsg->getVersionName()
                                    << " " << interestMsg->getSegmentNum()
                                    << " RNP:" << interestMsg->getReflexiveNamePrefix()
                                    << " FPT:" << interestMsg->getForwardPendingToken()
                                    << " RPT:" << interestMsg->getReversePendingToken()
                                    << endl;

                    // send msg to forwarding layer
                    send(interestMsg, "forwarderInOut$o");

                }

            } //If Prefix name of Reflexive Interest DOESN'T match the RNP sent, send Authentication failed
            else {

                segmentSize = par("segmentSize");
                cacheTime = par("cacheTime");

                //Sending "Authentication failed"
                ContentObjMsg* contentObjMsg = new ContentObjMsg("Authentication failed");
                contentObjMsg->setPrefixName(interestMsg->getPrefixName());
                contentObjMsg->setDataName(interestMsg->getDataName());
                contentObjMsg->setVersionName(interestMsg->getVersionName());
                contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
                contentObjMsg->setCachetime(cacheTime);
                contentObjMsg->setExpirytime(simTime() + cacheTime);
                contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                contentObjMsg->setPayloadSize(segmentSize);
                contentObjMsg->setTotalNumSegments(1);
                contentObjMsg->setPayloadAsString("Authentication failed");
                contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + segmentSize);
                contentObjMsg->setReversePendingToken(interestMsg->getForwardPendingToken());

                EV_INFO << simTime() << " Sending ContentObj: "
                        << contentObjMsg->getPrefixName()
                        << " " << contentObjMsg->getDataName()
                        << " " << contentObjMsg->getVersionName()
                        << " " << contentObjMsg->getSegmentNum()
                        << " " << contentObjMsg->getTotalNumSegments()
                        << " " << contentObjMsg->getExpirytime()
                        << " " << contentObjMsg->getPayloadSize()
                        << " RPT: " << contentObjMsg->getReversePendingToken()
                        << " Payload: " << contentObjMsg->getPayloadAsString()
                        << endl;

                // send msg to forwarding layer
                send(contentObjMsg, "forwarderInOut$o");
            }

            // generate stat for received interest size
            emit(totalInterestsBytesReceivedSignal, (long) interestMsg->getByteLength());

        //If Content Object is received
        } else if (contentObjMsg != NULL) {

            // write stats
            emit(segmentDownloadDurationSignal, (simTime() - contentObjMsg->getSendingTime()));
            emit(totalContentObjsBytesReceivedSignal, (long) contentObjMsg->getByteLength());
            emit(totalDataBytesReceivedSignal, contentObjMsg->getPayloadSize());

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

            if(string(contentObjMsg->getDataName()).find("ActuatorRegRequest") != string::npos){

                EV_INFO << simTime() << "Received Registration Confirmation" << endl;

                //Store the Actuator details(name) in servedActuatorDataList
                ActuatorDataEntry *actuatorDataEntry = new ActuatorDataEntry;
                actuatorDataEntry->actuatorDataName = getParentModule()->getName();
                servedActuatorDataList.push_back(actuatorDataEntry);

            }

            if(string(contentObjMsg->getDataName()).find("QueryActuatorCurrentConfig") != string::npos){

                //Set the corresponding Actuator to the required setting
            //    string payload =contentObjMsg->getPayloadAsString();
           //     string targetActuator = payload.substr(0,payload.find("/"));
            //    bool current_state_boolValue = (payload.substr(payload.find("-")+1) == "1");

                for (int i = 0; i < servedActuatorDataList.size(); i++) {

                    if (string(contentObjMsg->getDataName()).find(servedActuatorDataList[i]->actuatorDataName) != std::string::npos) {

                            //Set the corresponding Actuator to the required setting
                            string payload =contentObjMsg->getPayloadAsString();
                            string targetActuator = payload.substr(0,payload.find("/"));
                            bool current_state_boolValue = (payload.substr(payload.find("/")+1) == "1");

                            EV_INFO << simTime() << " payload " <<  payload
                                                               << " targetActuator " << targetActuator
                                                               << " current_state_boolValue " << current_state_boolValue
                                                               << endl;

                            servedActuatorDataList[i]->actuatorState = current_state_boolValue;
                            servedActuatorDataList[i]->lastActuatorStateTime = simTime().dbl();
                            servedActuatorDataList[i]->lastActuatorStateList.push_back(current_state_boolValue);
                            servedActuatorDataList[i]->lastActuatorStateTimesList.push_back(simTime().dbl());

                            if (servedActuatorDataList[i]->lastActuatorStateList.size() > maxActuatorDataReadingsToKeep) {
                                servedActuatorDataList[i]->lastActuatorStateList.erase(servedActuatorDataList[i]->lastActuatorStateList.begin());
                                servedActuatorDataList[i]->lastActuatorStateTimesList.erase(servedActuatorDataList[i]->lastActuatorStateTimesList.begin());
                            }
                            EV_INFO << simTime() << " Current Setting of " <<  servedActuatorDataList[i]->actuatorDataName
                                    << " is " << servedActuatorDataList[i]->actuatorState << endl;

                            //Keep Track of Content Freshness of Received Content
                        //    demiurgeModel->contentFreshness =  simTime()- sensorDataRecordedTime;
                         //   EV_INFO << simTime() << " Content Freshness Value: " << demiurgeModel->contentFreshness << endl;
                        //    emit(contentFreshnessSignal, demiurgeModel->getContentFreshness());

                            break;


                    }
                }

            }

        } else {

            EV_INFO << " Ignoring message - " << msg->getName() << "\n";

        }
        delete msg;
    }
}

void ActuatorApp::finish()
{

}


