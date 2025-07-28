//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
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
// A simple CCN wireless network.
//
network WirelessNetwork
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);
        @statistic[appNetworkInterestInjectedCount](source=appNetworkInterestInjectedCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);

        @display("bgb=400,400;bgg=100,0,grey95;bgu=m");

    submodules:
        wirelessAccessRouter: WirelessAccessRouter {
            @display("p=100,98");
        }
        wirelessNode: WirelessNode {
            @display("p=62,63");
        }
        coreRouter: CoreRouter {
            @display("p=226,163");
        }
        contentServer: ContentServer {
            @display("p=354,166");
        }
        demiurge: Demiurge {
            @display("p=280,27");
        }
    connections allowunconnected:
        wirelessAccessRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> coreRouter.wiredInOut++;
        coreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> contentServer.wiredInOut++;
}
