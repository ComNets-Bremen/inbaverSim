//
// An application that serves sensor data to the outside world
// and is able to receive and store sensor data from the local
// IoT. It implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 19-feb-2023
//
//

package inbaversim;

simple IoTGatewayApp like IApplication
{
    parameters:
        
        // Prefix hosted at the gateway for outside world to access sensor data
        string hostedPrefixName = default("ccnx://uni-bremen.de/ee/iot/");

        // Prefix hosted for the sensor network to send data
        string hostedPrefixNameForSensorNet = default("ccnx://uni-bremen.de/ee/gw/");

        // Prefix served by the sensor expecting the gw to request for data - the sensor name is appended to the end
        string sensorPrefixName = default("ccnx://uni-bremen.de/ee/sensor/");
        
       // Prefix served by the other Gateway in the Network - used to demonstGateway to Gateway Communication
        string otherGatewayAvailable = default("ccnx://uni-bremen.de/cs/iot/");
        
        // start time of sensor data retrieval (from other Gateway)
        double sensorDataRetrievalStartTime @unit(s) = default(10s);
        
        // names of the types of sensor data retrieved and random variable to select on each time
        string othersensorDataName = default("temperature");
        volatile int nextIndexOfSensorDataToRetrieve = default(intuniform(0, 4));
        
        // maximum hops allowed for Interest to travel
        int maxHopsAllowed = default(16);

        // time between sensor data retrievals(from other Gateway)
        volatile double interDataDownloadInterval @unit(s) = default(uniform(200s,500s));

        // lifetime of a interest sent to the sensor network
        double interestLifetime @unit(s) = default(2s);
        
        // maximum historical sensor reading to hold
        int maximumSensorReadingsToKeep = default(50);

        // statistic signals
        @signal[appTotalInterestsBytesReceived](type=long);
        @signal[appTotalContentObjsBytesSent](type=long);
        @signal[appTotalDataBytesSent](type=long);
        @signal[appSubContentObjectsSentToUserCount](type=long);


        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
