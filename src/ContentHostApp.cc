//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "ContentHostApp.h"

Define_Module(ContentHostApp);

void ContentHostApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        hostedPrefixNames = par("hostedPrefixNames").stringValue();
        dataNamePrefix = par("dataNamePrefix").stringValue();
        segmentSize = par("segmentSize");
        numSegmentsPerFile = par("numSegmentsPerFile");
        cacheTime = par("cacheTime");

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

        // make list of hosted prefixes
        string ccnPrefix;
        stringstream stream(hostedPrefixNames);
        while(getline(stream, ccnPrefix, ';')) {
            hostedPrefixList.push_back(ccnPrefix);
        }

    } else if (stage == 2) {

        // reminder to generate content host app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(CONTENTHOSTAPP_APP_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // register stat signals
        totalInterestsBytesReceivedSignal = registerSignal("appTotalInterestsBytesReceived");
        totalContentObjsBytesSentSignal = registerSignal("appTotalContentObjsBytesSent");
        totalTracerouteRqstsBytesReceivedSignal = registerSignal("appTotalTracerouteRqstsBytesReceived");
        totalTracerouteRplsBytesSentSignal = registerSignal("appTotalTraceorouteRplsBytesSent");
        totalDataBytesSentSignal = registerSignal("appTotalDataBytesSent");

    } else {
        EV_FATAL << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");

    }
}

void ContentHostApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == CONTENTHOSTAPP_APP_REG_REM_EVENT_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setContentServerApp(true);
        appRegMsg->setHostedPrefixNamesArraySize(hostedPrefixList.size());
        for (int i = 0; i < hostedPrefixList.size(); i++) {
            appRegMsg->setHostedPrefixNames(i, hostedPrefixList[i].c_str());

            EV_INFO << simTime() << " Registering prefix: " << hostedPrefixList[i] << endl;

        }
        appRegMsg->setAppDescription("Content Host Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    // process interest message
    } else {
        cGate *arrivalGate;
        char gateName[32];
        InterestMsg *interestMsg = NULL;
        TracerouteRqstMsg *tracerouteRqstMsg = NULL;

       // get message arrival gate name
        arrivalGate = msg->getArrivalGate();
        strcpy(gateName, arrivalGate->getName());

        if((interestMsg = dynamic_cast<InterestMsg*>(msg)) != NULL){
            processInterest(interestMsg);
        }
        else if((tracerouteRqstMsg = dynamic_cast<TracerouteRqstMsg*>(msg)) != NULL){
            processTracerouteRqst(tracerouteRqstMsg);
        }
//        else {
//            EV_INFO << " Ignoring message - " << msg->getName() << "\n";
//
//            //send interest return
//            InterestRtnMsg* interestRtnMsg = new InterestRtnMsg("Interest Return");
//            interestRtnMsg->setReturnCode(ReturnCodeTypeNoRoute);
//            interestRtnMsg->setPrefixName(msg->getPrefixName());
//            interestRtnMsg->setDataName(msg->getDataName());
//            interestRtnMsg->setVersionName(msg->getVersionName());
//            interestRtnMsg->setSegmentNum(msg->getSegmentNum());
//            interestRtnMsg->setHeaderSize(INBAVER_INTEREST_RTN_MSG_HEADER_SIZE);
//            interestRtnMsg->setPayloadSize(0);
//            interestRtnMsg->setByteLength(INBAVER_INTEREST_RTN_MSG_HEADER_SIZE);
//            interestRtnMsg->setReflexiveNamePrefix("0");
//
//            EV_INFO << simTime() << " Sending InterestRtn: "
//                    << interestRtnMsg->getPrefixName()
//                    << " " << interestRtnMsg->getDataName()
//                    << " " << interestRtnMsg->getVersionName()
//                    << " " << interestRtnMsg->getSegmentNum()
//                    << " " << interestRtnMsg->getReturnCode() << endl;
//
//            // send msg to forwarding layer
//            send(interestRtnMsg, "forwarderInOut$o");
//        }
    }
}

