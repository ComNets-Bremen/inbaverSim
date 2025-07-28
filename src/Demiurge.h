//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
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
        long expiredPITCount;
        long satisfiedPITCount;
        long cacheEntriesRemovedCount;
        long totalNetworkCountOfInterests;
        long totalNetworkCountOfContentObjects;

        long totalNetworkPITEntryCount;
        long totalNetworkFIBEntryCount;
        long totalNetworkCSEntryCount;


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
        void incrementExpiredPITCount();
        long getTotalExpiredPITCount();
        long getTotalSatisfiedPITCount();
        void incrementSatisfiedPITCount();
        long getTotalCacheEntriesRemovedCount();
        void incrementCacheEntriesRemovedCount();
        double durationOfFirstCacheEmit();

        long getTotalNetworkCountOfInterests();
        void incrementTotalNetworkCountOfInterests();
        long getTotalNetworkCountOfContentObjects();
        void incrementTotalNetworkCountOfContentObjects();
        void adjustTotalNetworkPITEntryCount(long oldVal, long newVal);
        long getTotalNetworkPITEntryCount();
        void adjustTotalNetworkFIBEntryCount(long oldVal, long newVal);
        long getTotalNetworkFIBEntryCount();
        void adjustTotalNetworkCSEntryCount(long oldVal, long newVal);
        long getTotalNetworkCSEntryCount();

};

#endif
