//
// The global control module of a CCN network. Only one
// of these instances are created in a simulation.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#include "Deus.h"

Define_Module(Deus);

void Deus::initialize()
{

}

void Deus::handleMessage(cMessage *msg)
{

}

void Deus::registerWirelessTransport(long nodeID, string macAddress, cModule *nodeModel,
                inet::IMobility *mobilityModel, WirelessTransport *wirelessTransportModel, Numen *numenModel,
                string wirelessTechnology, string operationMode, string connectString)
{

    // check if node is already registered
    bool found = false;
    NodeInfo *nodeInfo;
    list<NodeInfo*>::iterator iteratorAllNodesList = allNodesList.begin();
    while (iteratorAllNodesList != allNodesList.end()) {
        nodeInfo = *iteratorAllNodesList;
        if (nodeInfo->nodeID == nodeID) {
            found = true;
            break;
        }

        iteratorAllNodesList++;
    }

    if(!found) {
        nodeInfo = new NodeInfo;
        nodeInfo->nodeID = nodeID;
        nodeInfo->nodeModel = nodeModel;
        nodeInfo->mobilityModel = mobilityModel;
        nodeInfo->numenModel = numenModel;
        allNodesList.push_back(nodeInfo);
    }

    WirelessTransportInfo *wirelessTransportInfo = new WirelessTransportInfo;
    wirelessTransportInfo->macAddress = macAddress;
    wirelessTransportInfo->wirelessTransportModel = wirelessTransportModel;
    wirelessTransportInfo->mobilityModel = mobilityModel;
    wirelessTransportInfo->nodeModel = nodeModel;
    wirelessTransportInfo->numenModel = numenModel;
    wirelessTransportInfo->wirelessTechnology = wirelessTechnology;
    wirelessTransportInfo->operationMode = operationMode;
    wirelessTransportInfo->connectString = connectString;
    nodeInfo->wirelessTransportInfoList.push_back(wirelessTransportInfo);

     // check and update transport belonging to the same wireless group
    found = false;
    SameWirelessGroup *sameWirelessGroup;
    list<SameWirelessGroup*>::iterator iteratorSameWirelessGroupList = sameWirelessGroupList.begin();
    while (iteratorSameWirelessGroupList != sameWirelessGroupList.end()) {
        sameWirelessGroup = *iteratorSameWirelessGroupList;
        if (sameWirelessGroup->wirelessTechnology ==  wirelessTechnology
                && sameWirelessGroup->operationMode == operationMode
                && sameWirelessGroup->connectString == connectString) {
            found = true;
            break;
        }

        iteratorSameWirelessGroupList++;
    }

    if (!found) {
        sameWirelessGroup = new SameWirelessGroup;
        sameWirelessGroup->wirelessTechnology = wirelessTechnology;
        sameWirelessGroup->operationMode = operationMode;
        sameWirelessGroup->connectString = connectString;
        sameWirelessGroupList.push_back(sameWirelessGroup);
    }

    sameWirelessGroup->wirelessTransportInfoList.push_back(wirelessTransportInfo);
}
