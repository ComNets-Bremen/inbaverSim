//
// A file download client (content requester) application that 
// implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

simple ContentDownloadApp like IApplication
{
    parameters:
        
        // time between content downloads
        double interContentDownloadInterval @unit(s) = default(300s);
        
        // requested prefixs
        string requestedPrefixNames = default("ccnx://universe.com/;ccnx://spacetime.com/");
        
        // file name prefix (a number is added at the end)
        string dataNamePrefix = default("file");
        
        // maximum hops allowed for Interest to travel
        int maxHopsAllowed = default(16);
        
        // time after which an interest is retransmitted
        double interestRetransmitTimeout @unit(s) = default(5s);
        
        // statistic signal: content download duration 
        @signal[appContentDownloadDuration](type=simtime_t);
                
        // display icon
        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
