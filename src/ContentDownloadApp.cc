//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "ContentDownloadApp.h"

Define_Module(ContentDownloadApp);

void ContentDownloadApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        interContentDownloadInterval = par("interContentDownloadInterval");
        requestedPrefixNames = par("requestedPrefixNames").stringValue();
        dataNamePrefix = par("dataNamePrefix").stringValue();
        maxHopsAllowed = par("maxHopsAllowed");
        interestRetransmitTimeout = par("interestRetransmitTimeout");


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
        string ccnPrefix;
        stringstream stream(requestedPrefixNames);
        while(getline(stream, ccnPrefix, ';')) {
            requestedPrefixList.push_back(ccnPrefix);
        }

    } else if (stage == 2) {

        // reminder to generate content host app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(CONTENTDOWNLOADAPP_APP_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // start downloading content event
        contentDownloadStartEvent = new cMessage("Content Download Start Event");
        contentDownloadStartEvent->setKind(CONTENTDOWNLOADAPP_START_CONTENT_DOWNLOAD_EVENT_CODE);
        interContentDownloadInterval = par("interContentDownloadInterval");
        scheduleAt(simTime() + interContentDownloadInterval, contentDownloadStartEvent);

        // interest retransmission event
        interestRetransmitEvent = new cMessage("Interest Retransmission Event");
        interestRetransmitEvent->setKind(CONTENTDOWNLOADAPP_INTEREST_RETRANSMIT_EVENT_CODE);

        // register stat signals
        contentDownloadDurationSignal = registerSignal("appContentDownloadDuration");
        segmentDownloadDurationSignal = registerSignal("appSegmentDownloadDuration");
        totalInterestsBytesSentSignal = registerSignal("appTotalInterestsBytesSent");
        retransmissionInterestsBytesSentSignal = registerSignal("appRetransmissionInterestsBytesSent");
        totalContentObjsBytesReceivedSignal = registerSignal("appTotalContentObjsBytesReceived");
        totalDataBytesReceivedSignal = registerSignal("appTotalDataBytesReceived");
        networkInterestRetransmissionCountSignal = registerSignal("appNetworkInterestRetransmissionCount");
        networkInterestInjectedCountSignal = registerSignal("appNetworkInterestInjectedCount");

    } else {
        EV_FATAL << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");
    }
}

void ContentDownloadApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == CONTENTDOWNLOADAPP_APP_REG_REM_EVENT_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setAppDescription("Content Download Client Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    // start new content download
    } else if (msg->isSelfMessage() && msg->getKind() == CONTENTDOWNLOADAPP_START_CONTENT_DOWNLOAD_EVENT_CODE) {
        int rndNum;
        char tempString[128];

        // identify data to download
        rndNum = intuniform(0, (requestedPrefixList.size() - 1));
        requestingPrefixName = requestedPrefixList[rndNum];
        rndNum = par("nextFileSuffix");
        snprintf(tempString, sizeof(tempString), "%s%04d", dataNamePrefix.c_str(), rndNum);
        requestingDataName = string(tempString);
        requestedSegNum = 0;
        totalSegments = -1;
        contentDownloadStartTime = simTime();

        EV_INFO << simTime() << " Content download start: " << requestingPrefixName
                << " " << requestingDataName << " v01" << endl;

        // generate 1st interest
        InterestMsg* interestMsg = new InterestMsg("Interest");
        interestMsg->setHopLimit(maxHopsAllowed);
        interestMsg->setLifetime(simTime() + interestRetransmitTimeout);
        interestMsg->setPrefixName(requestingPrefixName.c_str());
        interestMsg->setDataName(requestingDataName.c_str());
        interestMsg->setVersionName("v01");
        interestMsg->setSegmentNum(requestedSegNum);
        interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
        interestMsg->setPayloadSize(0);
        interestMsg->setHopsTravelled(0);
        interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

        EV_INFO << simTime() << " Sending Interest: " << requestingPrefixName
                << " " << requestingDataName << " v01 " << requestedSegNum
                << " " << totalSegments << endl;

        // send msg to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // remember last interest sent time for statistic
        lastInterestSentTime = simTime();

        // update stats
        demiurgeModel->incrementNetworkInterestInjectedCount();

        // write stats
        emit(totalInterestsBytesSentSignal, (long) interestMsg->getByteLength());
        emit(networkInterestInjectedCountSignal, demiurgeModel->getNetworkInterestInjectedCount());

        // set interest re-send timer
        if (interestRetransmitEvent->isScheduled()) {
            cancelEvent(interestRetransmitEvent);
        }
        scheduleAt(simTime() + interestRetransmitTimeout, interestRetransmitEvent);

    // interest retransmit timeout
    } else if (msg->isSelfMessage() && msg->getKind() == CONTENTDOWNLOADAPP_INTEREST_RETRANSMIT_EVENT_CODE) {

        // generate retransmit interest
        InterestMsg* interestMsg = new InterestMsg("Interest");
        interestMsg->setHopLimit(maxHopsAllowed);
        interestMsg->setLifetime(simTime() + interestRetransmitTimeout);
        interestMsg->setPrefixName(requestingPrefixName.c_str());
        interestMsg->setDataName(requestingDataName.c_str());
        interestMsg->setVersionName("v01");
        interestMsg->setSegmentNum(requestedSegNum);
        interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
        interestMsg->setPayloadSize(0);
        interestMsg->setHopsTravelled(0);
        interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

        EV_INFO << simTime() << " Re-transmitting Interest: " << requestingPrefixName
                << " " << requestingDataName << " v01 " << requestedSegNum
                << " " << totalSegments << endl;

        // send msg to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // update stats
        demiurgeModel->incrementNetworkInterestRetransmissionCount();
        demiurgeModel->incrementNetworkInterestInjectedCount();

        // write stats
        emit(totalInterestsBytesSentSignal, (long) interestMsg->getByteLength());
        emit(retransmissionInterestsBytesSentSignal, (long) interestMsg->getByteLength());
        emit(networkInterestRetransmissionCountSignal, demiurgeModel->getNetworkInterestRetransmissionCount());
        emit(networkInterestInjectedCountSignal, demiurgeModel->getNetworkInterestInjectedCount());

        // set interest re-send timer
        scheduleAt(simTime() + interestRetransmitTimeout, interestRetransmitEvent);

    // process request content object
    } else {
        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        ContentObjMsg* contentObjMsg = dynamic_cast<ContentObjMsg*>(msg);

        if (strstr(gateName, "forwarderInOut$i") != NULL && contentObjMsg != NULL) {

            totalSegments = contentObjMsg->getTotalNumSegments();

            // write stats
            emit(segmentDownloadDurationSignal, (simTime() - lastInterestSentTime));
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
                    << " " << totalSegments << endl;

            // requested segment received check
            if (contentObjMsg->getSegmentNum() == requestedSegNum) {

                // was this the last segment?
                if ((requestedSegNum + 1) >= totalSegments) {

                    EV_INFO << simTime() << " Content download complete: "
                            << contentObjMsg->getPrefixName()
                            << " " << contentObjMsg->getDataName()
                            << " " << contentObjMsg->getVersionName() << endl;

                    // content downloaded, so generate duration stat
                    emit(contentDownloadDurationSignal, (simTime() - contentDownloadStartTime));

                    // cancel timeout event and setup for next content download
                    if (interestRetransmitEvent->isScheduled()) {
                        cancelEvent(interestRetransmitEvent);
                    }
                    interContentDownloadInterval = par("interContentDownloadInterval");
                    scheduleAt(simTime() + interContentDownloadInterval, contentDownloadStartEvent);

                // generate interest for next segment
                } else {

                    requestedSegNum++;

                    // generate next interest
                    InterestMsg* interestMsg = new InterestMsg("Interest");
                    interestMsg->setHopLimit(maxHopsAllowed);
                    interestMsg->setLifetime(simTime() + interestRetransmitTimeout);
                    interestMsg->setPrefixName(requestingPrefixName.c_str());
                    interestMsg->setDataName(requestingDataName.c_str());
                    interestMsg->setVersionName("v01");
                    interestMsg->setSegmentNum(requestedSegNum);
                    interestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
                    interestMsg->setPayloadSize(0);
                    interestMsg->setHopsTravelled(0);
                    interestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE);

                    EV_INFO << simTime() << " Sending Interest: " << requestingPrefixName
                            << " " << requestingDataName << " v01 " << requestedSegNum
                            << " " << totalSegments << endl;

                    // send msg to forwarding layer
                    send(interestMsg, "forwarderInOut$o");

                    // remember last interest sent time for statistic
                    lastInterestSentTime = simTime();

                    // update stats
                    demiurgeModel->incrementNetworkInterestInjectedCount();

                    // write stats
                    emit(totalInterestsBytesSentSignal, (long) interestMsg->getByteLength());
                    emit(networkInterestInjectedCountSignal, demiurgeModel->getNetworkInterestInjectedCount());

                    // set interest re-send timer
                    if (interestRetransmitEvent->isScheduled()) {
                        cancelEvent(interestRetransmitEvent);
                    }
                    scheduleAt(simTime() + interestRetransmitTimeout, interestRetransmitEvent);
                }
            }
        }
        delete msg;
    }
}

void ContentDownloadApp::finish()
{
    // remove download start event
    if (contentDownloadStartEvent->isScheduled()) {
        cancelEvent(contentDownloadStartEvent);
    }
    delete contentDownloadStartEvent;

     // remove interest retransmit event
    if (interestRetransmitEvent->isScheduled()) {
        cancelEvent(interestRetransmitEvent);
    }
    delete interestRetransmitEvent;
}
