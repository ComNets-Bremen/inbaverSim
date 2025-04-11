//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Simulates the CCN-based Internet as a single node. Provides a high-capacity
// network backbone with configurable wired and wireless transports to connect
// various network segments. Can host applications to emulate multiple Internet
// nodes.
//
// The model provides a highly abstracted representation of the global CCN
// Internet, focusing on its role as a content source and sink rather than
// modeling its internal structure in detail. The complex topology, routing
// infrastructure, and billions of nodes in the real Internet are simplified into
// a single node. Delay and bandwidth characteristics are approximated through
// basic parameters rather than modeling actual geographic distances and network
// congestion.
//
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
