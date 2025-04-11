//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A file hosting (content server) application that
// implements the IApplication interface.
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
        @signal[appTotalDataBytesSent](type=long);


        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
