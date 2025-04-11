//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim.simulations;

import inbaversim.AccessRouter;
import inbaversim.Demiurge;
import inbaversim.WiredNode;
import inbaversim.IoTGateway;
import inbaversim.WirelessSensorNode;
import ned.DatarateChannel;


//
// A simple CCN based sensor network.
//
network SimpleSensorNetwork
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);

        @display("bgb=301.15,206.15");

    submodules:
        demiurge: Demiurge {
            @display("p=102,23;is=m");
        }
        router: AccessRouter {
            @display("p=102,81;is=s");
        }
        frank: WiredNode {
            @display("p=37,144;is=s");
        }
        gw: IoTGateway {
            @display("p=184,131;is=s");
        }
        climateSensor: WirelessSensorNode {
            @display("p=139,155;is=s");
        }
        plantSensor: WirelessSensorNode {
            @display("p=234,155;is=s");
        }

    connections allowunconnected:
        frank.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> router.wiredInOut++;
        gw.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> router.wiredInOut++;

}
