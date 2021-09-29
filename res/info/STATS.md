# Statistics

inbaverSim collected statistics during simulations. Collected statistics
maybe categorized into the following types.

1. Network level - network wide statistics
2. Node level - node wide statistics
3. Scalar - measures of dispersion type statistics such as mean, last value, min, max, etc. (computed at the end of a simulation)
4. Vector - statistic values collected when they are written (continuously collected until simulation end) 
5. Protocol layer - collected at each protocol layer

Information about each statistic is listed below.


## Network-level Application-layer Statistics

- **appNetworkInterestRetransmissionCount** - The number of Interests retransmitted network-wide by applications
- **appNetworkInterestInjectedCount** - The number of Interest injections network-wide by applications


## Network-level Forwarding-layer Statistics

- **fwdNetworkCacheHitRatio** - The network-wide cache hit ratio (hits/(hits+misses))
- **fwdNetworkCacheMissRatio** - The network-wide cache miss ratio (misses/(hits+misses))


## Network-level Transport-layer Statistics



## Node-level Application-layer Statistics


- **appContentDownloadDuration** - Content download times for each complete content time difference between issue of 1st Interest to receipt of last Content Object)
- **appSegmentDownloadDuration** - Segment download times for each segment (time difference between issue of Interest to receipt of Content Object)
- **appTotalInterestsBytesSent** - Total bytes in all sent Interests
- **appRetransmissionInterestsBytesSent** - Total bytes in all retransmission Interests
- **appTotalContentObjsBytesReceived** - Total bytes in all received Content Objects
- **appTotalDataBytesReceived** - Total bytes received in all payload part of Content Objects
- **appTotalInterestsBytesReceived** - Total bytes in all received Interests
- **appTotalContentObjsBytesSent** - Total bytes in all sent Content Objects
- **appTotalDataBytesSent** - Total bytes sent in all payload part of Content Objects


## Node-level Forwarding-layer Statistics

- **fwdTotalInterestsBytesReceived** - Total bytes received as Interests
- **fwdTotalInterestRtnsBytesReceived** - Total bytes received as Interest Returns
- **fwdTotalContentObjsBytesReceived** - Total bytes received as Content Objects
- **fwdTotalInterestsBytesSent** - Total bytes sent as Interests
- **fwdTotalInterestRtnsBytesSent** - Total bytes sent as Interest Returns
- **fwdTotalContentObjsBytesSent** - Total bytes sent as Content Objects
- **fwdCacheSizeBytes** - Cache size in bytes
- **fwdCacheAdditionsBytes** - Additions to cache in bytes
- **fwdCacheRemovalsBytes** - Removals of cache entries in bytes
- **fwdFIBEntryCount** - Size of FIB in entries
- **fwdPITEntryCount** - Size of PIT in entries
- **fwdCacheHitRatio** - Cache hit ratio (for each node)
- **fwdCacheMissRatio** - Cache miss ratio (for each node)




## Node-level Transport-layer Statistics

- **transportTotalWiredBytesReceived** - Total bytes received by wired transports
- **transportTotalWiredBytesSent** - Total bytes sent by wired transports
- **transportTotalWirlessdBytesReceived** - Total bytes received by wireless transports
- **transportTotalWirelessBytesSent** - Total bytes sent by wireless transports




