//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_IOTGATEWAYAPP_H_
#define __INBAVERSIM_IOTGATEWAYAPP_H_

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

class IoTGatewayApp : public cSimpleModule
{

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();

  private:
    string hostedPrefixName;
    string hostedPrefixNameForSensorNet;
    string sensorPrefixName;
    double interestLifetime;
    int maximumSensorReadingsToKeep;
    double sensorDataRetrievalStartTime;
    int maxHopsAllowed;
    string otherGatewayAvailable;

    //For Controller action
    double temperatureLowerLimit;
    double temperatureHigherLimit;
    double lightIntensityLowerLimit;
    double lightIntensityUpperLimit;

    //Reminder for de-subscription
    vector <SubscriptionExpiryData*> subscriptionExpiryData;

    // interest retransmission event
    cMessage *endOfUserSubscriptionEvent;

    Demiurge *demiurgeModel;

    // list of sensors and the recent data received
    vector <SensorEntry*> servedSensorList;

    // list of actuators and the recent data received
    vector <ActuatorEntry*> servedActuatorList;

    //list of Subscriptions
    vector <SubscriptionData*> subscriptionData;


    // stat signals
    simsignal_t totalInterestsBytesReceivedSignal;
    simsignal_t totalContentObjsBytesSentSignal;
    simsignal_t totalDataBytesSentSignal;
    simsignal_t subContentObjectsSentCount;

};

#define IOTGATEWAYAPP_APP_REG_REM_EVENT_CODE             116
#define IOTGATEWAYAPP_QUERY_OTHER_GATEWAY_EVENT_CODE     117
#define IOTGATEWAYAPP_SUB_END_REMINDER_EVENT_CODE        118

#endif
