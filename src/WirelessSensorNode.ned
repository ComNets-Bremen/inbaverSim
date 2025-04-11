//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

import inbaversim.Node;
import inbaversim.WirelessTransport;


//
// A wireless sensor node that communicates over CCN. It has one
// LoRa wireless network interface.
//
module WirelessSensorNode extends Node
{
    parameters:
        @networkNode();

        // transports used in node
        int wirelessSensorNodeLoRaNumWirelessTransports = default(1);

        // forwarder used in node
        string forwarder = default("RFC8569Forwarder");

        // LoRa transport parameters
        //   source: https://www.techplayon.com/lora-long-range-network-architecture-protocol-architecture-and-frame-formats/
        //   how header size of 20 is computed for LoRa
        //     preamble 8 bytes
        //     header+CRC 20 bits (taken as 3 bytes)
        //     MAC header 1 byte
        //     frame header 7 bytes (no options, if with options can ho upto 22 bytes)
        //     frame port 1 byte
        //   how data rate is determined
        //     source: https://www.thethingsnetwork.org/docs/lorawan/regional-parameters/
        //     Channel Frequency (MHz) 868.10
        //     Bandwidth (kHz)         125
        //     LoRa data rate code     DR5
        //     Configuration (SF + BW) LoRa: SF7 / 125 kHz (DR5)
        //     Bit rate                5470 (~5.5kbps)
        double wirelessSensorNodeLoRaWirelessRange @unit(meter) = default(200m);
        double wirelessSensorNodeLoRaDataRate @unit(bps) = default(5.5kbps);
        double wirelessSensorNodeLoRaPacketErrorRate @unit(pct) = default(0.0pct);
        int wirelessSensorNodeLoRaHeaderSize @unit(byte) = default(20byte);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set transports
        numWiredTransports = 0;
        numWirelessTransports = wirelessSensorNodeLoRaNumWirelessTransports;

        // set first interface (LoRa)
        wireless[0].typename = "WirelessTransport";
        wireless[0].wirelessTechnology = "LoRa";
        wireless[0].operationMode = "client";
        wireless[0].connectString = "LoRa";
        wireless[0].wirelessRange = wirelessSensorNodeLoRaWirelessRange;
        wireless[0].dataRate = wirelessSensorNodeLoRaDataRate;
        wireless[0].packetErrorRate = wirelessSensorNodeLoRaPacketErrorRate;
        wireless[0].scanInterval = 1s;
        wireless[0].headerSize = wirelessSensorNodeLoRaHeaderSize;

        // set mobility default
        mob.typename = mobility;

        @display("i=misc/sensor3;bgb=418,317");

    submodules:



}
