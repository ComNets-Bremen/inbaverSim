//
// A CCN node containing the CCN protocol stack. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

import inet.mobility.contract.IMobility;

module Node
{
    parameters:
        @networkNode();

        // application configuration
        int numApps = default(0);

        // transport configuration
        int numWiredTransports = default(0);
        int numWirelessTransports = default(0);

        @display("bgb=423,301");

    gates:
        inout wiredInOut[numWiredTransports];

    submodules:
        numen: Numen {
            @display("p=198,22");
        }
        app[numApps]: <> like IApplication {
            @display("p=198,89");
        }
        fwd: <> like IForwarder {
            @display("p=198,169");
        }
        wired[numWiredTransports]: <> like ITransport {
            @display("p=96,246");
        }
        wireless[numWirelessTransports]: <> like ITransport {
            @display("p=309,246");
        }
        mobility: <> like IMobility {
            @display("p=299,169");
        }

    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].forwarderInOut <--> fwd.appInOut++;
        }

        for i=0..numWirelessTransports-1 {
            wireless[i].forwarderInOut <--> fwd.transportInOut++;
        }

        for i=0..numWiredTransports-1 {
            wired[i].forwarderInOut <--> fwd.transportInOut++;
            wired[i].physicalInOut <--> wiredInOut[i];
        }
}
