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
        // requested content details
        double contentRequestInterval @unit(s) = default(300s);
        string requestedPrefixNames = default("ccnx://universe.com/;ccnx://spacetime.com/");
        string dataNamePrefix = default("file");
        
        // other params
        int maxHopsAllowed = default(16);
        double interestRetransmitTimeout @unit(s) = default(5s);

        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
