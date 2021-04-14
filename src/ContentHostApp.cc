//
// A file hosting (content server) application that
// implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
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
            EV_FATAL <<  CONTENTHOSTAPP_SIMMODULEINFO << "The single Deus model instance not found. Please define one at the network level.\n";
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



    } else {
        EV_FATAL <<  CONTENTHOSTAPP_SIMMODULEINFO << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");

    }
}

void ContentHostApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == CONTENTHOSTAPP_APP_REG_REM_EVENT_CODE) {

        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setContentServerApp(true);
        appRegMsg->setHostedPrefixNamesArraySize(hostedPrefixList.size());
        for (int i = 0; i < hostedPrefixList.size(); i++) {
            appRegMsg->setHostedPrefixNames(i, hostedPrefixList[i].c_str());
        }
        appRegMsg->setAppDescription("Content Host Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;

    // process interest message
    } else {

        cGate *arrivalGate;
        char gateName[32];

       // get message arrival gate name
        arrivalGate = msg->getArrivalGate();
        strcpy(gateName, arrivalGate->getName());

        InterestMsg* interestMsg = dynamic_cast<InterestMsg*>(msg);

        if (strstr(gateName, "forwarderInOut$i") != NULL && interestMsg != NULL) {

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

//                cout << CONTENTHOSTAPP_SIMMODULEINFO << " no such file name and total segments, so created, segments "
//                        << hostedContentEntry->totalNumSegments << "\n";

            }

            // requested seg is not valid, send interest return
            if (interestMsg->getSegmentNum() >= hostedContentEntry->totalNumSegments) {

//                cout << CONTENTHOSTAPP_SIMMODULEINFO << " segment num not within range, so creating reply interest rtn " << "\n";

                InterestRtnMsg* interestRtnMsg = new InterestRtnMsg("Interest Return");
                interestRtnMsg->setReturnCode(ReturnCodeTypeNoRoute);
                interestRtnMsg->setPrefixName(interestMsg->getPrefixName());
                interestRtnMsg->setDataName(interestMsg->getDataName());
                interestRtnMsg->setVersionName(interestMsg->getVersionName());
                interestRtnMsg->setSegmentNum(interestMsg->getSegmentNum());
                interestRtnMsg->setHeaderSize(INBAVER_INTEREST_RTN_MSG_HEADER_SIZE);
                interestRtnMsg->setPayloadSize(0);
                interestRtnMsg->setByteLength(INBAVER_INTEREST_RTN_MSG_HEADER_SIZE);

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

                // send msg to forwarding layer
                send(contentObjMsg, "forwarderInOut$o");

            }
        } else {
            EV_INFO <<  CONTENTHOSTAPP_SIMMODULEINFO << " Ignoring message - " << msg->getName() << "\n";

        }

        delete msg;
    }
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
