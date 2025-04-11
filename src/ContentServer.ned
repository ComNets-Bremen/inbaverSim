//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Dedicated server node that hosts content-providing applications in the CCN
// network. Primarily connects via wired transports to deliver content in
// response to Interest messages. Designed to efficiently serve as a content
// repository within the network.
//
// The model implements the CCN-specific content serving functions in detail,
// including name-based content retrieval and segmentation. However, it abstracts
// server hardware aspects like processing power, memory architecture, and
// storage systems. Content management functions such as replication, load
// balancing, and content lifecycle management are not explicitly modeled.
// Security aspects like access control and content authentication are
// simplified.
//
module ContentServer extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int contentServerNumWiredTransports = default(1);
        int contentServerNumWirelessTransports = default(0);

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
        numWiredTransports = contentServerNumWiredTransports;
        numWirelessTransports = contentServerNumWirelessTransports;

        // set second interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = ethDataRate;
        wired[0].packetErrorRate = ethPacketErrorRate;
        wired[0].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=device/server;bgb=418,317");

    submodules:



}
