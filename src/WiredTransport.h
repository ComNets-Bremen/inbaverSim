//
// A wired transport that implements the ITransport
// interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_WIREDTRANSPORT_H_
#define __INBAVERSIM_WIREDTRANSPORT_H_

#include <omnetpp.h>
#include <queue>
#include "inbaver.h"
#include "InternalMessages_m.h"
#include "TransportMessages_m.h"
#include "Deus.h"
#include "Numen.h"

#include "inet/mobility/contract/IMobility.h"

using namespace omnetpp;
using namespace std;

class Deus;
class Numen;

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
      Deus *deusModel;
      Numen *numenModel;
      cModule *nodeModel;
      inet::IMobility *mobilityModel;

      // queues messages arriving from forwarding
      queue<cMessage*> messageQueue;
      cMessage *msgSendCompletedEvent;

      void buildMACLikeAddress();
      void getDeusModel();
      void getAllOtherModels();
      void registerWiredTransportWithDeus();
      void processOutgoingMessage(cMessage *msg);
      void processIncomingMessage(cMessage *msg);
      void sendOutgoingMessage(cMessage *msg);

      simsignal_t totalWiredBytesReceivedSignal;
      simsignal_t totalWiredBytesSentSignal;

};

#define WIREDTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE    106
#define WIREDTRANSPORT_MSG_SEND_COMPLETED_EVENT_CODE   107

#endif
