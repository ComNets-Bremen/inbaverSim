//
//  Created by Finke, Tarek & Wieker, Lars as part of the Network Simulation Project
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

network TracerouteTestNetwork
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);
        @statistic[appNetworkInterestInjectedCount](source=appNetworkInterestInjectedCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);
       	@statistic[fwdInterestToContentRatio](source=fwdInterestToContentRatio; record=min,max,count,last,vector,mean);
       	@statistic[fwdRequestToReplyRatio](source=fwdRequestToReplyRatio; record=min,max,count,last,vector,mean);
        @statistic[fwdTotalTraffic](source=fwdTotalTraffic; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdTotalTrafficBytes](source=fwdTotalTrafficBytes; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdtotalExpiredPITCountSignal](source=fwdtotalExpiredPITCountSignal; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdtotalSatisfiedPITCountSignal](source=fwdtotalSatisfiedPITCountSignal; record=min,max,count,last,vector,sum,mean);
        @statistic[fwddurationOfPITEntrySignal](source=fwddurationOfPITEntrySignal; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdtotalCacheEntriesCountSignal](source=fwdtotalCacheEntriesCountSignal; record=min,max,count,last,vector,sum,mean);
        @statistic[fwddurationOfFirstCacheEmitSignal](source=fwddurationOfFirstCacheEmitSignal; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdTotalNetworkPITEntryCount](source=fwdTotalNetworkPITEntryCount; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdTotalNetworkFIBEntryCount](source=fwdTotalNetworkFIBEntryCount; record=min,max,count,last,vector,sum,mean);
        @statistic[fwdTotalNetworkCSEntryCount](source=fwdTotalNetworkCSEntryCount; record=min,max,count,last,vector,sum,mean);

        @display("bgb=400,400;bgg=100,0,grey95;bgu=m");

    submodules:
        accessRouter: AccessRouter {
            @display("p=90,217");
        }
        wirelessAccessRouter: WirelessAccessRouter {
            @display("p=102,96");
        }
        wiredNode1: WiredNode {
            @display("p=51,307");
        }
        wiredNode2: WiredNode {
            @display("p=151,307");
        }
        wirelessNode[2]: WirelessNode {
            @display("p=103,51");
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
