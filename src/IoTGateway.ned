//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A CCN based IoT gatewway with 2 network interfaces - a
// LoRa interface facing a sensor network and an Ethernet
// interface facing the internet.
//
module IoTGateway extends Node
{
    parameters:
        @networkNode();

        // number of transports
        int iotGatewayNumWiredTransports = default(1);
        int iotGatewayNumWirelessTransports = default(1);

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
        double iotGatewayLoRaWirelessRange @unit(meter) = default(200m);
        double iotGatewayLoRaDataRate @unit(bps) = default(5.5kbps);
        double iotGatewayLoRaPacketErrorRate @unit(pct) = default(0.0pct);
        int iotGatewayLoRaHeaderSize @unit(byte) = default(20byte);

        // Ethernet transport parameters
        double iotGatewayEthDataRate @unit(bps) = default(1e6bps);
        double iotGatewayEthPacketErrorRate @unit(pct) = default(0.0pct);

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set tnum transports
        numWiredTransports = iotGatewayNumWiredTransports;
        numWirelessTransports = iotGatewayNumWirelessTransports;

        // set first interface (WLAN)
        wireless[0].typename = "WirelessTransport";
        wireless[0].wirelessTechnology = "LoRa";
        wireless[0].operationMode = "ap";
        wireless[0].connectString = "LoRa";
        wireless[0].wirelessRange = iotGatewayLoRaWirelessRange;
        wireless[0].dataRate = iotGatewayLoRaDataRate;
        wireless[0].packetErrorRate = iotGatewayLoRaPacketErrorRate;
        wireless[0].scanInterval = 1s;
        wireless[0].headerSize = iotGatewayLoRaHeaderSize;
        wireless[0].wirelessRangeRadiusShow = true;

        // set second interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = iotGatewayEthDataRate;
        wired[0].packetErrorRate = iotGatewayEthPacketErrorRate;
        wired[0].headerSize = 32byte;

        // set mobility default
        mob.typename = mobility;

        @display("i=misc/sensorgateway;bgb=418,317");

    submodules:

}
