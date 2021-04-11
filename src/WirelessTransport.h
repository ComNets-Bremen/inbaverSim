//
// A wireless transport that implements the ITransport
// interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_WIRELESSTRANSPORT_H_
#define __INBAVERSIM_WIRELESSTRANSPORT_H_

#include <omnetpp.h>
#include "inbaver.h"
#include "InternalMessages_m.h"
#include "TransportMessages_m.h"
#include "Deus.h"
#include "Numen.h"

#include "inet/mobility/contract/IMobility.h"

using namespace omnetpp;
using namespace std;

class Deus;
class Numen;
class WirelessTransportInfo;

class WirelessTransport : public cSimpleModule
{
    protected:
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);
        virtual int numInitStages() const {return 3;}
        virtual void finish();

    private:
        // model parameters
        string wirelessTechnology;
        string operationMode;
        string connectString;
        double wirelessRange;
        double dataRate;
        double packetErrorRate;
        double scanInterval;
        int headerSize;

        // globally unique MAC-like address
        string macAddress;

        // node ID
        long nodeID;

        // model info
        Deus *deusModel;
        Numen *numenModel;
        cModule *nodeModel;
        inet::IMobility *mobilityModel;

        // variables used when in client mode
        WirelessTransportInfo *lastConnectedAP;

        void buildMACLikeAddress();
        void getDeusModel();
        void getAllOtherModels();
        void registerWirelessTransportWithDeus();
        bool inWirelessRange(inet::IMobility *neighbourMobilityModel, inet::IMobility *ownMobilityModel, double radius);
        void processOutgoingMessage(cMessage *msg);
        void processOutgoingOnAPNode(cMessage *msg);
        void processOutgoingOnClientNode(cMessage *msg);
        void processOutgoingOnDirectNode(cMessage *msg);
        void processIncomingMessage(cMessage *msg);

};

#define WIRELESSTRANSPORT_SIMMODULEINFO                 simTime() << ">!<" << getParentModule()->getFullName() << ">!<WirelessTransport>!<"
#define WIRELESSTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE    116

#endif
