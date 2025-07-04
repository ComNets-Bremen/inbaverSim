//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_SENSINGAPP_H_
#define __INBAVERSIM_SENSINGAPP_H_

#include <omnetpp.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <iostream>
#include <cstring>
#include <cstdio>

#include "Demiurge.h"
#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"

using namespace omnetpp;
using namespace std;

class Demiurge;

class SensingApp : public cSimpleModule
{

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();
    string getSensorValues();

  private:

    string gwPrefixName;
    double interestLifetime;
    string sensorPrefixName;
    int segmentSize;
    int numSegmentsPerFile;
    double cacheTime;
    string sensedDataTypes;
    double wakeupInterval;

    Demiurge *demiurgeModel;

    // list of sensed data types
    vector <string> sensedDataTypesList;

    // last sensor reading to send
    string lastSensorReading;

    //Store RNP Details
    string reflexiveNamePrefix;

    //Store my RegistrationID
    string myRegistrationID;

    // since water and electricity has a current meter reading
    double waterCurrent;
    double electricCurrent;

    // stat signals
    simsignal_t totalInterestsBytesReceivedSignal;
    simsignal_t totalContentObjsBytesSentSignal;
    simsignal_t totalDataBytesSentSignal;

    // stat signals
    simsignal_t segmentDownloadDurationSignal;
    simsignal_t totalInterestsBytesSentSignal;
    simsignal_t totalContentObjsBytesReceivedSignal;
    simsignal_t totalDataBytesReceivedSignal;
    simsignal_t networkInterestInjectedCountSignal;

};

#define SENSINGAPP_APP_REG_REMINDER_CODE   116
#define SENSINGAPP_WAKEUP_REMINDER_CODE    117
#define SENSOR_REG_REMINDER_CODE           118

#endif
