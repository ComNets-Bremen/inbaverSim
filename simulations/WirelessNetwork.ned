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

        @display("bgb=410,372");

    submodules:
        wirelessAccessRouter: WirelessAccessRouter {
            @display("p=68,162");
        }
        wirelessNode: WirelessNode {
            @display("p=68,105");
        }
        coreRouter: CoreRouter {
            @display("p=226,163");
        }
        contentServer: ContentServer {
            @display("p=354,166");
        }
        demiurge: Demiurge {
            @display("p=226,27");
        }
    connections allowunconnected:
        wirelessAccessRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> coreRouter.wiredInOut++;
        coreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> contentServer.wiredInOut++;
}
