//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_WIREDTRANSPORT_H_
#define __INBAVERSIM_WIREDTRANSPORT_H_

#include <omnetpp.h>
#include <queue>
#include "inbaver.h"
#include "InternalMessages_m.h"
#include "TransportMessages_m.h"
#include "Numen.h"

#include "inet/mobility/contract/IMobility.h"
#include "Demiurge.h"

using namespace omnetpp;
using namespace std;

class Demiurge;
class Numen;

/**
 * A wired transport that implements the ITransport
 * interface.
 */
class WiredTransport : public cSimpleModule
{
  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();

  private:
      // model parameters
      string wiredTechnology;
      double dataRate;
      double packetErrorRate;
      int headerSize;

      // globally unique MAC-like address
      string macAddress;

      // node ID
      long nodeID;

      // model info
      Demiurge *demiurgeModel;
      Numen *numenModel;
      cModule *nodeModel;
      inet::IMobility *mobilityModel;

      // queues messages arriving from forwarding
      queue<cMessage*> messageQueue;
      cMessage *msgSendCompletedEvent;

      void buildMACLikeAddress();
      void getDemiurgeModel();
      void getAllOtherModels();
      void registerWiredTransportWithDemiurge();
      void processOutgoingMessage(cMessage *msg);
      void processIncomingMessage(cMessage *msg);
      void sendOutgoingMessage(cMessage *msg);

      simsignal_t totalWiredBytesReceivedSignal;
      simsignal_t totalWiredBytesSentSignal;

};

#define WIREDTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE    106
#define WIREDTRANSPORT_MSG_SEND_COMPLETED_EVENT_CODE   107

#endif
