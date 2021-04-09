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
#include "inet/mobility/contract/IMobility.h"

using namespace omnetpp;
using namespace std;

class Deus;
class Numen;

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

        // model info
        Deus *deusModel;
        Numen *numenModel;
        cModule *nodeModel;
        inet::IMobility *mobilityModel;

        bool inWirelessRange(inet::IMobility *neighbourMobilityModel, inet::IMobility *ownMobilityModel, double radius);

};

#define WIRELESSTRANSPORT_SIMMODULEINFO                 simTime() << ">!<" << getParentModule()->getFullName() << ">!<WirelessTransport>!<"
#define WIRELESSTRANSPORT_TRANSPORT_REG_REM_EVENT_CODE    116

#endif
