//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Forwarding component that implements the CCN protocol as specified in RFC 8569.
// Manages the Pending Interest Table (PIT), Forwarding Information Base (FIB),
// and Content Store (CS) to process Interest and Content Object messages
// according to the CCN protocol specification.
//
// The implementation models the core CCN forwarding mechanisms in detail, while
// abstracting some implementation-specific aspects. Cache replacement policies
// are simplified to FIFO (First-In-First-Out), and cache placement strategies
// use LCE (Leave Copy Everywhere). Security aspects mentioned in the RFC are
// not fully implemented, focusing instead on the basic forwarding functionality.
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
