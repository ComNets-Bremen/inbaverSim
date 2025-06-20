//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim.simulations;

import inbaversim.AccessRouter;
import inbaversim.CCNInternet;
import inbaversim.ContentServer;
import inbaversim.CoreRouter;
import inbaversim.Demiurge;
import inbaversim.IoTGateway;
import inbaversim.WiredNode;
import inbaversim.WirelessAccessRouter;
import inbaversim.WirelessNode;
import inbaversim.IoTGateway;
import inbaversim.WirelessSensorNode;
import inbaversim.WirelessActuatorNode;
import inet.node.wireless.AccessPoint;
import ned.DatarateChannel;



network SimpleIoTNetwork
{
    parameters:

		// statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector,mean);
        @statistic[appNetworkInterestInjectedCount](source=appNetworkInterestInjectedCount; record=last,vector);
        @statistic[appContentFreshnessSignal](source=appContentFreshnessSignal; record=min,max,count,last,vector,mean);
        @statistic[apptotalContentObjectReceivedCount](source=apptotalContentObjectReceivedCount; record=min,max,count,last,vector,sum,mean);
        @statistic[apptotalInterestsSentCount](source=apptotalInterestsSentCount; record=min,max,count,last,vector,sum,mean);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector,mean);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector,mean);
        @statistic[fwdInterestToContentRatio](source=fwdInterestToContentRatio; record=min,max,count,last,vector,mean);
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

        @display("bgb=600,300;bgg=100,0,grey95;bgu=m");

    submodules:
        demiurge: Demiurge {
            @display("p=50,27;is=m");
        }
        Internet: CCNInternet {
            @display("p=161.53847,37.69231");
        }
        zfnRouter: CoreRouter {
            @display("p=350.76926,37.69231;is=s");
        }
        zfnAP: WirelessAccessRouter {
            @display("p=468,51;is=s");
        }
        zfnUser: WirelessNode {
            @display("p=498,52;is=vs");
        }



        bulb: WirelessActuatorNode {
            @display("p=106,209;is=s");
        }
        furnace: WirelessActuatorNode {
            @display("p=56,187;is=s");
        }
        GW: IoTGateway {
            @display("p=106,149;is=s");
        }
        lightSen: WirelessSensorNode {
            @display("p=56,137;is=vs");
        }
        motionSen: WirelessSensorNode {
            @display("p=156,161;is=vs");
        }
        tempSen: WirelessSensorNode {
            @display("p=149,195;is=vs");
        }

        CoreRouter: CoreRouter {
            @display("p=245.38463,76.15385;is=s");
        }
        Management: WiredNode {
            @display("p=419,133;is=s");
        }
        Router: AccessRouter {
            @display("p=349,95;is=s");
        }

        AP: WirelessAccessRouter {
            @display("p=126,136;is=s");
        }
        User: WirelessNode {
            @display("p=109,110;is=vs");
        }

        ContentServer: ContentServer {
            @display("p=278,161;is=s");
        }

    connections allowunconnected:

		//Internet
        zfnRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=red,4"); } <--> Internet.wiredInOut[0];
        zfnAP.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;

        //Internal network
        CoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        CoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps; } <--> ContentServer.wiredInOut++;

        Management.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> Router.wiredInOut++;
        Router.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> CoreRouter.wiredInOut++;
        AP.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> CoreRouter.wiredInOut++;

}

