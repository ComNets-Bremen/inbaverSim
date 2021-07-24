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
    networkHitRatio = 0.0;
    networkMissRatio = 0.0;
    networkHitRatioFirstTime = true;
    networkMissRatioFirstTime = true;

}

void Demiurge::handleMessage(cMessage *msg)
{

}

void Demiurge::updateNodeHitRatio(double nodeHitRatio)
{
    if (!networkHitRatioFirstTime) {
        networkHitRatio = (networkHitRatio + nodeHitRatio) / 2;
    } else {
        networkHitRatio = nodeHitRatio;
        networkHitRatioFirstTime = false;
    }
}

double Demiurge::getNetworkHitRatio()
{
    return networkHitRatio;
}

void Demiurge::updateNodeMissRatio(double nodeMissRatio)
{
    if (!networkMissRatioFirstTime) {
        networkMissRatio = (networkMissRatio + nodeMissRatio) / 2;
    } else {
        networkMissRatio = nodeMissRatio;
        networkMissRatioFirstTime = false;
    }

}

double Demiurge::getNetworkMissRatio()
{
    return networkMissRatio;
}
