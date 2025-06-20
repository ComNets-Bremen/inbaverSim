//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// End-user device that connects to the CCN network through a wired Ethernet
// interface. Typically hosts content download applications and communicates with
// the network through access routers. Configured with appropriate data rates and
// error parameters for wired connections.
//
// The model provides detailed configuration of the basic connectivity parameters
// of wired end devices. However, it abstracts the actual Ethernet protocol
// stack, including MAC addressing, frame formats, and link-level procedures.
// TCP/IP protocols are not modeled as CCN operates directly over the link layer.
// Physical aspects like cabling types and distances are simplified to basic data
// rate and error rate parameters.
//
module WiredNode extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int wiredNodeNumWiredTransports = default(1);
        int wiredNodeNumWirelessTransports = default(0);

        // forwarder used in node
        string forwarder = default("RFC8569WithRFForwarder");

        // Ethernet transport parameters
        double ethDataRate @unit(bps) = default(1e6bps);
        double ethPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set transports
        numWiredTransports = wiredNodeNumWiredTransports;
        numWirelessTransports = wiredNodeNumWirelessTransports;

        // set second interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = ethDataRate;
        wired[0].packetErrorRate = ethPacketErrorRate;
        wired[0].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=device/pc2;bgb=418,317");

    submodules:


}
