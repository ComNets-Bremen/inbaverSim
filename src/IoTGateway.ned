//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Dual-interface gateway node that connects CCN-based IoT sensor networks to the
// wider CCN network. Features a LoRa wireless interface for communicating with
// sensor nodes and an Ethernet interface for connecting to the core network,
// enabling seamless data flow between these different network domains.
//
// The model provides detailed configuration of the physical connectivity aspects
// of IoT gateways, including wireless range, data rates, and protocol headers.
// However, it abstracts the actual LoRaWAN protocol stack into a simplified
// model. Gateway-specific functions like protocol translation between LoRaWAN
// and IP networks are not explicitly modeled, focusing instead on the CCN
// forwarding capabilities across different network domains.
//
package inbaversim;

module IoTGateway extends Node
{
    parameters:
        @networkNode();

        // number of transports
        int iotGatewayNumWiredTransports = default(1);
        int iotGatewayNumWirelessTransports = default(2);
        
        // is the gateway a pure wireless gateway or has also a wired 
        // interface in addition to the wireless interfaces
        bool pureWireless = default(false);
        
        // forwarder used in node
        string forwarder = default("RFC8569WithRFForwarder");

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
        
        // WLAN transport parameters
        string wlanConnectString = default("ccn");
        double wlanWirelessRange @unit(meter) = default(10m);
        double wlanDataRate @unit(bps) = default(1e6bps);
        double wlanPacketErrorRate @unit(pct) = default(0.0pct);
        

        // mobility model settings
        string mobility = default("StationaryMobility");

        // set forwarder used
        fwd.typename = forwarder;

        // set tnum transports
        numWiredTransports = iotGatewayNumWiredTransports;
        numWirelessTransports = iotGatewayNumWirelessTransports;

        // set first interface (LoRa)
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
        wireless[0].wirelessRangeRadiusColor = "teal";
        
        // set second interface (WLAN)
        wireless[1].typename = "WirelessTransport";
        wireless[1].wirelessTechnology = "WLAN";
        wireless[1].operationMode = "client";
        wireless[1].connectString = wlanConnectString;
        wireless[1].wirelessRange = wlanWirelessRange;
        wireless[1].dataRate = wlanDataRate;
        wireless[1].packetErrorRate = wlanPacketErrorRate;
        wireless[1].scanInterval = 1s;
        wireless[1].headerSize = 32byte;


        // set third interface (Wired)
        wired[0].typename = "WiredTransport";
        wired[0].dataRate = iotGatewayEthDataRate;
        wired[0].packetErrorRate = iotGatewayEthPacketErrorRate;
        wired[0].headerSize = 32byte;
        wired[0].dummyTransport = (pureWireless ? true : false);

        // set mobility default
        mob.typename = mobility;

        @display("i=misc/sensorgateway;bgb=418,317");

    submodules:

}
