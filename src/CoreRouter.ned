//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A CCN root router that routes between content providers
// and propagates prefixes.
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
