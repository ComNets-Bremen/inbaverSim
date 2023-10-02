//
// An application that connects an IoT with the Internet
// and is able to handle the functionality required to
// manage the duty cycled IoT and the permently-on
// Internet. It implements the IApplication interface. It uses
//Reflexive Forwarding.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 19-feb-2023
//
// @author : Sneha Kulkarni (snku@uni-bremen.de)
// @date   : 31-may-2023
//
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
        sensorDataRetrievalStartTime = par("sensorDataRetrievalStartTime");
        maxHopsAllowed = par("maxHopsAllowed");
        otherGatewayAvailable = par("otherGatewayAvailable").stringValue();

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

        // reminder to generate query message for Gateway to Gateway communication event
        cMessage *queryOtherGatewayEvent = new cMessage("Reminder Event to query the other Gateway");
        queryOtherGatewayEvent->setKind(IOTGATEWAYAPP_QUERY_OTHER_GATEWAY_EVENT_CODE);
        scheduleAt(simTime()+ sensorDataRetrievalStartTime, queryOtherGatewayEvent);

        // reminder that subscription by a particular user has ended
        endOfUserSubscriptionEvent = new cMessage("End of User Subscription Reminder Event");
        endOfUserSubscriptionEvent->setKind(IOTGATEWAYAPP_SUB_END_REMINDER_EVENT_CODE);

        // register stat signals
        totalInterestsBytesReceivedSignal = registerSignal("appTotalInterestsBytesReceived");
        totalContentObjsBytesSentSignal = registerSignal("appTotalContentObjsBytesSent");
        totalDataBytesSentSignal = registerSignal("appTotalDataBytesSent");
        subContentObjectsSentCount = registerSignal("appSubContentObjectsSentToUserCount");

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

    }//Reminder to send Final Acknowledgement at the end of Subscription Duration
    else if (msg->isSelfMessage() && msg->getKind() == IOTGATEWAYAPP_SUB_END_REMINDER_EVENT_CODE){

        //Send Final Acknowledgement to the user
        ContentObjMsg* contentObjMsg = new ContentObjMsg("Final Acknowledgement Content Object");
        contentObjMsg->setPrefixName(subscriptionExpiryData[0]->subscriptionExpPrefixName.c_str());
        contentObjMsg->setDataName(subscriptionExpiryData[0]->subscriptionExpDataName.c_str());
        contentObjMsg->setVersionName("v01");
        contentObjMsg->setSegmentNum(0);
        contentObjMsg->setCachetime(interestLifetime);
        contentObjMsg->setExpirytime(simTime() + interestLifetime);
        contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
        contentObjMsg->setPayloadSize(8);
        contentObjMsg->setTotalNumSegments(1);
        contentObjMsg->setPayloadAsString("Final Acknowledgement");
        contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);
        contentObjMsg->setReversePendingToken(subscriptionExpiryData[0]->subscriptionExpFPT);

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

        //  delete the Subscription expiry details record of the User to whom the Final Acknowledgement is already sent
        subscriptionExpiryData.erase(subscriptionExpiryData.begin());


        //Re-schedule de-subscription reminder if any more pending de-subscriptions are present and Delete the first record after scheduling the event
        if (!subscriptionExpiryData.empty()) {

            scheduleAt(subscriptionExpiryData[0]->subscriptionExpiryTime, endOfUserSubscriptionEvent);
            EV_INFO <<" End Of User Subscription Event scheduled at: "<< subscriptionExpiryData[0]->subscriptionExpiryTime <<endl;

        }


    } //Reminder to Query the the other Gateway (Scenario 3 of the Use case)
    else if (msg->isSelfMessage() && msg->getKind() == IOTGATEWAYAPP_QUERY_OTHER_GATEWAY_EVENT_CODE){

        // identify what type of data to download
        int index = par("nextIndexOfSensorDataToRetrieve");
        string requestingData = par("othersensorDataName");

        InterestMsg* interestMsg = new InterestMsg("Interest requesting Sensor Data");
        interestMsg->setHopLimit(maxHopsAllowed);
        interestMsg->setLifetime(simTime() + par("interestLifetime"));
        interestMsg->setPrefixName(otherGatewayAvailable.c_str());
        interestMsg->setDataName(requestingData.c_str());
        interestMsg->setVersionName("v01");
        interestMsg->setSegmentNum(0);
        interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
        interestMsg->setPayloadSize(0);
        interestMsg->setHopsTravelled(0);
        interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
        interestMsg->setReflexiveNamePrefix("0");

        EV_INFO << simTime() << " Sending Interest to Other Gateway to retrieve Data: " << interestMsg->getPrefixName()
                << " " << interestMsg->getDataName() << " " << interestMsg->getVersionName()
                << " " << interestMsg->getSegmentNum()<<" RNP: "<< interestMsg->getReflexiveNamePrefix()<< endl;

        // send interest to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // update stats
        demiurgeModel->incrementNetworkInterestInjectedCount();

        scheduleAt(simTime() + par("interDataDownloadInterval"), msg);

    }// process other messages (non-self message types)
    else {

        cGate *arrivalGate;
        char gateName[32];

        // get message arrival gate name
        arrivalGate = msg->getArrivalGate();
        strcpy(gateName, arrivalGate->getName());

        // cast to message types for later use
        InterestMsg *interestMsg = dynamic_cast<InterestMsg*>(msg);
        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);
        InterestRtnMsg *interestRtnMsg = dynamic_cast<InterestRtnMsg*>(msg);

        // process Interests received
        if (interestMsg != NULL) {

            EV_INFO << simTime() << " Received Interest: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum()
                    << " RNP:" << interestMsg->getReflexiveNamePrefix()
                    << " FPT:" << interestMsg->getForwardPendingToken()
                    << " RPT:" << interestMsg->getReversePendingToken()
                    << endl;

            // Interest by a sensor from Sensor Network
            if (string(interestMsg->getPrefixName()).find(hostedPrefixNameForSensorNet) != string::npos) {

                //To check if interest is an Registration Interest
                if(string(interestMsg->getDataName()).find("RegistrationRequest") != string::npos){

                    //Extract the Sensor Name from the Interest
                    string data_name = interestMsg->getDataName();
                    string sensor_name = data_name.substr(data_name.find(':') + 1);

                    EV_INFO << simTime() << " Received Registration Request from a Sensor: "<< sensor_name << endl;

                    // update the sensor name as a sensor in the network
                    int index = -1;
                    for (int i = 0; i < servedSensorList.size(); i++) {
                        if (string(interestMsg->getDataName()) == servedSensorList[i]->sensorName) {
                            index = i;
                            break;
                        }
                    }
                    if (index == -1) {

                        SensorEntry* sensorEnry = new SensorEntry;
                        sensorEnry->sensorName = sensor_name;
                        sensorEnry->faceID = interestMsg->getArrivalFaceID();
                        sensorEnry->faceType = interestMsg->getArrivalFaceType();
                        sensorEnry->prefixRegistered = false;
                        sensorEnry->initialFPT = interestMsg->getForwardPendingToken();
                        sensorEnry->initialPrefixName = interestMsg->getPrefixName();
                        sensorEnry->initialDataName = interestMsg->getDataName();

                        servedSensorList.push_back(sensorEnry);
                        index = servedSensorList.size() - 1;
                    }

                    //Form Data Name for the Reflexive Interest to be Sent
                    char completeDataName[64];
                    strcpy(completeDataName,"RegistrationDetails:");
                    strcat(completeDataName,sensor_name.c_str());

                    // generate a reflexive Interest to ask for Registration Details
                    InterestMsg* interestMsgNew = new InterestMsg("Reflexive Interest for Registration Interest");
                    interestMsgNew->setHopLimit(10);
                    interestMsgNew->setLifetime(simTime() + interestLifetime);
                    interestMsgNew->setPrefixName(interestMsg->getReflexiveNamePrefix());
                    interestMsgNew->setDataName(completeDataName);
                    interestMsgNew->setVersionName("v01");
                    interestMsgNew->setSegmentNum(0);
                    interestMsgNew->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                    interestMsgNew->setPayloadSize(0);
                    interestMsgNew->setHopsTravelled(0);
                    interestMsgNew->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                    interestMsgNew->setReflexiveNamePrefix("R");
                    interestMsgNew->setForwardPendingToken(0);
                    interestMsgNew->setReversePendingToken(interestMsg->getForwardPendingToken());

                    EV_INFO << simTime() << " Sending Reflexive Interest to the Sensor: " << interestMsgNew->getPrefixName()
                                    << " " << interestMsgNew->getDataName()
                                    << " " << interestMsgNew->getVersionName()
                                    << " " << interestMsgNew->getSegmentNum()
                                    << " RNP:" << interestMsgNew->getReflexiveNamePrefix()
                                    << " FPT:" << interestMsgNew->getForwardPendingToken()
                                    << " RPT:" << interestMsgNew->getReversePendingToken()
                                    << endl;

                    // send msg to forwarding layer
                    send(interestMsgNew, "forwarderInOut$o");

                }

                //To check if interest is an Phone Home Interest
                if(string(interestMsg->getDataName()).find("RegistrationID") != string::npos) {

                    EV_INFO << simTime() << " It is a Phone Home Interest" << endl;

                    //Retrieve RegistrationID and Sensor name from Dataname
                    string data_name = interestMsg->getDataName();
                    string registrationID = data_name.substr(data_name.find(':') + 1);
                    string sensor_name = data_name.substr(0, data_name.find("/"));


                    //Verify if the Sensor Name and RegistrationID match the values stored in the Record

                    // find the sensor entry
                       int index = -1;
                       for (int i = 0; i < servedSensorList.size(); i++) {
                           if(string(sensor_name).find(servedSensorList[i]->sensorName) != string::npos) {
                               index = i;
                               break;
                           }
                       }

                       if (index != -1){

                          //Sensor Authentication
                          if(registrationID == servedSensorList[index]->registrationId && sensor_name == servedSensorList[index]->sensorName ){

                              EV_INFO << simTime() << " Sensor name and RegistrationID match"
                                      << " SensorName:" << servedSensorList[index]->sensorName
                                      << " RegistartionID:" << servedSensorList[index]->registrationId
                                      << endl;

                              //Save initial Prefixname, Dataname  and FPT for Final Acknowledgement
                              servedSensorList[index]->initialPrefixName = interestMsg->getPrefixName();
                              servedSensorList[index]->initialDataName = interestMsg->getDataName();
                              servedSensorList[index]->initialFPT = interestMsg->getForwardPendingToken();

                              //Form Data Name for the Reflexive Interest to be Sent
                              char completeDataName[64];
                              strcpy(completeDataName,"SensorData:");
                              strcat(completeDataName,sensor_name.c_str());

                              //Once the Sensor is authenticated, send reflexive interest to fetch data from the Sensor
                              InterestMsg* interestMsgNew = new InterestMsg("Reflexive Interest to fetch Data from the Sensor");
                              interestMsgNew->setHopLimit(10);
                              interestMsgNew->setLifetime(simTime() + interestLifetime);
                              interestMsgNew->setPrefixName(interestMsg->getReflexiveNamePrefix());
                              interestMsgNew->setDataName(completeDataName);
                              interestMsgNew->setVersionName("v01");
                              interestMsgNew->setSegmentNum(0);
                              interestMsgNew->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                              interestMsgNew->setPayloadSize(0);
                              interestMsgNew->setHopsTravelled(0);
                              interestMsgNew->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                              interestMsgNew->setReflexiveNamePrefix("R");
                              interestMsgNew->setForwardPendingToken(0);
                              interestMsgNew->setReversePendingToken(interestMsg->getForwardPendingToken());

                              EV_INFO << simTime() << " Sending Reflexive Interest to the Sensor: " << interestMsgNew->getPrefixName()
                                              << " " << interestMsgNew->getDataName()
                                              << " " << interestMsgNew->getVersionName()
                                              << " " << interestMsgNew->getSegmentNum()
                                              << " RNP:" << interestMsgNew->getReflexiveNamePrefix()
                                              << " FPT:" << interestMsgNew->getForwardPendingToken()
                                              << " RPT:" << interestMsgNew->getReversePendingToken()
                                              << endl;

                              // send msg to forwarding layer
                              send(interestMsgNew, "forwarderInOut$o");

                          }// If Sensor Authentication fails, send Interest Return
                          else{
                              EV_INFO << simTime() << " Sensor name and RegistrationID doesn't match" << endl;

                              //Send Interest Return
                              InterestRtnMsg *interestRtnMsg = new InterestRtnMsg("Interest Return: Authentication Failed!! ");
                              interestRtnMsg->setReturnCode(ReturnCodeTypeNoRoute);
                              interestRtnMsg->setPrefixName(interestMsg->getPrefixName());
                              interestRtnMsg->setDataName(interestMsg->getDataName());
                              interestRtnMsg->setVersionName(interestMsg->getVersionName());
                              interestRtnMsg->setSegmentNum(0);
                              interestRtnMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                              interestRtnMsg->setPayloadSize(0);
                              interestRtnMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                              interestRtnMsg->setReflexiveNamePrefix("0");

                              EV_INFO << simTime() << " Authentication Failed!! Sending Interest Return to the Sensor: " << interestRtnMsg->getPrefixName()
                                              << " " << interestRtnMsg->getDataName()
                                              << " " << interestRtnMsg->getVersionName()
                                              << " " << interestRtnMsg->getSegmentNum()
                                              << " " << interestRtnMsg->getReflexiveNamePrefix()
                                              << endl;

                              // send msg to forwarding layer
                              send(interestRtnMsg, "forwarderInOut$o");
                          }
                       }

                }

            }//  request for stored sensor data at gateway from outside (e.g. Internet)
            else if (string(interestMsg->getPrefixName()).find(hostedPrefixName) != string::npos) {

                //Check if it is a Subscription Interest
                if(string(interestMsg->getDataName()).find("SubscriptionRequest") != string::npos){

                    EV_INFO << simTime() << " It is a Subscription Request" << endl;

                    //Retrieve Subscriber's username and subscribed data
                    string data_name = interestMsg->getDataName();
                    string requestingUserName = data_name.substr(data_name.find(':') + 1 ,data_name.find('/') - data_name.find(':') - 1 );
                    string requestedData = data_name.substr(data_name.find("/") + 1);

                    //Save Subscription Data
                    SubscriptionData* subscriptiondata = new SubscriptionData;
                    subscriptiondata->subscribedUser = requestingUserName;
                    subscriptiondata->subscribedData = requestedData;
                    subscriptiondata->subscriptionFPT = interestMsg->getForwardPendingToken();
                    subscriptiondata->subscriptionPrefixName = interestMsg->getPrefixName();
                    subscriptiondata->subscriptionDataName = interestMsg->getDataName();
                    subscriptiondata->subscribedDuration = 0;

                    EV_INFO  << " Size of subscriptionData : "  << subscriptionData.size() <<endl;

                    //Add the Subscription user to subscriptionData only if he has not yet subscribed to that data
                    if(subscriptionData.size() == 0){

                        subscriptionData.push_back(subscriptiondata);
                        EV_INFO << simTime() << " Subscription Data added to subscriptionData  table"  << endl;
                        EV_INFO  << " Size of SubscriptionData: "  << subscriptionData.size() <<endl;

                    }
                    else{

                        for (int i = 0; i < subscriptionData.size(); i++){

                            if(subscriptionData[i]->subscribedUser != requestingUserName || subscriptionData[i]->subscribedData != requestedData){

                                subscriptionData.push_back(subscriptiondata);
                                EV_INFO << simTime() << " Subscription Data added to subscriptionData  table"  << endl;
                                break;

                            }
                        }

                        EV_INFO  << " Size of subscriptionData : "  << subscriptionData.size() <<endl;
                    }

                    // find the sensor entry
                    int index = -1;
                    string sensedData = "";
                    for (int i = 0; i < servedSensorList.size(); i++) {
                        stringstream stream(servedSensorList[i]->lastSensorReading);
                        while(getline(stream, sensedData, ';')) {
                            if (sensedData.find(requestedData) != string::npos) {
                                index = i;
                                break;
                            }
                        }
                        if (index != -1) {
                            break;
                        }
                    }

                    // if requested sensor data is found, send Reflexive Interest with ID
                    if (index != -1) {

                        EV_INFO << simTime() << " Sensor Reading:" <<sensedData.c_str()
                                << "Sensor Reading ID:" <<servedSensorList[index]->lastSensorReadingID
                                << endl;

                        //Form Data Name for the Reflexive Interest to be Sent
                        char completeDataName[64];
                        strcpy(completeDataName,"SubscriptionDetails:");
                        strcat(completeDataName,requestedData.c_str());
                        strcat(completeDataName,"/");
                        string s = to_string(servedSensorList[index]->lastSensorReadingID);
                        char const *sensorReadingID = s.c_str();
                        strcat(completeDataName,sensorReadingID);

                        //Send Reflexive Interest asking to fetch data
                        InterestMsg* interestMsgNew = new InterestMsg("Reflexive Interest to User asking to fetch Data");
                        interestMsgNew->setHopLimit(10);
                        interestMsgNew->setLifetime(simTime() + interestLifetime);
                        interestMsgNew->setPrefixName(interestMsg->getReflexiveNamePrefix());
                        interestMsgNew->setDataName(completeDataName);
                        interestMsgNew->setVersionName("v01");
                        interestMsgNew->setSegmentNum(0);
                        interestMsgNew->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                        interestMsgNew->setPayloadSize(0);
                        interestMsgNew->setHopsTravelled(0);
                        interestMsgNew->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                        interestMsgNew->setReflexiveNamePrefix("R");
                        interestMsgNew->setForwardPendingToken(0);
                        interestMsgNew->setReversePendingToken(interestMsg->getForwardPendingToken());

                        EV_INFO << simTime() << " Sending Reflexive Interest to the User: " << interestMsgNew->getPrefixName()
                                        << " " << interestMsgNew->getDataName()
                                        << " " << interestMsgNew->getVersionName()
                                        << " " << interestMsgNew->getSegmentNum()
                                        << " RNP:" << interestMsgNew->getReflexiveNamePrefix()
                                        << " FPT:" << interestMsgNew->getForwardPendingToken()
                                        << " RPT:" << interestMsgNew->getReversePendingToken()
                                        << endl;

                        // send msg to forwarding layer
                        send(interestMsgNew, "forwarderInOut$o");


                    }

                }
                //If not a Subscription Request
                else{
                    //Check if DataName has ID, if yes, separate requested "Data" and "ID"
                    string receivedDataName = interestMsg->getDataName();

                    string requestedData;
                    int requestedDataID;

                    if(receivedDataName.find("/") != string::npos){

                        requestedData = receivedDataName.substr(0,receivedDataName.find("/"));
                        requestedDataID = stoi(receivedDataName.substr(receivedDataName.find("/")+1));

                    }
                    else{
                        requestedData = receivedDataName;
                    }

                    // find the sensor entry
                    int index = -1;
                    string sensedData = "";
                    for (int i = 0; i < servedSensorList.size(); i++) {
                        stringstream stream(servedSensorList[i]->lastSensorReading);
                        while(getline(stream, sensedData, ';')) {
                            if (sensedData.find(requestedData) != string::npos) {
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

                        //Form Data Name for the Content Object to be Sent
                        char completePayload[64];
                        strcpy(completePayload,sensedData.c_str());
                        strcat(completePayload,"/");
                        strcat(completePayload,to_string(servedSensorList[index]->lastSensorReadingID).c_str());
                        strcat(completePayload,"-");
                        strcat(completePayload,servedSensorList[index]->lastSensorReadingRecordedTime.str().c_str());

                        //If it is a Content Object which is a response to Subscription, send it with cache time as 0, else with usual cache time defined
                        if(receivedDataName.find("/") != string::npos){

                            // reply with a content obj
                            ContentObjMsg* contentObjMsg = new ContentObjMsg("Content Object");
                            contentObjMsg->setPrefixName(interestMsg->getPrefixName());
                            contentObjMsg->setDataName(interestMsg->getDataName());
                            contentObjMsg->setVersionName(interestMsg->getVersionName());
                            contentObjMsg->setSegmentNum(interestMsg->getSegmentNum());
                            contentObjMsg->setCachetime(0);
                            contentObjMsg->setExpirytime(simTime() + servedSensorList[index]->dataLifetime);
                            contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                            contentObjMsg->setPayloadSize(servedSensorList[index]->dataSize);
                            contentObjMsg->setTotalNumSegments(1);
                            contentObjMsg->setPayloadAsString(completePayload);
                            contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);
                            contentObjMsg->setReversePendingToken(interestMsg->getForwardPendingToken());

                            EV_INFO << simTime() << " Sending ContentObj: "
                                    << contentObjMsg->getPrefixName()
                                    << " " << contentObjMsg->getDataName()
                                    << " " << contentObjMsg->getVersionName()
                                    << " " << contentObjMsg->getSegmentNum()
                                    << " " << contentObjMsg->getTotalNumSegments()
                                    << " " << contentObjMsg->getExpirytime()
                                    << " " << contentObjMsg->getPayloadSize()
                                    << " Cache Time: " << contentObjMsg->getCachetime()
                                    << " RPT: " << contentObjMsg->getReversePendingToken()
                                    << " Payload: " << contentObjMsg->getPayloadAsString()
                                    << endl;

                            // send msg to forwarding layer
                            send(contentObjMsg, "forwarderInOut$o");

                            // update stats
                            demiurgeModel->incrementSubContentobjectSentCount();
                            // write stats
                            emit(subContentObjectsSentCount, demiurgeModel->getSubContentobjectSentCount());

                        }
                        else {

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
                            contentObjMsg->setPayloadAsString(completePayload);
                            contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);
                            contentObjMsg->setReversePendingToken(interestMsg->getForwardPendingToken());

                            EV_INFO << simTime() << " Sending ContentObj: "
                                    << contentObjMsg->getPrefixName()
                                    << " " << contentObjMsg->getDataName()
                                    << " " << contentObjMsg->getVersionName()
                                    << " " << contentObjMsg->getSegmentNum()
                                    << " " << contentObjMsg->getTotalNumSegments()
                                    << " " << contentObjMsg->getExpirytime()
                                    << " " << contentObjMsg->getPayloadSize()
                                    << " Cache Time: " << contentObjMsg->getCachetime()
                                    << " RPT: " << contentObjMsg->getReversePendingToken()
                                    << " Payload: " << contentObjMsg->getPayloadAsString()
                                    << endl;

                            // send msg to forwarding layer
                            send(contentObjMsg, "forwarderInOut$o");
                        }


                    } // if not found, send an Interest Return
                    else {

                        //Send Interest return
                        InterestRtnMsg *interestRtnMsg = new InterestRtnMsg("Interest Return: Content not Available!! ");
                        interestRtnMsg->setReturnCode(ReturnCodeTypeNoRoute);
                        interestRtnMsg->setPrefixName(interestMsg->getPrefixName());
                        interestRtnMsg->setDataName(interestMsg->getDataName());
                        interestRtnMsg->setVersionName(interestMsg->getVersionName());
                        interestRtnMsg->setSegmentNum(0);
                        interestRtnMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                        interestRtnMsg->setPayloadSize(0);
                        interestRtnMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                        interestRtnMsg->setReflexiveNamePrefix("0");

                        EV_INFO << simTime() << " Content Not Available!! Sending Interest Return to the User: " << interestRtnMsg->getPrefixName()
                                        << " " << interestRtnMsg->getDataName()
                                        << " " << interestRtnMsg->getVersionName()
                                        << " " << interestRtnMsg->getSegmentNum()
                                        << " " << interestRtnMsg->getReflexiveNamePrefix()
                                        << endl;

                        // send msg to forwarding layer
                        send(interestRtnMsg, "forwarderInOut$o");

                    }

                }

            }
            else {
                EV_INFO << " Ignoring Interest message - "
                        << interestMsg->getPrefixName()
                        << endl;

            }

        } // process content obj received from sensor
        if (contentObjMsg != NULL) {

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

            //If Content is the Registration details of a sensor
            if(string(contentObjMsg->getDataName()).find("RegistrationDetails") != string::npos){

                   // find the sensor entry
                   int index = -1;
                   for (int i = 0; i < servedSensorList.size(); i++) {
                       if(string(contentObjMsg->getDataName()).find(servedSensorList[i]->sensorName) != string::npos) {
                           index = i;
                           break;
                       }
                   }
                   if (index != -1){
                       servedSensorList[index]->registrationId = std::to_string(intuniform(0, 65536));
                       servedSensorList[index]->prefixRegistered = true;

                   }

                   //Send Generated unique RegistrationID to the Sensor
                   ContentObjMsg* contentObjMsg = new ContentObjMsg("Content Object containing RegistrationID of the Sensor");
                   contentObjMsg->setPrefixName(servedSensorList[index]->initialPrefixName.c_str());
                   contentObjMsg->setDataName(servedSensorList[index]->initialDataName.c_str());
                   contentObjMsg->setVersionName("v01");
                   contentObjMsg->setSegmentNum(0);
                   contentObjMsg->setCachetime(interestLifetime);
                   contentObjMsg->setExpirytime(simTime() + interestLifetime);
                   contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                   contentObjMsg->setPayloadSize(8);
                   contentObjMsg->setTotalNumSegments(1);
                   contentObjMsg->setPayloadAsString(servedSensorList[index]->registrationId.c_str());
                   contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);
                   contentObjMsg->setReversePendingToken(servedSensorList[index]->initialFPT);

                   EV_INFO << simTime() << " Sending ContentObj: "
                           << contentObjMsg->getPrefixName()
                           << " " << contentObjMsg->getDataName()
                           << " " << contentObjMsg->getVersionName()
                           << " " << contentObjMsg->getSegmentNum()
                           << " " << contentObjMsg->getExpirytime()
                           << " " << contentObjMsg->getPayloadSize()
                           << " RPT: " << contentObjMsg->getReversePendingToken()
                           << " RegistrationId: " << contentObjMsg->getPayloadAsString()
                           << endl;

                   // send msg to forwarding layer
                   send(contentObjMsg, "forwarderInOut$o");


            }//If Content is Subscription details from User
            else if (string(contentObjMsg->getDataName()).find("SubscriptionDetails") != string::npos){

                // Extract Requester details from the Content Object
                string payload = contentObjMsg->getPayloadAsString();
                string requestingUserName = payload.substr(0, payload.find('/') );
                string requestingData = payload.substr(payload.find('/') + 1 ,payload.find(':') - payload.find('/') - 1 );
                double subscriptionDuration = stod(payload.substr(payload.find(":")+1));

                EV_INFO <<" Requesting User Name: "<<requestingUserName
                        <<" Requesting Data: "<<requestingData
                        <<" Subscription Duration: "<<subscriptionDuration <<endl;

                EV_INFO  << " Size of subscriptionData : "  << subscriptionData.size() <<endl;

                for (int i = 0; i < subscriptionData.size(); i++){

                    if(subscriptionData[i]->subscribedUser == requestingUserName && subscriptionData[i]->subscribedData == requestingData){

                        //Update RNP details of the user in subscriptionData table
                         subscriptionData[i]->reflexivePrefix = contentObjMsg->getPrefixName();
                         EV_INFO <<" Updated subscription details  RNP in the table " <<endl;

                         //If Subscription Duration of the user is not updated, update it with the received value
                         if(subscriptionData[i]->subscribedDuration == 0){

                             subscriptionData[i]->subscribedDuration = subscriptionDuration;
                             subscriptionData[i]->subscribedDurationExpiry = subscriptionDuration + simTime().dbl();

                             //Enter the Expiry details in subscriptionExpiryData and sort according to ascending order of expiry time
                             //Also, if there is already SubscriptionExpiryData present for same prefixname and same dataname, don`t add it again

                             SubscriptionExpiryData* newSubscriptionexpiry = new SubscriptionExpiryData;
                             newSubscriptionexpiry->subscriptionExpiryTime = subscriptionData[i]->subscribedDurationExpiry;
                             newSubscriptionexpiry->subscriptionExpPrefixName = subscriptionData[i]->subscriptionPrefixName;
                             newSubscriptionexpiry->subscriptionExpDataName = subscriptionData[i]->subscriptionDataName;
                             newSubscriptionexpiry->subscriptionExpFPT = subscriptionData[i]->subscriptionFPT;

                             EV_INFO <<" subscriptionData[i]->subscriptionPrefixName; " << subscriptionData[i]->subscriptionPrefixName
                                     <<" subscriptionData[i]->subscriptionDataName; " << subscriptionData[i]->subscriptionDataName
                                     <<endl;

                             bool inserted = false;
                                 for (auto it = subscriptionExpiryData.begin(); it != subscriptionExpiryData.end(); ++it) {
                                     if (subscriptionData[i]->subscribedDurationExpiry < (*it)->subscriptionExpiryTime) {
                                         subscriptionExpiryData.insert(it, newSubscriptionexpiry);
                                         inserted = true;
                                         break;
                                     }
                                 }

                                 if (!inserted) {
                                     subscriptionExpiryData.push_back(newSubscriptionexpiry);
                                 }

                             EV_INFO <<" Updated subscription details in the table " <<endl;

                             // schedule the de-subscription event only if it is not scheduled earlier
                             if (!endOfUserSubscriptionEvent->isScheduled()) {

                                 //Schedule de-subscription reminder
                                 scheduleAt(subscriptionExpiryData[0]->subscriptionExpiryTime, endOfUserSubscriptionEvent);
                                 EV_INFO <<" End of User Subscription Event scheduled at: "<<subscriptionExpiryData[0]->subscriptionExpiryTime<<endl;

                             }

                         }

                      }

                }

                EV_INFO  << " Size of SubscriptionData : "  << subscriptionData.size() <<endl;

            }//If Sensor reading is received
            else if(string(contentObjMsg->getDataName()).find("SensorData") != string::npos){

                // find the sensor entry
                int index = -1;
                for (int i = 0; i < servedSensorList.size(); i++) {
                    if(string(contentObjMsg->getDataName()).find(servedSensorList[i]->sensorName) != string::npos) {
                        index = i;
                        break;
                    }
                }
                if (index != -1){
                    servedSensorList[index]->lastSensorReading = contentObjMsg->getPayloadAsString();
                    servedSensorList[index]->dataLifetime = contentObjMsg->getCachetime().dbl();
                    servedSensorList[index]->dataSize = contentObjMsg->getPayloadSize();
                    servedSensorList[index]->lastSensorReadingID = intuniform(0, 65536);
                    servedSensorList[index]->lastSensorReadingRecordedTime = simTime(); // Used to determine content freshness

                    //Save the Sensor readings with respective IDs(randomly generated)
                    servedSensorList[index]->lastSensorReadingsList.push_back(contentObjMsg->getPayloadAsString());
                    servedSensorList[index]->lastSensorReadingsListID.push_back(servedSensorList[index]->lastSensorReadingID);

                    if (servedSensorList[index]->lastSensorReadingsList.size() > maximumSensorReadingsToKeep) {
                        servedSensorList[index]->lastSensorReadingsList.erase(servedSensorList[index]->lastSensorReadingsList.begin());
                    }

                    EV_INFO  << " Size of subscriptionData : "  << subscriptionData.size() <<endl;

                    //Check if any of the users have Subscribed for any of the received Sensor Data, if yes, we will send Reflexive Interest asking to fetch Data
                    for (int i = 0; i < subscriptionData.size(); i++) {

                        //Check if User has subscribed to this Data
                        if((servedSensorList[index]->lastSensorReading.find(subscriptionData[i]->subscribedData) != std::string::npos)){


                            //If Subscription duration is not exceeded, then send Reflexive Interest
                            if(subscriptionData[i]->subscribedDurationExpiry >= simTime().dbl()){

                                //Form Data name of Reflexive Interest to be Sent
                                char completeDataName[64];
                                strcpy(completeDataName,"SubscriptionDetails:");
                                strcat(completeDataName,subscriptionData[i]->subscribedData.c_str());
                                strcat(completeDataName,"/");
                                string s = to_string(servedSensorList[index]->lastSensorReadingID);
                                char const *sensorReadingID = s.c_str();
                                strcat(completeDataName,sensorReadingID);

                                //Send Reflexive Interest to fetch Data, for that User, for that Data
                                InterestMsg* interestMsgNew = new InterestMsg("Reflexive Interest to User asking to fetch Data");
                                interestMsgNew->setHopLimit(10);
                                interestMsgNew->setLifetime(simTime() + interestLifetime);
                                interestMsgNew->setPrefixName(subscriptionData[i]->reflexivePrefix.c_str());
                                interestMsgNew->setDataName(completeDataName);
                                interestMsgNew->setVersionName("v01");
                                interestMsgNew->setSegmentNum(0);
                                interestMsgNew->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                                interestMsgNew->setPayloadSize(0);
                                interestMsgNew->setHopsTravelled(0);
                                interestMsgNew->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);
                                interestMsgNew->setReflexiveNamePrefix("R");
                                interestMsgNew->setForwardPendingToken(0);
                                interestMsgNew->setReversePendingToken(subscriptionData[i]->subscriptionFPT);

                                EV_INFO << simTime() << " Sending Reflexive Interest to the User: " << interestMsgNew->getPrefixName()
                                                << " " << interestMsgNew->getDataName()
                                                << " " << interestMsgNew->getVersionName()
                                                << " " << interestMsgNew->getSegmentNum()
                                                << " RNP:" << interestMsgNew->getReflexiveNamePrefix()
                                                << " FPT:" << interestMsgNew->getForwardPendingToken()
                                                << " RPT:" << interestMsgNew->getReversePendingToken()
                                                << endl;

                                // send msg to forwarding layer
                                send(interestMsgNew, "forwarderInOut$o");

                            }


                        }


                    }

                }
                else {
                    EV_FATAL << "Corresponding entry in sensor data not found for " << contentObjMsg->getPrefixName() << endl;
                    throw cRuntimeError("Check log for details");
                }

                EV_INFO  << "Last Sensor Reading: " << servedSensorList[index]->lastSensorReading<< endl;
                EV_INFO  << "Last Sensor Reading ID: " << servedSensorList[index]->lastSensorReadingID<< endl;
                EV_INFO  << "Last Sensor Reading Recorded Time: " << servedSensorList[index]->lastSensorReadingRecordedTime<< endl;


                //Send Acknowledgement for received Sensor Readings
                ContentObjMsg* contentObjMsg = new ContentObjMsg("Acknowledgement for received Sensor Readings");
                contentObjMsg->setPrefixName(servedSensorList[index]->initialPrefixName.c_str());
                contentObjMsg->setDataName(servedSensorList[index]->initialDataName.c_str());
                contentObjMsg->setVersionName("v01");
                contentObjMsg->setSegmentNum(0);
                contentObjMsg->setCachetime(interestLifetime);
                contentObjMsg->setExpirytime(simTime() + interestLifetime);
                contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
                contentObjMsg->setPayloadSize(8);
                contentObjMsg->setTotalNumSegments(1);
                contentObjMsg->setPayloadAsString("Acknowledge: OK");
                contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + 8);
                contentObjMsg->setReversePendingToken(servedSensorList[index]->initialFPT);

                EV_INFO << simTime() << " Sending ContentObj: "
                        << contentObjMsg->getPrefixName()
                        << " " << contentObjMsg->getDataName()
                        << " " << contentObjMsg->getVersionName()
                        << " " << contentObjMsg->getSegmentNum()
                        << " " << contentObjMsg->getExpirytime()
                        << " " << contentObjMsg->getPayloadSize()
                        << " RPT: " << contentObjMsg->getReversePendingToken()
                        << " RegistrationId: " << contentObjMsg->getPayloadAsString()
                        << endl;

                // send msg to forwarding layer
                send(contentObjMsg, "forwarderInOut$o");


            }

        }
       if (interestRtnMsg != NULL){

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

void IoTGatewayApp::finish()
{
    // remove sensor info from list
    for (int i = 0; i < servedSensorList.size(); i++) {
        servedSensorList[i]->lastSensorReadingsList.clear();
        delete servedSensorList[i];
    }
    servedSensorList.clear();

}
