//
// The global control module of a CCN network. Only one
// of these instances are created in a simulation.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_DEUS_H_
#define __INBAVERSIM_DEUS_H_

#include <omnetpp.h>
#include "inbaver.h"

using namespace omnetpp;
using namespace std;

class WirelessTransport;
class WiredTransport;
class Numen;
class NodeInfo;
class SameWirelessGroup;

class Deus : public cSimpleModule
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        void registerWirelessTransport(long nodeID, string macAddress, cModule *nodeModel,
                        inet::IMobility *mobilityModel, WirelessTransport *wirelessTransportModel, Numen *numenModel,
                        string wirelessTechnology, string operationMode, string connectString);

    private:
        list<NodeInfo*> allNodesList;
        list<SameWirelessGroup*> sameWirelessGroupList;

};

#endif
