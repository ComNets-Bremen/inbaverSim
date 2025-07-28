//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "Demiurge.h"

Define_Module(Demiurge);

void Demiurge::initialize()
{
    networkCacheHitCount = 0;
    networkCacheMissCount = 0;
    networkInterstRetransmissionCount = 0;
    totalTraffic = 0;
    totalTrafficBytes = 0;
    subContentObjectSentCount = 0;
    subContentObjectReceivedCount = 0;
    expiredPITCount = 0;
    satisfiedPITCount = 0;
    cacheEntriesRemovedCount = 0;
    totalNetworkCountOfInterests = 0;
    totalNetworkCountOfContentObjects = 0;

    totalNetworkPITEntryCount = 0;
    totalNetworkFIBEntryCount = 0;
    totalNetworkCSEntryCount = 0;

}

void Demiurge::handleMessage(cMessage *msg)
{

}

void Demiurge::incrementNetworkCacheHitCount()
{
    networkCacheHitCount++;
}

simtime_t Demiurge::getContentFreshness()
{
    return contentFreshness;
}
void Demiurge::incrementSubContentobjectSentCount()
{
    subContentObjectSentCount++;
}
void Demiurge::incrementSubContentobjectReceivedCount()
{
    subContentObjectReceivedCount++;
}

long Demiurge::getSubContentobjectSentCount()
{
    return subContentObjectSentCount;
}
long Demiurge::getSubContentobjectReceivedCount()
{
    return subContentObjectReceivedCount;
}

void Demiurge::incrementTrafficCount()
{
    totalTraffic++;
}

long Demiurge:: getTotalTrafficCount()
{
    return totalTraffic;
}

long Demiurge:: getTotalTrafficBytesCount()
{
    return totalTrafficBytes;
}

double Demiurge::getNetworkCacheHitRatio()
{
    return (double) networkCacheHitCount / (networkCacheHitCount + networkCacheMissCount);
}

void Demiurge::incrementNetworkCacheMissCount()
{
    networkCacheMissCount++;
}

double Demiurge::getNetworkCacheMissRatio()
{
    return (double) networkCacheMissCount / (networkCacheHitCount + networkCacheMissCount);
}

void Demiurge::incrementNetworkInterestRetransmissionCount()
{
    networkInterstRetransmissionCount++;
}

long Demiurge::getNetworkInterestRetransmissionCount()
{
    return networkInterstRetransmissionCount;
}

void Demiurge::incrementNetworkInterestInjectedCount()
{
    networkInterstInjectedCount++;
}

long Demiurge::getNetworkInterestInjectedCount()
{
    return networkInterstInjectedCount;
}
void Demiurge::incrementExpiredPITCount()
{
    expiredPITCount++;
}

long Demiurge:: getTotalExpiredPITCount()
{
    return expiredPITCount;
}
void Demiurge::incrementSatisfiedPITCount()
{
    satisfiedPITCount++;
}

long Demiurge:: getTotalSatisfiedPITCount()
{
    return satisfiedPITCount;
}
void Demiurge::incrementCacheEntriesRemovedCount()
{
    cacheEntriesRemovedCount++;
}

long Demiurge:: getTotalCacheEntriesRemovedCount()
{
    return cacheEntriesRemovedCount;
}
double Demiurge::durationOfFirstCacheEmit()
{
    return (double) (simTime()).dbl();
}

long Demiurge::getTotalNetworkCountOfInterests()
{
    return totalNetworkCountOfInterests;
}

void Demiurge::incrementTotalNetworkCountOfInterests()
{
    totalNetworkCountOfInterests++;
}

long Demiurge::getTotalNetworkCountOfContentObjects()
{
    return totalNetworkCountOfContentObjects;
}

void Demiurge::incrementTotalNetworkCountOfContentObjects()
{
    totalNetworkCountOfContentObjects++;
}



void Demiurge::adjustTotalNetworkPITEntryCount(long oldVal, long newVal)
{
    totalNetworkPITEntryCount = totalNetworkPITEntryCount - oldVal + newVal;
}

long Demiurge::getTotalNetworkPITEntryCount()
{
    return totalNetworkPITEntryCount;
}

void Demiurge::adjustTotalNetworkFIBEntryCount(long oldVal, long newVal)
{
    totalNetworkFIBEntryCount = totalNetworkFIBEntryCount - oldVal + newVal;
}

long Demiurge::getTotalNetworkFIBEntryCount()
{
    return totalNetworkFIBEntryCount;
}

void Demiurge::adjustTotalNetworkCSEntryCount(long oldVal, long newVal)
{
    totalNetworkCSEntryCount = totalNetworkCSEntryCount - oldVal + newVal;
}

long Demiurge::getTotalNetworkCSEntryCount()
{
    return totalNetworkCSEntryCount;
}

