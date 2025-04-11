//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A file download client (content requester) application that
// implements the IApplication interface.
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
