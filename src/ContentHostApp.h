//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_CONTENTHOSTAPP_H_
#define __INBAVERSIM_CONTENTHOSTAPP_H_

#include <omnetpp.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <iostream>

#include "Demiurge.h"
#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"

using namespace omnetpp;
using namespace std;

class Demiurge;

class ContentHostApp : public cSimpleModule
{

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();

  private:
    string hostedPrefixNames;
    string dataNamePrefix;
    int segmentSize;
    int numSegmentsPerFile;
    double cacheTime;

    Demiurge *demiurgeModel;

    // list of prefixes hosted
    vector <string> hostedPrefixList;

    // list to keep already requested file details
    vector <HostedContentEntry *> hostedContentList;

    // stat signals
    simsignal_t totalInterestsBytesReceivedSignal;
    simsignal_t totalContentObjsBytesSentSignal;
    simsignal_t totalTracerouteRqstsBytesReceivedSignal;
    simsignal_t totalTracerouteRplsBytesSentSignal;
    simsignal_t totalDataBytesSentSignal;

    void processInterest(InterestMsg *interestMsg);
    void processTracerouteRqst(TracerouteRqstMsg *tracerouteRqstMsg);

};

#define CONTENTHOSTAPP_APP_REG_REM_EVENT_CODE    116

#endif
