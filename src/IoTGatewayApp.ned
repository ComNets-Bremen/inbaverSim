//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Gateway application that bridges IoT sensor networks with the wider CCN
// network. Collects and stores data from local sensors while making this
// information available to external requesters. Manages name prefixes for both
// sensor data collection and external data serving.
//
// The model implements the CCN-based data collection and serving mechanisms in
// detail, including name-based routing between sensor and Internet domains.
// However, it abstracts gateway-specific functions like protocol translation,
// data aggregation, and filtering. Security aspects such as access control and
// data validation are not modeled. Storage constraints are approximated through
// a simple limit on historical readings rather than detailed memory management.
//
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
        
        //For Controller action
        double temperatureLowerLimit = default(15);
        double temperatureHigherLimit = default(25);
        double lightIntensityLowerLimit = default(50);
        double lightIntensityUpperLimit = default(500);

        // statistic signals
        @signal[appTotalInterestsBytesReceived](type=long);
        @signal[appTotalContentObjsBytesSent](type=long);
        @signal[appTotalDataBytesSent](type=long);
        @signal[appSubContentObjectsSentToUserCount](type=long);


        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
