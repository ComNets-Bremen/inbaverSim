//
// A file hosting (content server) application that 
// implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

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
