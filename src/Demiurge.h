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

class Demiurge : public cSimpleModule
{

    private:
        long networkCacheHitCount;
        long networkCacheMissCount;
        long networkInterstInjectedCount;
        long networkInterstRetransmissionCount;
        long totalTraffic;
        long subContentObjectSentCount;
        long subContentObjectReceivedCount;


    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        list<NodeInfo*> allNodesList;
        list<SameWirelessGroup*> sameWirelessGroupList;
        simtime_t contentFreshness;
        long totalTrafficBytes;

        void incrementNetworkCacheHitCount();
        double getNetworkCacheHitRatio();
        void incrementNetworkCacheMissCount();
        double getNetworkCacheMissRatio();
        void incrementNetworkInterestRetransmissionCount();
        long getNetworkInterestRetransmissionCount();
        void incrementNetworkInterestInjectedCount();
        long getNetworkInterestInjectedCount();
        void incrementTrafficCount();
        long getTotalTrafficCount();
        long getTotalTrafficBytesCount();
        simtime_t getContentFreshness();
        void incrementSubContentobjectSentCount();
        long getSubContentobjectSentCount();
        void incrementSubContentobjectReceivedCount();
        long getSubContentobjectReceivedCount();

};

#endif
