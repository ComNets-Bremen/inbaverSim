//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_CONTENTDOWNLOADAPP_H_
#define __INBAVERSIM_CONTENTDOWNLOADAPP_H_

#include <omnetpp.h>

#include "Demiurge.h"
#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"
#include "Numen.h"

using namespace omnetpp;
using namespace std;

class Demiurge;
class Numen;

/**
 * A file download client (content requester) application that
 * implements the IApplication interface.
 */
class ContentDownloadApp : public cSimpleModule
{
  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();


  private:
    double interContentDownloadInterval;
    string requestedPrefixNames;
    string dataNamePrefix;
    int maxHopsAllowed;
    double interestRetransmitTimeout;

    Demiurge *demiurgeModel;
    Numen *numenModel;

    vector <string> requestedPrefixList;

    // start content download event
    cMessage *contentDownloadStartEvent;

    // interest retransmission event
    cMessage *interestRetransmitEvent;

    // details of current download
    string requestingPrefixName;
    string requestingDataName;
    int requestedSegNum;
    int totalSegments;
    simtime_t contentDownloadStartTime;
    simtime_t lastInterestSentTime;


    // stat signals
    simsignal_t contentDownloadDurationSignal;
    simsignal_t segmentDownloadDurationSignal;
    simsignal_t totalInterestsBytesSentSignal;
    simsignal_t retransmissionInterestsBytesSentSignal;
    simsignal_t totalContentObjsBytesReceivedSignal;
    simsignal_t totalDataBytesReceivedSignal;
    simsignal_t networkInterestRetransmissionCountSignal;
    simsignal_t networkInterestInjectedCountSignal;
};

#define CONTENTDOWNLOADAPP_APP_REG_REM_EVENT_CODE              116
#define CONTENTDOWNLOADAPP_START_CONTENT_DOWNLOAD_EVENT_CODE   117
#define CONTENTDOWNLOADAPP_INTEREST_RETRANSMIT_EVENT_CODE      118

#endif
