//
// Created by Finke, Tarek & Wieker, Lars as part of the Network Simulation Project
//

#ifndef __INBAVERSIM_TRACEROUTEAPP_H_
#define __INBAVERSIM_TRACEROUTEAPP_H_

#include "Demiurge.h"
#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"
#include "Numen.h"

using namespace omnetpp;

/**
 * A TraceRoute calling application that implements the IApplication interface
 */
class TraceRouteApp : public cSimpleModule
{
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();


  private:
    string requestedPrefixNames;
    string dataNamePrefix;
    int maxHopsAllowed;
    double interestRetransmitTimeout;
    string PathTLV;

    Demiurge *demiurgeModel;
    Numen *numenModel;

    vector <string> requestedPrefixList;

    // start traceroute event
    cMessage *traceRouteStartEvent;

    // interest retransmission event
    cMessage *interestRetransmitEvent;

    //trace timeout event
    cMessage *traceTimeoutEvent;

    // details of current download
    string requestingPrefixName;
    string requestingDataName;
    int requestedSegNum;
    int totalSegments;
    simtime_t lastInterestSentTime;


    // stat signals
    simsignal_t totalInterestsBytesSentSignal;
    simsignal_t retransmissionInterestsBytesSentSignal;
    simsignal_t totalContentObjsBytesReceivedSignal;
    simsignal_t totalDataBytesReceivedSignal;
    simsignal_t networkInterestRetransmissionCountSignal;
    simsignal_t networkInterestInjectedCountSignal;
};

#define TRACEROUTEAPP_APP_REG_REM_EVENT_CODE            116
#define TRACEROUTEAPP_START_TRACEROUTE_EVENT_CODE       117
#define TRACEROUTEAPP_INTEREST_RETRANSMIT_EVENT_CODE    118
#define TRACEROUTEAPP_TIMEOUT_EVENT_CODE                119

#endif
