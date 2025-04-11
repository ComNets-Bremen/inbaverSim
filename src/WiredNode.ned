//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A wired CCN node with one wired interface.
//
module WiredNode extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int wiredNodeNumWiredTransports = default(1);
        int wiredNodeNumWirelessTransports = default(0);

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
