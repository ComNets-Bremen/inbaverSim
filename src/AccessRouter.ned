//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A CCN wired access router to connect wired CCN
// nodes.
//
module AccessRouter extends Node
{
    parameters:
        @networkNode();

        // number of transports
        int accessRouterNumWiredTransports = default(0);
        int accessRouterNumWirelessTransports = default(0);

        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

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
