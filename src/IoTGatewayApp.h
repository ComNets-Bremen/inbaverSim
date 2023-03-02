//
// An application that connects an IoT with the Internet
// and is able to handle the functionality required to
// manage the duty cycled IoT and the permently-on
// Internet. It implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 19-feb-2023
//
//

#ifndef __INBAVERSIM_IOTGATEWAYAPP_H_
#define __INBAVERSIM_IOTGATEWAYAPP_H_

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

    Demiurge *demiurgeModel;

    // list of sensors and the recent data received
    vector <SensorEntry*> servedSensorList;

    // stat signals
    simsignal_t totalInterestsBytesReceivedSignal;
    simsignal_t totalContentObjsBytesSentSignal;
    simsignal_t totalDataBytesSentSignal;

};

#define IOTGATEWAYAPP_APP_REG_REM_EVENT_CODE             116

#endif
