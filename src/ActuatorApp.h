//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_ACTUATORAPP_H_
#define __INBAVERSIM_ACTUATORAPP_H_

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

class ActuatorApp : public cSimpleModule
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
    string actuatorPrefixName;
    int segmentSize;
    int numSegmentsPerFile;
    int maxActuatorDataReadingsToKeep;
    string actuatorDataTypes;
    double cacheTime;
    double wakeupInterval;

    Demiurge *demiurgeModel;

    // list of actuators data recently received
    vector <ActuatorDataEntry*> servedActuatorDataList;

    // list of actuator data types
    vector <string> actuatorDataTypesList;

    //Store RNP Details
    string reflexiveNamePrefix;

    //Store my RegistrationID
    string myRegistrationID;

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

#define ACTUATORAPP_APP_REG_REMINDER_CODE   116
#define ACTUATOR_REG_REMINDER_CODE          117
#define ACTUATOR_SUB_REMINDER_CODE          118


#endif
