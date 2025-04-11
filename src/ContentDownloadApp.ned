//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Client application that periodically requests and downloads content from the
// CCN network. Sends Interest messages for specific named content and processes
// received Content Objects. Supports configurable download intervals and
// interest retransmission for reliability.
//
// The model implements the core CCN content retrieval mechanisms in detail,
// including interest retransmission and segment-by-segment content download.
// However, it abstracts user behavior patterns to simple statistical
// distributions. Content selection is randomized rather than based on realistic
// popularity models, and application-level protocols (HTTP, FTP, etc.) are not
// explicitly modeled.
//
simple ContentDownloadApp like IApplication
{
    parameters:

        // time between content downloads
        volatile double interContentDownloadInterval @unit(s) = default(uniform(200s,500s));

        // requested prefixs
        string requestedPrefixNames = default("ccnx://universe.com/;ccnx://spacetime.com/");

        // file name prefix (a number is suffixed, based on nextFileSuffix)
        string dataNamePrefix = default("file");

        // maximum files stored at requested prefix
        volatile int nextFileSuffix = default(intuniform(0, 9999));

        // maximum hops allowed for Interest to travel
        int maxHopsAllowed = default(16);

        // time after which an interest is retransmitted
        double interestRetransmitTimeout @unit(s) = default(5s);

        // statistic signals
        @signal[appContentDownloadDuration](type=simtime_t);
        @signal[appSegmentDownloadDuration](type=simtime_t);
        @signal[appTotalInterestsBytesSent](type=long);
        @signal[appRetransmissionInterestsBytesSent](type=long);
        @signal[appTotalContentObjsBytesReceived](type=long);
        @signal[appTotalDataBytesReceived](type=long);
        @signal[appNetworkInterestRetransmissionCount](type=long);
        @signal[appNetworkInterestInjectedCount](type=long);

        // display icon
        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
