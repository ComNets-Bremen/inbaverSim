//
// The global control module of a CCN network. Only one
// of these instances are created in a simulation.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_DEMIURGE_H_
#define __INBAVERSIM_DEMIURGE_H_

#include <omnetpp.h>
#include "inbaver.h"

using namespace omnetpp;
using namespace std;

class NodeInfo;
class SameWirelessGroup;

class Demiurge : public cSimpleModule
{
    private:
        double networkHitRatio;
        bool networkHitRatioFirstTime;
        double networkMissRatio;
        bool networkMissRatioFirstTime;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        list<NodeInfo*> allNodesList;
        list<SameWirelessGroup*> sameWirelessGroupList;

        void updateNodeHitRatio(double nodeHitRatio);
        double getNetworkHitRatio();
        void updateNodeMissRatio(double nodeMissRatio);
        double getNetworkMissRatio();

};

#endif
