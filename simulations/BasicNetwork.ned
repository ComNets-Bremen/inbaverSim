//
// A basic CCN network for testing. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim.simulations;

import inbaversim.ContentServer;
import inbaversim.Demiurge;
import inbaversim.WirelessAccessRouter;
import inbaversim.WirelessNode;
import ned.DatarateChannel;

network BasicNetwork
{
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
