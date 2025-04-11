//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
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
