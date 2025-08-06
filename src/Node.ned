//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

import inet.mobility.contract.IMobility;

//
// Base module for all CCN network nodes, implementing the complete CCN protocol
// stack. Provides a modular architecture with configurable application,
// forwarding, and transport layers. Collects comprehensive statistics for
// performance analysis of CCN operations.
//
// The model implements the core CCN protocol stack architecture in detail,
// including the layered design and interfaces between components. However, it
// abstracts operating system aspects like process scheduling, memory
// management, and device drivers. Hardware aspects such as CPU architecture,
// bus systems, and physical interfaces are not modeled. The focus is on the
// network protocol functionality rather than the underlying platform that would
// host it in a real system.
//
module Node
{
    parameters:
        @networkNode();

        // application configuration
        int numApps = default(0);

        // transport configuration
        int numWiredTransports = default(0);
        int numWirelessTransports = default(0);

        // statistics
        @statistic[appContentDownloadDuration](source=appContentDownloadDuration; record=min,max,count,mean,vector);
        @statistic[appSegmentDownloadDuration](source=appSegmentDownloadDuration; record=min,max,count,mean,vector);
        @statistic[appTotalInterestsBytesSent](source=appTotalInterestsBytesSent; record=count,mean,sum,vector);
        @statistic[appRetransmissionInterestsBytesSent](source=appRetransmissionInterestsBytesSent; record=count,mean,sum,vector);
        @statistic[appTotalContentObjsBytesReceived](source=appTotalContentObjsBytesReceived; record=count,mean,sum,vector);
        @statistic[appTotalTracerouteRqstsBytesSent](source=appTotalTracerouteRqstsBytesSent; record=count,mean,sum,vector);
        @statistic[appTotalTracerouteRplsBytesReceived](source=appTotalTracerouteRplsBytesReceived; record=count,mean,sum,vector);
        @statistic[appTotalDataBytesReceived](source=appTotalDataBytesReceived; record=count,mean,sum,vector);

        @statistic[appTotalInterestsBytesReceived](source=appTotalInterestsBytesReceived; record=count,mean,sum,vector);
        @statistic[appTotalContentObjsBytesSent](source=appTotalContentObjsBytesSent; record=count,mean,sum,vector);
        @statistic[appTotalTracerouteRqstsBytesReceived](source=appTotalTracerouteRqstsBytesReceived; record=count,mean,sum,vector);
        @statistic[appTotalTracerouteRplsBytesSent](source=appTotalTracerouteRplsBytesSent; record=count,mean,sum,vector);
        @statistic[appTotalDataBytesSent](source=appTotalDataBytesSent; record=count,mean,sum,vector);

        @statistic[fwdTotalInterestsBytesReceived](source=fwdTotalInterestsBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalInterestRtnsBytesReceived](source=fwdTotalInterestRtnsBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalContentObjsBytesReceived](source=fwdTotalContentObjsBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalTracerouteRqstBytesReceived](source=fwdTotalTracerouteRqstBytesReceived; record=count,mean,sum,vector);
        @statsitic[fwdTotalTracerouteRplBytesReceived](source=fwdTotalTracerouteRplBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalInterestsBytesSent](source=fwdTotalInterestsBytesSent; record=count,mean,sum,vector);
        @statistic[fwdTotalInterestRtnsBytesSent](source=fwdTotalInterestRtnsBytesSent; record=count,mean,sum,vector);
        @statistic[fwdTotalContentObjsBytesSent](source=fwdTotalContentObjsBytesSent; record=count,mean,sum,vector);
        @statistic[fwdTotalTracerouteRqstBytesSent](source=fwdTotalTracerouteRqstBytesSent; record=count,mean,sum,vector);
        @statistic[fwdTotalTracerouteRplBytesSent](source=fwdTotalTracerouteRplBytesSent; record=count,mean,sum,vector);
        @statistic[fwdCacheSizeBytes](source=fwdCacheSizeBytes; record=min,max,count,mean,timeavg,last,vector);
        @statistic[fwdCacheAdditionsBytes](source=fwdCacheAdditionsBytes; record=count,mean,sum,vector);
        @statistic[fwdCacheRemovalsBytes](source=fwdCacheRemovalsBytes; record=count,mean,sum,vector);
        @statistic[fwdFIBEntryCount](source=fwdFIBEntryCount; record=min,max,count,mean,timeavg,last,vector);
        @statistic[fwdPITEntryCount](source=fwdPITEntryCount; record=min,max,count,mean,timeavg,last,vector);
        @statistic[fwdCacheHitRatio](source=fwdCacheHitRatio; record=min,max,count,last,vector);
        @statistic[fwdCacheMissRatio](source=fwdCacheMissRatio; record=min,max,count,last,vector);

        @statistic[transportTotalWiredBytesReceived](source=transportTotalWiredBytesReceived; record=count,mean,sum,vector);
        @statistic[transportTotalWiredBytesSent](source=transportTotalWiredBytesSent; record=count,mean,sum,vector);

        @statistic[transportTotalWirelessBytesReceived](source=transportTotalWirelessBytesReceived; record=count,mean,sum,vector);
        @statistic[transportTotalWirelessBytesSent](source=transportTotalWirelessBytesSent; record=count,mean,sum,vector);

        @display("bgb=392,289");

    gates:
        inout wiredInOut[numWiredTransports];

    submodules:
        numen: Numen {
            @display("p=43,39;t=Numen,t");
        }
        app[numApps]: <> like IApplication {
            @display("p=185,39;t=Applications,t");
        }
        fwd: <> like IForwarder {
            @display("p=185,145;t=Forwarder,t");
        }
        wired[numWiredTransports]: <> like ITransport {
            @display("p=91,240;t=WiredTransports,t");
        }
        wireless[numWirelessTransports]: <> like ITransport {
            @display("p=287,240;t=WirelessTransports,t");
        }
        mob: <> like IMobility {
            @display("p=326,39;t=Mobility,t");
        }

    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].forwarderInOut <--> fwd.appInOut++;
        }

        for i=0..numWirelessTransports-1 {
            wireless[i].forwarderInOut <--> fwd.transportInOut++;
        }

        for i=0..numWiredTransports-1 {
            wired[i].forwarderInOut <--> fwd.transportInOut++;
            wired[i].physicalInOut <--> wiredInOut[i];
        }
}
