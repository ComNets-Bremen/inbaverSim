//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Server application that hosts and provides content in the CCN network.
// Responds to Interest messages by sending Content Objects containing the
// requested data. Supports configurable content parameters including segment
// size, number of segments per file, and caching time.
//
// The model implements the core CCN content serving mechanisms in detail,
// including content segmentation and caching directives. However, it abstracts
// the actual content generation process, using random data rather than real
// files. Server-side processing delays, content popularity distributions, and
// storage constraints are simplified or not modeled explicitly.
//
simple ContentHostApp like IApplication
{
    parameters:
        // hosted data details
        string hostedPrefixNames = default("ccnx://spacetime.com/;ccnx://relativity.com/;ccnx://universe.com/");
        string dataNamePrefix = default("file");
        volatile int segmentSize @unit(byte) = default(10000byte);
        volatile int numSegmentsPerFile = default(intuniform(5, 100));
        volatile double cacheTime @unit(s) = default(3600s); // 0 means infinite 

        // statistic signals
        @signal[appTotalInterestsBytesReceived](type=long);
        @signal[appTotalContentObjsBytesSent](type=long);
        @signal[appTotalTracerouteRqstsBytesReceived](type=long);
        @signal[appTotalTracerouteRplsBytesSent](type=long);
        @signal[appTotalDataBytesSent](type=long);


        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
