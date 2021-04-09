//
// A file download client (content requester) application that
// implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#include "ContentDownloadApp.h"

Define_Module(ContentDownloadApp);

void ContentDownloadApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        contentRequestInterval = par("contentRequestInterval");
        requestedPrefixNames = par("requestedPrefixNames").stringValue();
        dataNamePrefix = par("dataNamePrefix").stringValue();
        maxHopsAllowed = par("maxHopsAllowed");
        interestRetransmitTimeout = par("interestRetransmitTimeout");


    } else if (stage == 1) {

        // get Deus model instance
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

        // when Deus or Numen not found, terminate
        if (deusModel == NULL || numenModel == NULL) {
            EV_FATAL <<  CONTENTDOWNLOADAPP_SIMMODULEINFO << "The global Deus instance and/or node specific Numen instance not found.\n";
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
        scheduleAt(simTime() + contentRequestInterval, contentDownloadStartEvent);

        // interest retransmission event
        interestRetransmitEvent = new cMessage("Interest Retransmission Event");
        interestRetransmitEvent->setKind(CONTENTDOWNLOADAPP_INTEREST_RETRANSMIT_EVENT_CODE);

        // register stat signals
        contentDownloadDurationSignal = registerSignal("appContentDownloadDuration");

    } else {
        EV_FATAL <<  CONTENTDOWNLOADAPP_SIMMODULEINFO << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");
    }
}

void ContentDownloadApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == CONTENTDOWNLOADAPP_APP_REG_REM_EVENT_CODE) {

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
        rndNum = intuniform(0, 9999);
        sprintf(tempString, "%s%04d", dataNamePrefix.c_str(), rndNum);
        requestingDataName = string(tempString);
        requestedSegNum = 0;
        totalSegments = -1;
        startTime = simTime();

        EV_INFO << CONTENTDOWNLOADAPP_SIMMODULEINFO << " content details start - "
                << requestingPrefixName << ", " << requestingDataName
                << ", prefix list size - " << requestedPrefixList.size() << "\n";

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


        EV_INFO << CONTENTDOWNLOADAPP_SIMMODULEINFO << " first interest "
                << requestingPrefixName << "/" << requestingDataName
                << ", requested seg - " << requestedSegNum
                << ", total segs - " << totalSegments << "\n";

        // send msg to forwarding layer
        send(interestMsg, "forwarderInOut$o");

        // set interest re-send timer
        if (interestRetransmitEvent->isScheduled()) {
            cancelEvent(interestRetransmitEvent);
        }
        scheduleAt(simTime() + interestRetransmitTimeout, interestRetransmitEvent);

    // interest retransmit timeout
    } else if (msg->isSelfMessage() && msg->getKind() == CONTENTDOWNLOADAPP_INTEREST_RETRANSMIT_EVENT_CODE) {

        EV_INFO << CONTENTDOWNLOADAPP_SIMMODULEINFO << " retransmission triggered sending interest "
                << requestingPrefixName << "/" << requestingDataName
                << ", requested seg - " << requestedSegNum
                << ", total segs - " << totalSegments << "\n";


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

        // send msg to forwarding layer
        send(interestMsg, "forwarderInOut$o");

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

            EV_INFO << CONTENTDOWNLOADAPP_SIMMODULEINFO << " received segment "
                    << contentObjMsg->getPrefixName() << "/" << contentObjMsg->getDataName()
                    << ", requested seg - " << contentObjMsg->getSegmentNum()
                    << ", total segs - " << contentObjMsg->getTotalNumSegments() << "\n";


            // requested segment received check
            if (contentObjMsg->getSegmentNum() == requestedSegNum) {

                // was this the last segment?
                if ((requestedSegNum + 1) >= totalSegments) {

                    EV_INFO << CONTENTDOWNLOADAPP_SIMMODULEINFO << " last segment received - delay "
                            << (simTime() - startTime) << "\n";

                    // content downloaded, so generate duration stat
                    emit(contentDownloadDurationSignal, (simTime() - startTime));

                    // cancel timeout event and setup for next content download
                    if (interestRetransmitEvent->isScheduled()) {
                        cancelEvent(interestRetransmitEvent);
                    }
                    scheduleAt(simTime() + contentRequestInterval, contentDownloadStartEvent);


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

                    EV_INFO << CONTENTDOWNLOADAPP_SIMMODULEINFO << " next interest "
                            << requestingPrefixName << "/" << requestingDataName
                            << ", requested seg - " << requestedSegNum
                            << ", total segs - " << totalSegments << "\n";


                    // send msg to forwarding layer
                    send(interestMsg, "forwarderInOut$o");

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
