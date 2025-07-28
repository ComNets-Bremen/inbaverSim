//
// Created by Finke, Tarek & Wieker, Lars as part of the Network Simulation Project
// Extension to RFC8569Forwarder by Asanga Kudugama from 2021
//
// Adds the traceroute capability specified in RFC9507

package inbaversim;

//
// TODO auto-generated module
//
simple RFC8569WithPingForwarder like IForwarder

{
    parameters:

        // size of cache in bytes - zero means unlimited caching
        int maximumContentStoreSize @unit(byte) = default(0byte);

        // statistic signals
        @signal[fwdTotalInterestsBytesReceived](type=long);
        @signal[fwdTotalInterestRtnsBytesReceived](type=long);
        @signal[fwdTotalContentObjsBytesReceived](type=long);
        @signal[fwdTotalTracerouteRqstBytesReceived](type=long);
        @signal[fwdTotalTracerouteRplBytesReceived](type=long);
        @signal[fwdTotalInterestsBytesSent](type=long);
        @signal[fwdTotalInterestRtnsBytesSent](type=long);
        @signal[fwdTotalContentObjsBytesSent](type=long);
        @signal[fwdTotalTracerouteRqstBytesSent](type=long);
        @signal[fwdTotalTracerouteRplBytesSent](type=long);
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
