//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_THINGSAPP_H_
#define __INBAVERSIM_THINGSAPP_H_

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
 * A simple application to retrieve data from a sensor network
 * managed by a gateway, that implements the IApplication
 * interface.
 */
class ThingsApp : public cSimpleModule
{
  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();


  private:

    double sensorDataRetrievalStartTime;
    string requestedSensorNetworkPrefixName;
    string sensorDataNames;
    int maxSensorDataReadingsToKeep;
    int maxHopsAllowed;

    Demiurge *demiurgeModel;
    Numen *numenModel;

    // list of sensors data recently received
    vector <SensorDataEntry*> servedSensorDataList;

    // stat signals
    simsignal_t totalInterestsBytesSentSignal;
    simsignal_t totalContentObjsBytesReceivedSignal;
    simsignal_t totalDataBytesReceivedSignal;
    simsignal_t networkInterestRetransmissionCountSignal;
    simsignal_t networkInterestInjectedCountSignal;
};

#define THINGSAPP_APP_REG_REM_EVENT_CODE              116
#define THINGSAPP_NEXT_DATA_DOWNLOAD_EVENT_CODE       117

#endif
