//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//import inet.mobility.static.StationaryMobility;
//import inet.mobility.base.MobilityBase;

//
// A wireless CCN node with two network interfaces, one
// WLAN and the other Bluetooth.
//
module WirelessNode extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int wirelessNodeNumWiredTransports = default(0);
        int wirelessNodeNumWirelessTransports = default(2);

        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

        // WLAN transport parameters
        string wlanConnectString = default("ccn");
        double wlanWirelessRange @unit(meter) = default(10m);
        double wlanDataRate @unit(bps) = default(1e6bps);
        double wlanPacketErrorRate @unit(pct) = default(0.0pct);

        // Bluetooth transport settings
        string btConnectString = default("ccn");
        double btWirelessRange @unit(meter) = default(10m);
        double btDataRate @unit(bps) = default(1e6bps);
        double btPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set transports
        numWiredTransports = wirelessNodeNumWiredTransports;
        numWirelessTransports = wirelessNodeNumWirelessTransports;

        // set first interface (WLAN)
        wireless[0].typename = "WirelessTransport";
        wireless[0].wirelessTechnology = "WLAN";
        wireless[0].operationMode = "client";
        wireless[0].connectString = wlanConnectString;
        wireless[0].wirelessRange = wlanWirelessRange;
        wireless[0].dataRate = wlanDataRate;
        wireless[0].packetErrorRate = wlanPacketErrorRate;
        wireless[0].scanInterval = 1s;
        wireless[0].headerSize = 32byte;

        // set second interface (Bluetooth)
        wireless[1].typename = "WirelessTransport";
        wireless[1].wirelessTechnology = "Bluetooth";
        wireless[1].operationMode = "direct";
        wireless[1].connectString = btConnectString;
        wireless[1].wirelessRange = btWirelessRange;
        wireless[1].dataRate = btDataRate;
        wireless[1].packetErrorRate = btPacketErrorRate;
        wireless[1].scanInterval = 1s;
        wireless[1].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=device/palm2;bgb=418,317");

    submodules:



}
