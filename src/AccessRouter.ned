//
// A CCN wired access router to connect wired CCN 
// nodes. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

module AccessRouter extends Node
{
    parameters:
        @networkNode();

        numWiredTransports = 10;
        numWirelessTransports = 0;
        fwd.typename = "RFC8569Forwarder";

        // set second interface (Wired)
        wired[*].typename = "WiredTransport";
        wired[*].dataRate = 1e6bps;
        wired[*].packetErrorRate = 0.0pct;
        wired[*].headerSize = 32byte;

        // set mobility default
        mobility.typename = "StationaryMobility";

        @display("i=device/switch;bgb=418,317");

}