void ContentHostApp::processInterest(InterestMsg *interestMsg){

        EV_INFO << simTime() << " Received Interest: "
                << interestMsg->getPrefixName()
                << " " << interestMsg->getDataName()
                << " " << interestMsg->getVersionName()
                << " " << interestMsg->getSegmentNum() << endl;

        // generate stat for received interest size
        emit(totalInterestsBytesReceivedSignal, (long) interestMsg->getByteLength());

        // look for details of file in list
        bool found = false;
        HostedContentEntry *hostedContentEntry = NULL;
        for (int i = 0; i < hostedContentList.size(); i++) {
            hostedContentEntry = hostedContentList[i];
            if (strcmp(interestMsg->getPrefixName(), hostedContentEntry->prefixName.c_str()) == 0
                    && strcmp(interestMsg->getDataName(), hostedContentEntry->dataName.c_str()) == 0
                    && strcmp(interestMsg->getVersionName(), hostedContentEntry->versionName.c_str()) == 0) {
                found = true;
                break;
            }
        }

        // if not found, insert new file details
        if (!found) {
            hostedContentEntry = new HostedContentEntry;
            hostedContentEntry->prefixName = interestMsg->getPrefixName();
            hostedContentEntry->dataName = interestMsg->getDataName();
            hostedContentEntry->versionName = interestMsg->getVersionName();
            hostedContentEntry->totalNumSegments = par("numSegmentsPerFile");
            hostedContentList.push_back(hostedContentEntry);

            EV_INFO << simTime() << " Content created and added: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum()
                    << " " << hostedContentEntry->totalNumSegments << endl;

        }

        // requested seg is not valid, send interest return
        if (interestMsg->getSegmentNum() >= hostedContentEntry->totalNumSegments) {

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

        // if right data segment requested, send data
        } else {

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
            contentObjMsg->setTotalNumSegments(hostedContentEntry->totalNumSegments);
            contentObjMsg->setPayloadAsString("");
            contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + segmentSize);

            EV_INFO << simTime() << " Sending ContentObj: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum()
                    << " " << hostedContentEntry->totalNumSegments
                    << " " << simTime() + cacheTime
                    << " " << segmentSize << endl;

            // send msg to forwarding layer
            send(contentObjMsg, "forwarderInOut$o");

            // generate stat for sent content obj
            emit(totalContentObjsBytesSentSignal, (long) contentObjMsg->getByteLength());
            emit(totalDataBytesSentSignal, (long) contentObjMsg->getPayloadSize());

        }
       delete interestMsg;
}

