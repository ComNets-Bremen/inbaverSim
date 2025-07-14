//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#ifndef __INBAVERSIM_ADMINTHINGSAPP_H_
#define __INBAVERSIM_ADMINTHINGSAPP_H_


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

class AdminThingsApp : public cSimpleModule
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
    double interestLifetime;
    int segmentSize;
    // int numSegmentsPerFile;
    double cacheTime;
    long intCount;
    long coCount;

    //Subscription Settings
    double subscriptionStartTime;
    double subscriptionDuration;
    bool subscriptionON;

    //Configuration Settings
    double configurationSettingsStartTime;
    string configurationParameter;
    double configurationValue;

    //Details needed for next Query/Response Request
    int nextdataReadingID;
    string nextSensorData;

    //Store RNP Details
    string reflexiveNamePrefix;

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
    simsignal_t contentFreshnessSignal;
    simsignal_t subContentObjectReceivedCount;
    simsignal_t totalContentObjectReceivedCount;
    simsignal_t totalInterestsSentCount;
};

#define THINGSAPP_APP_REG_REM_EVENT_CODE              116
#define THINGSAPP_NEXT_DATA_DOWNLOAD_EVENT_CODE       117
#define THINGSAPP_SUBSCRIPTION_REMINDER_CODE          118
#define THINGSAPP_CONFIG_SETTINGS_REMINDER_CODE          119

#endif
