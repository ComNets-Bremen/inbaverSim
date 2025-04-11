//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A wired transport that implements the ITransport
// interface.
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
