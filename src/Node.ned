//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

import inet.mobility.contract.IMobility;

//
// A CCN node containing the CCN protocol stack.
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
        @statistic[appTotalDataBytesReceived](source=appTotalDataBytesReceived; record=count,mean,sum,vector);

        @statistic[appTotalInterestsBytesReceived](source=appTotalInterestsBytesReceived; record=count,mean,sum,vector);
        @statistic[appTotalContentObjsBytesSent](source=appTotalContentObjsBytesSent; record=count,mean,sum,vector);
        @statistic[appTotalDataBytesSent](source=appTotalDataBytesSent; record=count,mean,sum,vector);

        @statistic[fwdTotalInterestsBytesReceived](source=fwdTotalInterestsBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalInterestRtnsBytesReceived](source=fwdTotalInterestRtnsBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalContentObjsBytesReceived](source=fwdTotalContentObjsBytesReceived; record=count,mean,sum,vector);
        @statistic[fwdTotalInterestsBytesSent](source=fwdTotalInterestsBytesSent; record=count,mean,sum,vector);
        @statistic[fwdTotalInterestRtnsBytesSent](source=fwdTotalInterestRtnsBytesSent; record=count,mean,sum,vector);
        @statistic[fwdTotalContentObjsBytesSent](source=fwdTotalContentObjsBytesSent; record=count,mean,sum,vector);
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

        @display("bgb=423,301");

    gates:
        inout wiredInOut[numWiredTransports];

    submodules:
        numen: Numen {
            @display("p=198,22;t=Numen,l");
        }
        app[numApps]: <> like IApplication {
            @display("p=198,89");
        }
        fwd: <> like IForwarder {
            @display("p=198,169");
        }
        wired[numWiredTransports]: <> like ITransport {
            @display("p=96,246;t=WiredTransport");
        }
        wireless[numWirelessTransports]: <> like ITransport {
            @display("p=309,246");
        }
        mob: <> like IMobility {
            @display("p=299,169;t=Mobility,l");
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
