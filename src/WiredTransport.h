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

using namespace omnetpp;

class WiredTransport : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
