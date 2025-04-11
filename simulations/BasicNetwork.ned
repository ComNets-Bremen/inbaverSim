//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim.simulations;

import inbaversim.ContentServer;
import inbaversim.Demiurge;
import inbaversim.WirelessAccessRouter;
import inbaversim.WirelessNode;
import ned.DatarateChannel;

//
// A basic CCN network for testing.
//
network BasicNetwork
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);
        @display("bgb=410,372");

    submodules:
        wirelessAccessRouter: WirelessAccessRouter {
            @display("p=206,149");
        }
        wirelessNode[2]: WirelessNode {
            @display("p=82,104");
        }
        contentServer: ContentServer {
            @display("p=295,236");
        }
        demiurge: Demiurge {
            @display("p=226,27");
        }

    connections allowunconnected:
        wirelessAccessRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> contentServer.wiredInOut++;
}
