//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_NUMEN_H_
#define __INBAVERSIM_NUMEN_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * The node specific control module of a CCN node. Only one
 * of these instances are created in a simulation for every
 * node.
 */
class Numen : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
