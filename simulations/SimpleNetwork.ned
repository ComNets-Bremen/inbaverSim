//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim.simulations;

import inbaversim.AccessRouter;
import inbaversim.ContentServer;
import inbaversim.CoreRouter;
import inbaversim.Demiurge;
import inbaversim.WiredNode;
import inbaversim.WirelessAccessRouter;
import inbaversim.WirelessNode;
import ned.DatarateChannel;


//
// A simple CCN network.
//
network SimpleNetwork
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);

        @display("bgb=410,372");

    submodules:
        accessRouter: AccessRouter {
            @display("p=90,217");
        }
        wirelessAccessRouter: WirelessAccessRouter {
            @display("p=90,119");
        }
        wiredNode1: WiredNode {
            @display("p=51,307");
        }
        wiredNode2: WiredNode {
            @display("p=151,307");
        }
        wirelessNode[2]: WirelessNode {
            @display("p=72,32");
        }
        coreRouter1: CoreRouter {
            @display("p=246,237");
        }
        coreRouter2: CoreRouter {
            @display("p=246,107");
        }
        contentServer1: ContentServer {
            @display("p=336,284");
        }
        contentServer2: ContentServer {
            @display("p=336,153");
        }
        demiurge: Demiurge {
            @display("p=226,27");
        }
    connections allowunconnected:
        wiredNode1.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> accessRouter.wiredInOut++;
        wiredNode2.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> accessRouter.wiredInOut++;
        accessRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> coreRouter1.wiredInOut++;
        wirelessAccessRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> coreRouter1.wiredInOut++;
        coreRouter1.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> contentServer1.wiredInOut++;
        coreRouter1.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> coreRouter2.wiredInOut++;
        coreRouter2.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> contentServer2.wiredInOut++;
}
