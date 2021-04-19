//
// The node specific control module of a CCN node. Only one
// of these instances are created in a simulation for every
// node.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_NUMEN_H_
#define __INBAVERSIM_NUMEN_H_

#include <omnetpp.h>

using namespace omnetpp;

class Numen : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
