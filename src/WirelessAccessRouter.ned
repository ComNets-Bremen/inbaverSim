//
// A CCN wireless access router to connect WLAN based 
// CCN nodes. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

module WirelessAccessRouter extends Node
{
    parameters:
        @networkNode();

        numWiredTransports = 1;
        numWirelessTransports = 1;
        fwd.typename = "RFC8569Forwarder";

        // set first interface (WLAN)
        wireless[0].typename = "WirelessTransport";
        wireless[0].wirelessTechnology = "WLAN";
        wireless[0].operationMode = "ap";
        wireless[0].connectString = "ccn";
        wireless[0].wirelessRange = 10m;
        wireless[0].dataRate = 6e6bps;
        wireless[0].packetErrorRate = 0.0pct;
        wireless[0].scanInterval = 1s;
        wireless[0].headerSize = 32byte;

        // set second interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = 1e6bps;
        wired[0].packetErrorRate = 0.0pct;
        wired[0].headerSize = 32byte;

        // set mobility default
        mobility.typename = "StationaryMobility";

        @display("i=device/accesspoint;bgb=418,317");

    submodules:

}
