//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A wireless transport that implements the ITransport
// interface.
//
simple WirelessTransport like ITransport
{
    parameters:

        // wireless technology -> WLAN, Bluetooth
        string wirelessTechnology = default("WLAN");

        // wireless mode of operation -> ap, client, direct
        string operationMode = default("client");

        // wireless connection string - for ap, client
        string connectString = default("ccn");

        // wireless range in meters
        double wirelessRange @unit(meter) = default(10m);

        // bandwidth in bits per second
        double dataRate @unit(bps) = default(1e6bps);

        // packet error rate
        double packetErrorRate @unit(pct) = default(0.0pct);

        // when direct, this is the interval of scanning neighbourhood
        // to send a list of neighbours to the forwarding layer
        double scanInterval @unit(s) = default(1s);

        // header size
        int headerSize @unit(byte) = default(32byte);

        // whether to show and the color of the circle drawn for the wireless range
        bool wirelessRangeRadiusShow = default(false);
        string wirelessRangeRadiusColor = default("black");

        // statistic signals
        @signal[transportTotalWirelessBytesReceived](type=long);
        @signal[transportTotalWirelessBytesSent](type=long);

        // display icon
        @display("i=block/transport");

    gates:
        inout forwarderInOut;
        inout physicalInOut;
        input radioIn @directIn;

}
