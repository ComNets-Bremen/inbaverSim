//
// The global control module of a CCN network. Only one
// of these instances are created in a simulation.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
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
