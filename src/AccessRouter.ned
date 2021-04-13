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

        // transports used in node
        numWiredTransports = 10;
        numWirelessTransports = 0;

        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

        // Ethernet transport parameters
        double ethDataRate @unit(bps) = default(1e6bps);
        double ethPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set second interface (Wired)
        wired[*].typename = "WiredTransport";
        wired[*].dataRate = ethDataRate;
        wired[*].packetErrorRate = ethPacketErrorRate;
        wired[*].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=device/switch;bgb=418,317");

}
