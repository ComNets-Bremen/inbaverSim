//
// A generic sensing application that is able to be
// configured to return different data values of 
// sensors. It implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 18-feb-2023
//
//

package inbaversim;

simple SensingApp like IApplication
{
    parameters:
        // Prefix served by the this sensor expecting the gw to request for data - the sensor name is appended at the end
        string sensorPrefixName = default("ccnx://uni-bremen.de/ee/sensor/");
        
        // Prefix used by this sensor to inform GW its presence and asking to send request for the sensor data
        string gwPrefixName = default("ccnx://uni-bremen.de/ee/gw/");

        // lifetime of a interest sent to the GW
        double interestLifetime @unit(s) = default(2s);

        // details of the segments
        volatile int segmentSize @unit(byte) = default(24byte);
        volatile int numSegmentsPerFile = default(1);
        
        // duration of validity of a data item
        volatile double cacheTime @unit(s) = default(900s); // 0 means infinite
        
        // types of data sensed
        string sensedDataTypes = default("temperature;humidity;vwc;salinity");

        // parameters for temperature sensing
        volatile double temperature = default(uniform(5.0, 7.0)); // celsius
        
        // parameters for humidity sensing
        volatile double humidity = default(uniform(45.0, 60.0)); // percetage
        
        // parameters for volumetric water content sensing
        volatile double vwc = default(uniform(20.0, 30.0)); // percentage

        // parameters for salinity sensing
        volatile int salinity = default(intuniform(1000, 1200)); // range from 0-23000 uS/cm
        
        // duty cycling info
        volatile double wakeupInterval @unit(s) = default(10s);

        // statistic signals
        @signal[appTotalInterestsBytesReceived](type=long);
        @signal[appTotalContentObjsBytesSent](type=long);
        @signal[appTotalDataBytesSent](type=long);

        @signal[appSegmentDownloadDuration](type=simtime_t);
        @signal[appTotalInterestsBytesSent](type=long);
        @signal[appTotalContentObjsBytesReceived](type=long);
        @signal[appTotalDataBytesReceived](type=long);
        @signal[appNetworkInterestInjectedCount](type=long);

        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
