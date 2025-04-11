//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Transport component that handles the transmission of CCN messages over wired
// connections such as Ethernet or fiber optic links. Simulates link
// characteristics including data rate, packet error rate, and header overhead.
// Collects statistics on transmitted and received data volumes.
//
// The model abstracts the actual wired protocols (Ethernet, TCP/IP) into a
// simplified model that focuses on basic transmission properties rather than
// protocol-specific details. Link-level framing, error detection, and flow
// control mechanisms are not explicitly modeled but approximated through
// configurable parameters.
//
simple WiredTransport like ITransport
{
    parameters:

        // wired technology -> Ethernet, Fibre Optic
        string wiredTechnology = default("Ethernet");

        // bandwidth in bits per second
        double dataRate @unit(bps) = default(1e6bps);

        // packet error rate
        double packetErrorRate @unit(pct) = default(0.0pct);

        // header size
        int headerSize @unit(byte) = default(32byte);

        // statistic signals
        @signal[transportTotalWiredBytesReceived](type=long);
        @signal[transportTotalWiredBytesSent](type=long);

        // display icon
        @display("i=block/transport");

    gates:
        inout forwarderInOut;
        inout physicalInOut;
        input radioIn @directIn;

}
