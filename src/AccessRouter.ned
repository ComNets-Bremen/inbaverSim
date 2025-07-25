//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Wired access router that connects wired CCN nodes to the network. Functions as an
// intermediary between end nodes and the core network, forwarding CCN messages
// between connected devices.
//
// The model provides detailed configuration of the basic connectivity parameters
// of wired routers. However, it abstracts traditional IP routing protocols and
// functions, focusing instead on CCN's name-based forwarding. Ethernet switching
// functions like MAC learning and spanning tree protocols are not modeled.
// Physical aspects of routing hardware like processing delays and buffer sizes
// are simplified to basic data rate parameters.
//
module AccessRouter extends Node
{
    parameters:
        @networkNode();

        // number of transports
        int accessRouterNumWiredTransports = default(0);
        int accessRouterNumWirelessTransports = default(0);

        // forwarder used in node
        string forwarder = default("RFC8569WithRFForwarder");

        // Ethernet transport parameters
        double ethDataRate @unit(bps) = default(1e6bps);
        double ethPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set transports used in node
        numWiredTransports = accessRouterNumWiredTransports;
        numWirelessTransports = accessRouterNumWirelessTransports;

        // set second interface (Wired)
        wired[*].typename = "WiredTransport";
        wired[*].dataRate = ethDataRate;
        wired[*].packetErrorRate = ethPacketErrorRate;
        wired[*].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=device/switch;bgb=418,317");

}
