//
// A node that acts as the CCN Internet. Applications can be
// deployed to simulate the nodes in the Internt. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 23-feb-2023
//
//

package inbaversim;

module CCNInternet extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int internetNumWiredTransports = default(0);
        int internetNumWirelessTransports = default(0);
        
        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

        // Ethernet transport parameters
        double ethDataRate @unit(bps) = default(1Gbps);
        double ethPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set transports
        numWiredTransports = internetNumWiredTransports;
        numWirelessTransports = internetNumWirelessTransports;

        // set wired transports
        wired[*].typename = "WiredTransport";
        wired[*].dataRate = ethDataRate;
        wired[*].packetErrorRate = ethPacketErrorRate;
        wired[*].headerSize = 32byte;

        // set mobility used
        mob.typename = mobility;

        @display("i=misc/cloud;bgb=418,317");

    submodules:



}
