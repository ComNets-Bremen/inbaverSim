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

};

#endif
