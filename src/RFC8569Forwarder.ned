//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// A CCN forwarder implementing the RFC 8569 using the
// IForwarder interface.
//
//
// TODO auto-generated module
//
simple RFC8569Forwarder like IForwarder
{
    parameters:

        // size of cache in bytes - zero means unlimited caching
        int maximumContentStoreSize @unit(byte) = default(0byte);

        // statistic signals
        @signal[fwdTotalInterestsBytesReceived](type=long);
        @signal[fwdTotalInterestRtnsBytesReceived](type=long);
        @signal[fwdTotalContentObjsBytesReceived](type=long);
        @signal[fwdTotalInterestsBytesSent](type=long);
        @signal[fwdTotalInterestRtnsBytesSent](type=long);
        @signal[fwdTotalContentObjsBytesSent](type=long);
        @signal[fwdCacheSizeBytes](type=long);
        @signal[fwdCacheAdditionsBytes](type=long);
        @signal[fwdCacheRemovalsBytes](type=long);
        @signal[fwdFIBEntryCount](type=long);
        @signal[fwdPITEntryCount](type=long);
        @signal[fwdCacheHitRatio](type=double);
        @signal[fwdCacheMissRatio](type=double);
        @signal[fwdNetworkCacheHitRatio](type=double);
        @signal[fwdNetworkCacheMissRatio](type=double);

        // display icon
        @display("i=block/fork");

    gates:
        inout appInOut[];
        inout transportInOut[];
}
