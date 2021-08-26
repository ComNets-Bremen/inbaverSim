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

}

void Demiurge::handleMessage(cMessage *msg)
{

}

void Demiurge::incrementNetworkCacheHitCount()
{
    networkCacheHitCount++;
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
