//
// A CCN content server hosting applications. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

module ContentServer extends Node
{
    parameters:
        @networkNode();

        numWiredTransports = 1;
        numWirelessTransports = 0;
        fwd.typename = "RFC8569Forwarder";

        // set second interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = 1e6bps;
        wired[0].packetErrorRate = 0.0pct;
        wired[0].headerSize = 32byte;

        // set mobility default
        mobility.typename = "StationaryMobility";

        @display("i=device/server;bgb=418,317");

    submodules:



}
