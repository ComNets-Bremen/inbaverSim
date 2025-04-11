//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A CCN content server hosting applications.
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
