//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A CCN wireless access router to connect WLAN based
// CCN nodes.
//
module WirelessAccessRouter extends Node
{
    parameters:
        @networkNode();

        // number of transports
        int wirelessAccessRouterNumWiredTransports = default(1);
        int wirelessAccessRouterNumWirelessTransports = default(1);

        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

        // WLAN transport parameters
        string wlanConnectString = default("ccn");
        double wlanWirelessRange @unit(meter) = default(10m);
        double wlanDataRate @unit(bps) = default(1e6bps);
        double wlanPacketErrorRate @unit(pct) = default(0.0pct);

        // Ethernet transport parameters
        double ethDataRate @unit(bps) = default(1e6bps);
        double ethPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set tnum transports
        numWiredTransports = wirelessAccessRouterNumWiredTransports;
        numWirelessTransports = wirelessAccessRouterNumWirelessTransports;

        // set first interface (WLAN)
        wireless[0].typename = "WirelessTransport";
        wireless[0].wirelessTechnology = "WLAN";
        wireless[0].operationMode = "ap";
        wireless[0].connectString = wlanConnectString;
        wireless[0].wirelessRange = wlanWirelessRange;
        wireless[0].dataRate = wlanDataRate;
        wireless[0].packetErrorRate = wlanPacketErrorRate;
        wireless[0].scanInterval = 1s;
        wireless[0].headerSize = 32byte;
        wireless[0].wirelessRangeRadiusShow = true;

        // set second interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = ethDataRate;
        wired[0].packetErrorRate = ethPacketErrorRate;
        wired[0].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=device/accesspoint;bgb=418,317");

    submodules:

}
