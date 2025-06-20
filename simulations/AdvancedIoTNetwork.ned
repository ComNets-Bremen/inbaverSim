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



network AdvancedIoTNetwork
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

        @display("bgb=1300,600;bgg=100,0,grey95;bgu=m");

    submodules:
        S0Router: AccessRouter {
            @display("p=342,132;is=s");
        }
        S0Management: WiredNode {
            @display("p=389,80;is=s");
        }
        S0CoreRouter: CoreRouter {
            @display("p=342,182;is=s");
        }
        S0ContentServer: ContentServer {
            @display("p=262,103;is=s");
        }
        demiurge: Demiurge {
            @display("p=50,27;is=m");
        }
        Internet: CCNInternet {
            @display("p=594.1,45.5");
        }
        zfnRouter: CoreRouter {
            @display("p=605.8,266.5;is=s");
        }



        bulb12: WirelessActuatorNode {
            @display("p=266.86392,194.67458;is=s");
        }
        furnace12: WirelessActuatorNode {
            @display("p=256.80475,214.2012;is=s");
        }
        GW12: IoTGateway {
            @display("p=296.44974,219.52666;is=s");
        }
        lightSen12: WirelessSensorNode {
            @display("p=288.1657,194.67458;is=vs");
        }
        motionSen12: WirelessSensorNode {
            @display("p=278.10654,225.44382;is=vs");
        }
        tempSen12: WirelessSensorNode {
            @display("p=237.86984,225.44382;is=vs");
        }

        bulb13: WirelessActuatorNode {
            @display("p=114.61539,456.15387;is=s");
        }
        furnace13: WirelessActuatorNode {
            @display("p=111.53847,416.15387;is=s");
        }
        GW13: IoTGateway {
            @display("p=146.92308,448.46155;is=s");
        }
        lightSen13: WirelessSensorNode {
            @display("p=114.61539,436.15387;is=vs");
        }
        motionSen13: WirelessSensorNode {
            @display("p=155.38463,424.6154;is=vs");
        }
        tempSen13: WirelessSensorNode {
            @display("p=140,408.46155;is=vs");
        }
        bulb14: WirelessActuatorNode {
            @display("p=210.00002,512.30774;is=s");
        }
        furnace14: WirelessActuatorNode {
            @display("p=236.15385,512.30774;is=s");
        }
        GW14: IoTGateway {
            @display("p=213.84616,474.6154;is=s");
        }
        motionSen14: WirelessSensorNode {
            @display("p=246.15385,491.53848;is=vs");
        }
        tempSen14: WirelessSensorNode {
            @display("p=196.15385,494.61542;is=vs");
        }
        bulb15: WirelessActuatorNode {
            @display("p=309.96817,293.127;is=s");
        }
        furnace15: WirelessActuatorNode {
            @display("p=329.99548,310.8785;is=s");
        }
        GW15: IoTGateway {
            @display("p=332.2713,258.5344;is=s");
        }
        motionSen15: WirelessSensorNode {
            @display("p=341.8298,279.47205;is=vs");
        }
        tempSen15: WirelessSensorNode {
            @display("p=306.782,267.63773;is=vs");
        }
        bulb16: WirelessActuatorNode {
            @display("p=420.11838,220.30043;is=s");
        }
        furnace16: WirelessActuatorNode {
            @display("p=412.83572,247.6104;is=s");
        }
        GW16: IoTGateway {
            @display("p=398.7256,225.30727;is=s");
        }
        motionSen16: WirelessSensorNode {
            @display("p=405.55307,188.4388;is=vs");
        }
        tempSen16: WirelessSensorNode {
            @display("p=436.0492,199.3628;is=vs");
        }
        lightSen14: WirelessSensorNode {
            @display("p=236.15385,474.6154;is=vs");
        }
        lightSen15: WirelessSensorNode {
            @display("p=351.84344,293.127;is=vs");
        }
        lightSen16: WirelessSensorNode {
            @display("p=440.1457,233.95541;is=vs");
        }
        bulb17: WirelessActuatorNode {
            @display("p=804.1421,399.40833;is=s");
        }
        furnace17: WirelessActuatorNode {
            @display("p=838.4616,366.86395;is=s");
        }
        GW17: IoTGateway {
            @display("p=838.4616,393.49118;is=s");
        }
        lightSen17: WirelessSensorNode {
            @display("p=820.1184,410.65094;is=vs");
        }
        motionSen17: WirelessSensorNode {
            @display("p=869.8226,408.8758;is=vs");
        }
        tempSen17: WirelessSensorNode {
            @display("p=820.1184,375.14798;is=vs");
        }
        bulb18: WirelessActuatorNode {
            @display("p=883.43207,491.1243;is=s");
        }
        furnace18: WirelessActuatorNode {
            @display("p=833.13617,471.00598;is=s");
        }
        GW18: IoTGateway {
            @display("p=863.3137,462.13022;is=s");
        }
        lightSen18: WirelessSensorNode {
            @display("p=855.02966,491.1243;is=vs");
        }
        motionSen18: WirelessSensorNode {
            @display("p=890.53265,453.2545;is=vs");
        }
        tempSen18: WirelessSensorNode {
            @display("p=898.225,473.37283;is=vs");
        }
        bulb19: WirelessActuatorNode {
            @display("p=1017.2964,301.77518;is=s");
        }
        furnace19: WirelessActuatorNode {
            @display("p=994.08295,306.782;is=s");
        }
        GW19: IoTGateway {
            @display("p=1025.9446,321.34732;is=s");
        }
        lightSen19: WirelessSensorNode {
            @display("p=976.3315,329.08514;is=vs");
        }
        motionSen19: WirelessSensorNode {
            @display("p=1025.9446,344.5608;is=vs");
        }
        tempSen19: WirelessSensorNode {
            @display("p=1005.0069,342.28497;is=vs");
        }
        bulb20: WirelessActuatorNode {
            @display("p=1109.6952,351.3883;is=s");
        }
        furnace20: WirelessActuatorNode {
            @display("p=1135.1844,293.127;is=s");
        }
        GW20: IoTGateway {
            @display("p=1109.6952,315.43018;is=s");
        }
        lightSen20: WirelessSensorNode {
            @display("p=1109.6952,293.127;is=vs");
        }
        motionSen20: WirelessSensorNode {
            @display("p=1143.8326,315.8853;is=vs");
        }
        tempSen20: WirelessSensorNode {
            @display("p=1135.1844,335.4575;is=vs");
        }

        OContentServer: ContentServer {
            @display("p=869.7,169;is=s");
        }
        OCoreRouter: CoreRouter {
            @display("p=955,204;is=s");
        }
        OManagement: WiredNode {
            @display("p=1143.7871,152.66273;is=s");
        }
        ORouter: AccessRouter {
            @display("p=984.0238,152.66273;is=s");
        }

        S0AP1: WirelessAccessRouter {
            @display("p=345.01596,229.40375;is=s");
        }
        S0AP2: WirelessAccessRouter {
            @display("p=176.15385,465.38464;is=s");
        }

        OAP1: WirelessAccessRouter {
            @display("p=842.0119,430.7693;is=s");
        }
        OAP2: WirelessAccessRouter {
            @display("p=1063.2682,321.34732;is=s");
        }
        OUser2: WirelessNode {
            @display("p=810.7693,443.84616;is=vs");
        }
        OUser1: WirelessNode {
            @display("p=1065.0889,286.29953;is=vs");
        }

        S0User1: WirelessNode {
            @display("p=364.13293,199.3628;is=vs");
        }
        zfnAP: WirelessAccessRouter {
            @display("p=988.46155,44.615387;is=s");
        }
        zfnUser1: WirelessNode {
            @display("p=1025.3846,46.153847;is=vs");
        }
        zfnUser2: WirelessNode {
            @display("p=1016.9231,75.38462;is=vs");
        }
        zfnUser3: WirelessNode {
            @display("p=954.6154,63.846157;is=vs");
        }

    connections allowunconnected:

		//Internet
        zfnRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=red,4"); } <--> Internet.wiredInOut[0];
        zfnAP.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;

        //S0 - section
        S0CoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        S0Management.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> S0Router.wiredInOut++;
        S0CoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps; } <--> S0ContentServer.wiredInOut++;
        S0Router.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> S0CoreRouter.wiredInOut++;
        S0AP1.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> S0CoreRouter.wiredInOut++;
        S0AP2.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> S0CoreRouter.wiredInOut++;


        //O - section
        OCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        OManagement.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> ORouter.wiredInOut++;
        OCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps; } <--> OContentServer.wiredInOut++;
        ORouter.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> OCoreRouter.wiredInOut++;
        OAP1.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> OCoreRouter.wiredInOut++;
        OAP2.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> OCoreRouter.wiredInOut++;

}

