//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Central backbone router in the CCN network that interconnects different
// network segments. Handles high-volume traffic between content providers and
// consumers while propagating content name prefixes throughout the network to
// maintain routing information.
//
// The model implements the CCN-specific routing functions in detail, including
// prefix propagation and forwarding based on longest prefix matching. However,
// it abstracts traditional backbone routing protocols like BGP and OSPF.
// Hardware aspects of core routers such as switching fabric architecture,
// line card processing, and quality of service mechanisms are not explicitly
// modeled. High-availability features like redundant components and failover
// mechanisms are also not implemented.
//
module CoreRouter extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int coreRouterNumWiredTransports = default(0);
        int coreRouterNumWirelessTransports = default(0);

        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

        // Ethernet transport parameters
        double ethDataRate @unit(bps) = default(1e6bps);
        double ethPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set transports
        numWiredTransports = coreRouterNumWiredTransports;
        numWirelessTransports = coreRouterNumWirelessTransports;

        // set wired transports
        wired[*].typename = "WiredTransport";
        wired[*].dataRate = ethDataRate;
        wired[*].packetErrorRate = ethPacketErrorRate;
        wired[*].headerSize = 32byte;

        // set mobility used
        mob.typename = mobility;

        @display("i=device/router;bgb=418,317");

    submodules:



}
