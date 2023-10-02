//
// A simple IoT CCN network. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 18-feb-2023
//
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


network IoTNetworkNew
{
    parameters:

        // statistics
        @statistic[appNetworkInterestRetransmissionCount](source=appNetworkInterestRetransmissionCount; record=last,vector);

        @statistic[fwdNetworkCacheHitRatio](source=fwdNetworkCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdNetworkCacheMissRatio](source=fwdNetworkCacheMissRatio; record=min,max,count,last,vector);

        @statistic[fwdInterestToContentRatio](source=fwdInterestToContentRatio; record=min,max,count,last,vector);

        @statistic[fwdTotalTraffic](source=fwdTotalTraffic; record=min,max,count,last,vector,sum);
        @statistic[appContentFreshnessSignal](source=appContentFreshnessSignal; record=min,max,count,last,vector);

        @statistic[fwdTotalTrafficBytes](source=fwdTotalTrafficBytes; record=min,max,count,last,vector,sum);

        @display("bgb=1139.05,380.1375");

    submodules:
        eeRouter: AccessRouter {
            @display("p=63,172;is=s");
        }
        csAP: WirelessAccessRouter {
            @display("p=760.275,79.025;is=s");
        }
        frank: WiredNode {
            @display("p=40,123;is=s");
        }
        eeManagement: WiredNode {
            @display("p=22,172;is=s");
        }
        csHOD: WirelessNode {
            @display("p=761,33;is=s");
        }
        eeHOD: WirelessNode {
            @display("p=45,21;is=s");
        }
        eeCoreRouter: CoreRouter {
            @display("p=121,105;is=s");
        }
        csCoreRouter: CoreRouter {
            @display("p=895.1625,122.625;is=s");
        }
        eeContentServer: ContentServer {
            @display("p=123,172;is=s");
        }
        csContentServer: ContentServer {
            @display("p=896,160;is=s");
        }
        demiurge: Demiurge {
            @display("p=127,44;is=m");
        }
        eeIoTGW: IoTGateway {
            @display("p=56,262;is=s");
        }
        csIoTGW: IoTGateway {
            @display("p=961,59;is=s");
        }
        temperature1: WirelessSensorNode {
            @display("p=21,275;is=s");
        }
        humidity1: WirelessSensorNode {
            @display("p=35,224;is=s");
        }
        electricity1: WirelessSensorNode {
            @display("p=103,251;is=s");
        }
        water1: WirelessSensorNode {
            @display("p=67,302;is=s");
        }
        humidity2: WirelessSensorNode {
            @display("p=984,8;is=s");
        }
        temperature2: WirelessSensorNode {
            @display("p=939,19;is=s");
        }
        electricity2: WirelessSensorNode {
            @display("p=998,80;is=s");
        }
        water2: WirelessSensorNode {
            @display("p=1024,32;is=s");
        }
        eeAP: WirelessAccessRouter {
            @display("p=63,61;is=s");
        }
        internet: CCNInternet {
            @display("p=287.4875,59.95");
        }
        zfnRouter: CoreRouter {
            @display("p=499,160;is=s");
        }

        csRouter: AccessRouter {
            @display("p=958,123;is=s");
        }
        csManagement: WiredNode {
            @display("p=1025,123;is=s");
        }
        bioCoreRouter: CoreRouter {
            @display("p=688,225;is=s");
        }
        bioContentServer: ContentServer {
            @display("p=761,183;is=s");
        }
        bioIoTGW: IoTGateway {
            @display("p=926,267;is=s");
        }
        temperature3: WirelessSensorNode {
            @display("p=880,302;is=s");
        }
        water3: WirelessSensorNode {
            @display("p=976,308;is=s");
        }
        electricity3: WirelessSensorNode {
            @display("p=925,319;is=s");
        }
        bioRouter: AccessRouter {
            @display("p=625,263;is=s");
        }
        bioManagement: WiredNode {
            @display("p=625,320;is=s");
        }
        bioAP: WirelessAccessRouter {
            @display("p=695,275;is=s");
        }
        john: WirelessNode {
            @display("p=695,309;is=s");
        }
        humidity3: WirelessSensorNode {
            @display("p=976,262;is=s");
        }
        labIoTGW: IoTGateway {
            @display("p=388,319;is=s");
        }
        serverRoomIoTGW: IoTGateway {
            @display("p=489,59;is=s");
        }
        temperature5: WirelessSensorNode {
            @display("p=506,21;is=s");
        }
        labCoreRouter: CoreRouter {
            @display("p=314.7375,190.75;is=s");
        }
        labTechnician: WiredNode {
            @display("p=258.875,250.7;is=s");
        }
        systemAdmin: WiredNode {
            @display("p=598.1375,44.9625;is=s");
        }
        temperature4: WirelessSensorNode {
            @display("p=419,286;is=s");
        }
        humidity4: WirelessSensorNode {
            @display("p=354,291;is=s");
        }
        humidity5: WirelessSensorNode {
            @display("p=459,32;is=s");
        }
    connections allowunconnected:
        frank.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeRouter.wiredInOut++;
        eeManagement.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeRouter.wiredInOut++;
        eeRouter.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeCoreRouter.wiredInOut++;
        eeAP.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeCoreRouter.wiredInOut++;
        csAP.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csCoreRouter.wiredInOut++;
        bioAP.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> bioCoreRouter.wiredInOut++;
        eeCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps; } <--> eeContentServer.wiredInOut++;
        csCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1e6bps; } <--> csContentServer.wiredInOut++;
        bioCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps; } <--> bioContentServer.wiredInOut++;
        eeIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> eeRouter.wiredInOut++;
        csIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csCoreRouter.wiredInOut++;
        bioIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> bioCoreRouter.wiredInOut++;
        eeCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        csCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        bioCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        zfnRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=red,4"); } <--> internet.wiredInOut[0];
        csManagement.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csRouter.wiredInOut++;
        bioManagement.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> bioRouter.wiredInOut++;
        csRouter.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> csCoreRouter.wiredInOut++;
        bioRouter.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> bioCoreRouter.wiredInOut++;

        serverRoomIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        labCoreRouter.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
        labIoTGW.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> labCoreRouter.wiredInOut++;
        labTechnician.wiredInOut++ <--> DatarateChannel {  datarate = 100Mbps; } <--> labCoreRouter.wiredInOut++;
        systemAdmin.wiredInOut++ <--> DatarateChannel {  datarate = 1Gbps;@display("ls=black,3"); } <--> zfnRouter.wiredInOut++;
}

