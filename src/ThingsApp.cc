//
// A simple application to retrieve data from a sensor network
// managed by a gateway, that implements the IApplication
// interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 24-feb-2023
//
// @author : Sneha Kulkarni (snku@uni-bremen.de)
// @date   : 31-may-2023
//
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
        interestLifetime = par("interestLifetime");
        maxHopsAllowed = par("maxHopsAllowed");
        subscriptionON = par("subscriptionON");
        subscriptionStartTime = par("subscriptionStartTime");
        subscriptionDuration = par("subscriptionDuration");

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

        // reminder to send subscription interest
        cMessage *subscriptionReminderEvent = new cMessage("Subscription Reminder Event");
        subscriptionReminderEvent->setKind(THINGSAPP_SUBSCRIPTION_REMINDER_CODE);
        scheduleAt(simTime() + subscriptionStartTime, subscriptionReminderEvent);


        // register stat signals
        totalInterestsBytesSentSignal = registerSignal("appTotalInterestsBytesSent");
        totalContentObjsBytesReceivedSignal = registerSignal("appTotalContentObjsBytesReceived");
        totalDataBytesReceivedSignal = registerSignal("appTotalDataBytesReceived");
        networkInterestRetransmissionCountSignal = registerSignal("appNetworkInterestRetransmissionCount");
        networkInterestInjectedCountSignal = registerSignal("appNetworkInterestInjectedCount");
        contentFreshnessSignal = registerSignal("appContentFreshnessSignal");
        subContentObjectReceivedCount = registerSignal("appSubContentObjectsReceivedByUserCount");

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

        InterestMsg* interestMsg = new InterestMsg("Interest requesting Sensor Data");
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
        interestMsg->setReflexiveNamePrefix("0");

        EV_INFO << simTime() << " Sending Interest: " << interestMsg->getPrefixName()
                << " " << interestMsg->getDataName() << " " << interestMsg->getVersionName()
                << " " << interestMsg->getSegmentNum()<<" RNP:"<< interestMsg->getReflexiveNamePrefix()<< endl;

        // send interest to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // update stats
        demiurgeModel->incrementNetworkInterestInjectedCount();

        // write stats
        emit(totalInterestsBytesSentSignal, (long) interestMsg->getByteLength());
        emit(networkInterestInjectedCountSignal, demiurgeModel->getNetworkInterestInjectedCount());

        scheduleAt(simTime() + par("interDataDownloadInterval"), msg);

    }//Subscription reminder
    else if (msg->isSelfMessage() && msg->getKind() == THINGSAPP_SUBSCRIPTION_REMINDER_CODE) {

        //Check if Subscription is ON
        if(subscriptionON == true){

            // identify what type of data to download
            int index = par("nextIndexOfSensorDataToRetrieve");
            SensorDataEntry *sensorDataEntry = servedSensorDataList[index];

            char completeDataName[64];
            strcpy(completeDataName,"SubscriptionRequest:");
            strcat(completeDataName,getParentModule()->getName());
            strcat(completeDataName,"/");
            strcat(completeDataName,sensorDataEntry->sensorDataName.c_str());

            //Send Subscription interest to the gateway
            InterestMsg* interestMsg = new InterestMsg("Subscription Interest");
            interestMsg->setHopLimit(10);
            interestMsg->setLifetime(simTime() + interestLifetime);
            interestMsg->setPrefixName(requestedSensorNetworkPrefixName.c_str());
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

        }
        else{

            EV_INFO << simTime() << " Subscription is turned Off "<<endl;

        }

    }//If interest or Content is received
    else {
        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        // cast to message types for later use
        InterestMsg *interestMsg = dynamic_cast<InterestMsg*>(msg);
        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);
        InterestRtnMsg *interestRtnMsg = dynamic_cast<InterestRtnMsg*>(msg);

        //If interest is received
        if(interestMsg != NULL){

            EV_INFO << simTime() << " Received Interest: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum()
                    << " RNP:" << interestMsg->getReflexiveNamePrefix()
                    << " FPT:" << interestMsg->getForwardPendingToken()
                    << " RPT:" << interestMsg->getReversePendingToken()
                    << endl;

            //If it is a Reflexive interest asking to fetch data from Gateway
            if(string(interestMsg->getDataName()).find("SubscriptionDetails") != string::npos){

                //Store Sensor Data and ID that needs to be used in the Query Interest to be sent

                string dataname =interestMsg->getDataName();
                nextSensorData = dataname.substr(dataname.find(':') + 1 ,dataname.find('/') - dataname.find(':') - 1 );
                nextdataReadingID = stoi(dataname.substr(dataname.find("/")+1));

                //Subscription duration details
                string s = to_string(subscriptionDuration);
                char const *subscription_Duration = s.c_str();

                //form complete Data Name
                char completePayload[64];
                strcpy(completePayload,getParentModule()->getName());
                strcat(completePayload,"/");
                strcat(completePayload,nextSensorData.c_str());
                strcat(completePayload,":");
                strcat(completePayload,subscription_Duration);

                segmentSize = par("segmentSize");
                cacheTime = par("cacheTime");

                //Send Acknowledgement for the Reflexive interest
                ContentObjMsg* contentObjMsg = new ContentObjMsg("Acknowledgement with Subscription Details");
                contentObjMsg->setPrefixName(interestMsg->getPrefixName());
                contentObjMsg->setDataName(interestMsg->getDataName());
                contentObjMsg->setVersionName(interestMsg->getVersionName());
                contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
                contentObjMsg->setCachetime(0); // Cache Time is 0 because it is a response to reflexive Interest
                contentObjMsg->setExpirytime(simTime() + cacheTime);
                contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                contentObjMsg->setPayloadSize(segmentSize);
                contentObjMsg->setTotalNumSegments(1);
                contentObjMsg->setPayloadAsString(completePayload);
                contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + segmentSize);
                contentObjMsg->setReversePendingToken(interestMsg->getForwardPendingToken());

                EV_INFO << simTime() << " Acknowledgement with Subscription Details: "
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


                //Send Query Interest to fetch the Subscribed Data
                char completeDataname[64];
                strcpy(completeDataname,nextSensorData.c_str());
                strcat(completeDataname,"/");
                char nextdatareadingID[16];
                sprintf(nextdatareadingID, "%d", nextdataReadingID);
                strcat(completeDataname,nextdatareadingID);

                InterestMsg* interestMsg = new InterestMsg("Query Interest to fetch Subscribed Data");
                interestMsg->setHopLimit(10);
                interestMsg->setLifetime(simTime() + interestLifetime);
                interestMsg->setPrefixName(requestedSensorNetworkPrefixName.c_str());
                interestMsg->setDataName(completeDataname);
                interestMsg->setVersionName("v01");
                interestMsg->setSegmentNum(0);
                interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                interestMsg->setPayloadSize(0);
                interestMsg->setHopsTravelled(0);
                interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                interestMsg->setReflexiveNamePrefix("0");

                //Store the RNP details
                reflexiveNamePrefix = interestMsg->getReflexiveNamePrefix();

                EV_INFO << simTime() << " Query Interest to fetch Subscribed Data: " << interestMsg->getPrefixName()
                                << " " << interestMsg->getDataName()
                                << " " << interestMsg->getVersionName()
                                << " " << interestMsg->getSegmentNum()
                                << " RNP:" << interestMsg->getReflexiveNamePrefix()
                                << " FPT:" << interestMsg->getForwardPendingToken()
                                << " RPT:" << interestMsg->getReversePendingToken()
                                << endl;

                // send msg to forwarding layer
                send(interestMsg, "forwarderInOut$o");

            } // else, send interest return
            else {

                InterestRtnMsg* interestRtnMsg = new InterestRtnMsg("Interest Return");
                interestRtnMsg->setReturnCode(ReturnCodeTypeNoRoute);
                interestRtnMsg->setPrefixName(interestMsg->getPrefixName());
                interestRtnMsg->setDataName(interestMsg->getDataName());
                interestRtnMsg->setVersionName(interestMsg->getVersionName());
                interestRtnMsg->setSegmentNum(interestMsg->getSegmentNum());
                interestRtnMsg->setHeaderSize(INBAVER_INTEREST_RTN_MSG_HEADER_SIZE);
                interestRtnMsg->setPayloadSize(0);
                interestRtnMsg->setByteLength(INBAVER_INTEREST_RTN_MSG_HEADER_SIZE);
                interestRtnMsg->setReflexiveNamePrefix("0");

                EV_INFO << simTime() << " Sending InterestRtn: "
                        << interestRtnMsg->getPrefixName()
                        << " " << interestRtnMsg->getDataName()
                        << " " << interestRtnMsg->getVersionName()
                        << " " << interestRtnMsg->getSegmentNum()
                        << " " << interestRtnMsg->getReturnCode() << endl;

                // send msg to forwarding layer
                send(interestRtnMsg, "forwarderInOut$o");
            }
        }

        else if (contentObjMsg != NULL) {

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

            //If Content Object is a Subscription Content Object, record stats
            if (string(contentObjMsg->getDataName()).find("/") != string::npos) {

                // update stats
                demiurgeModel->incrementSubContentobjectReceivedCount();

                // write stats
                emit(subContentObjectReceivedCount, demiurgeModel->getSubContentobjectReceivedCount());

            }

            EV_INFO << simTime() << " Received sensor data in ContentObj: "
                    << contentObjMsg->getPayloadAsString()
                    << endl;

            for (int i = 0; i < servedSensorDataList.size(); i++) {

                if (string(contentObjMsg->getDataName()).find(servedSensorDataList[i]->sensorDataName) != std::string::npos) {

                    //If it is a data with Record ID
                    if(string(contentObjMsg->getPayloadAsString()).find("/") != string::npos){

                        string payload =contentObjMsg->getPayloadAsString();
                        string dataReading = payload.substr(0,payload.find("/"));
                        int dataReadingID = stoi(payload.substr(payload.find("/") + 1, payload.find("-") - payload.find("/") - 1));
                        double sensorDataRecordedTime = stod(payload.substr(payload.find("-")+1));

                        servedSensorDataList[i]->lastDataReading = dataReading;
                        servedSensorDataList[i]->lastDataReadingID = dataReadingID;
                        servedSensorDataList[i]->lastDataReadingTime = simTime().dbl();
                        servedSensorDataList[i]->dataLifetime = contentObjMsg->getCachetime().dbl();
                        servedSensorDataList[i]->lastDataReadingsList.push_back(contentObjMsg->getPayloadAsString());
                        servedSensorDataList[i]->lastDataReadingTimesList.push_back(simTime().dbl());
                        if (servedSensorDataList[i]->lastDataReadingsList.size() > maxSensorDataReadingsToKeep) {
                            servedSensorDataList[i]->lastDataReadingsList.erase(servedSensorDataList[i]->lastDataReadingsList.begin());
                            servedSensorDataList[i]->lastDataReadingTimesList.erase(servedSensorDataList[i]->lastDataReadingTimesList.begin());
                        }
                        EV_INFO << simTime() << " lastDataReading: " << servedSensorDataList[i]->lastDataReading
                                << " lastDataReadingID: " << servedSensorDataList[i]->lastDataReadingID
                                << " Sensor Data Recoded Time: " << sensorDataRecordedTime << endl;

                        //Keep Track of Content Freshness of Received Content
                        demiurgeModel->contentFreshness =  simTime()- sensorDataRecordedTime;
                        EV_INFO << simTime() << " Content Freshness Value: " << demiurgeModel->contentFreshness << endl;
                        emit(contentFreshnessSignal, demiurgeModel->getContentFreshness());

                        break;
                                           }

                }
            }

        }
        else if (interestRtnMsg != NULL){

            EV_INFO << simTime() << " Received Interest Return: " << interestRtnMsg->getPrefixName()
                            << " " << interestRtnMsg->getDataName()
                            << " " << interestRtnMsg->getVersionName()
                            << " " << interestRtnMsg->getSegmentNum()
                            << " " << interestRtnMsg->getReflexiveNamePrefix()
                            << endl;

        }
         delete msg;

    }
}

void ThingsApp::finish()
{
}
