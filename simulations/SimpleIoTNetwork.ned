//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim.simulations;

import inbaversim.AccessRouter;
import inbaversim.CCNInternet;
import inbaversim.ContentServer;
import inbaversim.CoreRouter;
import inbaversim.Demiurge;
import inbaversim.WiredNode;
import inbaversim.WirelessAccessRouter;
import inbaversim.WirelessNode;
import inbaversim.IoTGateway;
import inbaversim.WirelessSensorNode;
import ned.DatarateChannel;


//
// A simple IoT CCN network.
//
network SimpleIoTNetwork
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);

        @display("bgb=727,333");

    submodules:
        eeRouter: AccessRouter {
            @display("p=69,148;is=s");
        }
        csAP: WirelessAccessRouter {
            @display("p=490,155;is=s");
        }
        frank: WiredNode {
            @display("p=34,216;is=s");
        }
        eeManagement: WiredNode {
            @display("p=69,256;is=s");
        }
        daniel: WirelessNode {
            @display("p=490,108;is=s");
        }
        silvia: WirelessNode {
            @display("p=446,135;is=s");
        }
        peter: WirelessNode {
            @display("p=76,55;is=s");
        }
        eeCoreRouter: CoreRouter {
            @display("p=194,156;is=s");
        }
        csCoreRouter: CoreRouter {
            @display("p=468,216;is=s");
        }
        eeContentServer: ContentServer {
            @display("p=245,98;is=s");
        }
        csContentServer: ContentServer {
            @display("p=532,270;is=s");
        }
        demiurge: Demiurge {
            @display("p=226,27;is=m");
        }
        eeIoTGW: IoTGateway {
            @display("p=163,224;is=s");
        }
        csIoTGW: IoTGateway {
            @display("p=591,147;is=s");
        }
        temperature1: WirelessSensorNode {
            @display("p=152,279;is=s");
        }
        humidity1: WirelessSensorNode {
            @display("p=226,248;is=s");
        }
        humidity2: WirelessSensorNode {
            @display("p=633,71;is=s");
        }
        temperature2: WirelessSensorNode {
            @display("p=573,97;is=s");
        }
        eeAP: WirelessAccessRouter {
            @display("p=107,97;is=s");
        }
        internet: CCNInternet {
            @display("p=436,47;is=vl");
        }
        zfnRouter: CoreRouter {
            @display("p=332,193;is=s");
        }

        csRouter: AccessRouter {
            @display("p=618,205;is=s");
        }
        csManagement: WiredNode {
            @display("p=657,262;is=s");
        }
    connections allowunconnected:
        frank.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeRouter.wiredInOut++;
        eeManagement.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeRouter.wiredInOut++;
        eeRouter.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeCoreRouter.wiredInOut++;
        eeAP.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeCoreRouter.wiredInOut++;
        csAP.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csCoreRouter.wiredInOut++;
        eeCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps; } <--> eeContentServer.wiredInOut++;
        csCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> csContentServer.wiredInOut++;
        eeIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeRouter.wiredInOut++;
        csIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csCoreRouter.wiredInOut++;
        eeCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        csCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        zfnRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=red,4"); } <--> internet.wiredInOut++;
        csManagement.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csRouter.wiredInOut++;
        csRouter.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csCoreRouter.wiredInOut++;

}
