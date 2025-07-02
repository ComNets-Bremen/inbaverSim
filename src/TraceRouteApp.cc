//
// Created by Finke, Tarek & Wieker, Lars as part of the Network Simulation Project
//
#include "TraceRouteApp.h"

Define_Module(TraceRouteApp);

void TraceRouteApp::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
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
    } else if (stage == 2){

        // reminder to generate content host app registration event
        cMessage *appRegReminderEvent = new cMessage("App Registration Reminder Event");
        appRegReminderEvent->setKind(TRACEROUTEAPP_APP_REG_REM_EVENT_CODE);
        scheduleAt(simTime(), appRegReminderEvent);

        // start traceroute
        traceRouteStartEvent = new cMessage("Content Download Start Event");
        traceRouteStartEvent->setKind(TRACEROUTEAPP_START_TRACEROUTE_EVENT_CODE);
        scheduleAt(simTime(), traceRouteStartEvent);

        // interest retransmission event
        interestRetransmitEvent = new cMessage("Interest Retransmission Event");
        interestRetransmitEvent->setKind(TRACEROUTEAPP_INTEREST_RETRANSMIT_EVENT_CODE);

        // register stat signals
        totalInterestsBytesSentSignal = registerSignal("appTotalInterestsBytesSent");
        retransmissionInterestsBytesSentSignal = registerSignal("appRetransmissionInterestsBytesSent");
        totalContentObjsBytesReceivedSignal = registerSignal("appTotalContentObjsBytesReceived");
        totalDataBytesReceivedSignal = registerSignal("appTotalDataBytesReceived");
        networkInterestRetransmissionCountSignal = registerSignal("appNetworkInterestRetransmissionCount");
        networkInterestInjectedCountSignal = registerSignal("appNetworkInterestInjectedCount");
    }
    else {
        EV_FATAL << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");
    }
}

void TraceRouteApp::handleMessage(cMessage *msg)
{
    // register app with lower layer (forwarder)
    if (msg->isSelfMessage() && msg->getKind() == TRACEROUTEAPP_APP_REG_REM_EVENT_CODE) {

        EV_INFO << simTime() << " Registering application with ID: " << getId() << endl;

        AppRegistrationMsg *appRegMsg = new AppRegistrationMsg();
        appRegMsg->setAppID(getId());
        appRegMsg->setAppDescription("Traceroute Client Application");

        send(appRegMsg, "forwarderInOut$o");

        delete msg;
    }
    else if (msg->isSelfMessage() && msg->getKind() == TRACEROUTEAPP_START_TRACEROUTE_EVENT_CODE){
        /*
         * TODO
         */
    }
    else if(msg->isSelfMessage() && msg->getKind() == TRACEROUTEAPP_INTEREST_RETRANSMIT_EVENT_CODE){
        /*
         * TODO
         */
    }

}

void TraceRouteApp::finish(){
    // remove remaining events
    if (traceRouteStartEvent -> isScheduled()){
        cancelEvent(traceRouteStartEvent);
    }
    delete traceRouteStartEvent;

    if (interestRetransmitEvent -> isScheduled()){
        cancelEvent(interestRetransmitEvent);
    }
    delete interestRetransmitEvent;
}