void ContentHostApp::processTracerouteRqst(TracerouteRqstMsg *tracerouteRqstMsg){

            EV_INFO << simTime() << " Received Traceroute Request: "
                    << tracerouteRqstMsg->getPrefixName()
                    << " " << tracerouteRqstMsg->getDataName()
                    << " " << tracerouteRqstMsg->getVersionName()
                    << " " << tracerouteRqstMsg->getSegmentNum() << endl;

            // generate stat for received interest size
            emit(totalTracerouteRqstsBytesReceivedSignal, (long) tracerouteRqstMsg->getByteLength());

            // look for details of file in list
            bool found = false;
            HostedContentEntry *hostedContentEntry = NULL;
            for (int i = 0; i < hostedContentList.size(); i++) {
                hostedContentEntry = hostedContentList[i];
                if (strcmp(tracerouteRqstMsg->getPrefixName(), hostedContentEntry->prefixName.c_str()) == 0
                        && strcmp(tracerouteRqstMsg->getDataName(), hostedContentEntry->dataName.c_str()) == 0
                        && strcmp(tracerouteRqstMsg->getVersionName(), hostedContentEntry->versionName.c_str()) == 0) {
                    found = true;
                    break;
                }
            }

            // unclear if applicable for traceroute requests
//
//            // if not found, insert new file details
//            if (!found) {
//                hostedContentEntry = new HostedContentEntry;
//                hostedContentEntry->prefixName = tracerouteRqstMsg->getPrefixName();
//                hostedContentEntry->dataName = tracerouteRqstMsg->getDataName();
//                hostedContentEntry->versionName = tracerouteRqstMsg->getVersionName();
//                hostedContentEntry->totalNumSegments = par("numSegmentsPerFile");
//                hostedContentList.push_back(hostedContentEntry);
//
//                EV_INFO << simTime() << " Content created and added: "
//                        << tracerouteRqstMsg->getPrefixName()
//                        << " " << tracerouteRqstMsg->getDataName()
//                        << " " << tracerouteRqstMsg->getVersionName()
//                        << " " << tracerouteRqstMsg->getSegmentNum()
//                        << " " << hostedContentEntry->totalNumSegments << endl;
//
//            }

            // requested seg is not valid, send interest return
            if (found){
                if (tracerouteRqstMsg->getSegmentNum() >= hostedContentEntry->totalNumSegments) {

                    InterestRtnMsg* interestRtnMsg = new InterestRtnMsg("Interest Return");
                    interestRtnMsg->setReturnCode(ReturnCodeTypeNoRoute);
                    interestRtnMsg->setPrefixName(tracerouteRqstMsg->getPrefixName());
                    interestRtnMsg->setDataName(tracerouteRqstMsg->getDataName());
                    interestRtnMsg->setVersionName(tracerouteRqstMsg->getVersionName());
                    interestRtnMsg->setSegmentNum(tracerouteRqstMsg->getSegmentNum());
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

                // if right data segment requested, send data
                } else {

                    segmentSize = par("segmentSize");
                    cacheTime = par("cacheTime");
                    TracerouteRplMsg* tracerouteRplMsg = new TracerouteRplMsg("Traceroute Reply");
                    tracerouteRplMsg->setPrefixName(tracerouteRqstMsg->getPrefixName());
                    tracerouteRplMsg->setDataName(tracerouteRqstMsg->getDataName());
                    tracerouteRplMsg->setVersionName(tracerouteRqstMsg->getVersionName());
                    tracerouteRplMsg->setSegmentNum(tracerouteRqstMsg->getSegmentNum());
                    tracerouteRplMsg->setCachetime(cacheTime);
                    tracerouteRplMsg->setExpirytime(simTime() + cacheTime);
                    tracerouteRplMsg->setHeaderSize(INBAVER_TRACEROUTE_RPL_MSG_HEADER_SIZE);
                    tracerouteRplMsg->setPayloadSize(segmentSize);
                    tracerouteRplMsg->setTotalNumSegments(hostedContentEntry->totalNumSegments);
                    tracerouteRplMsg->setPayloadAsString("");
                    tracerouteRplMsg->setByteLength(INBAVER_TRACEROUTE_RPL_MSG_HEADER_SIZE + segmentSize);
                    tracerouteRplMsg->setPathlabel("");

                    EV_INFO << simTime() << " Sending TracerouteRpl: "
                            << tracerouteRplMsg->getPrefixName()
                            << " " << tracerouteRplMsg->getDataName()
                            << " " << tracerouteRplMsg->getVersionName()
                            << " " << tracerouteRplMsg->getSegmentNum()
                            << " " << hostedContentEntry->totalNumSegments
                            << " " << simTime() + cacheTime
                            << " " << segmentSize << endl;

                    // send msg to forwarding layer
                    send(tracerouteRplMsg, "forwarderInOut$o");

                    // generate stat for sent content obj
                    emit(totalTracerouteRplsBytesSentSignal, (long) tracerouteRplMsg->getByteLength());
                    emit(totalDataBytesSentSignal, (long) tracerouteRplMsg->getPayloadSize());

                }
            } else {

                 // content not found, ignoring message
                EV_INFO << simTime() << " Content not found:"
                        << " " << tracerouteRqstMsg->getPrefixName()
                        << " " << tracerouteRqstMsg->getDataName()
                        << " " << tracerouteRqstMsg->getVersionName()
                        << " " << tracerouteRqstMsg->getSegmentNum()
                        << endl;

                // drop request
                delete tracerouteRqstMsg;
                return;
            }
        delete tracerouteRqstMsg;
}

void ContentHostApp::finish()
{
    // remove content file list
    for (int i = 0; i < hostedContentList.size(); i++) {
        delete hostedContentList[i];
    }
    hostedContentList.clear();

    // remove hosted prefixes list
    hostedPrefixList.clear();
}
