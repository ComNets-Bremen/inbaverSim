//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Client application that periodically retrieves sensor data from IoT networks
// through gateway nodes. Requests various types of sensor readings (temperature,
// humidity, etc.) using CCN Interest messages and stores historical data.
// Configurable for different retrieval patterns and data types.
//
// The model implements the CCN-based sensor data retrieval mechanisms in detail,
// including interest generation and content processing. However, it abstracts
// the actual data usage and analysis processes. User behavior is simplified to
// statistical distributions rather than context-aware patterns, and data
// visualization or processing algorithms are not modeled. Storage constraints
// are approximated through a simple limit on historical readings.
//
simple ThingsApp like IApplication
{
    parameters:

        // start time of sensor data retrieval
        double sensorDataRetrievalStartTime @unit(s) = default(10s);

        // time between sensor data retrievals
        volatile double interDataDownloadInterval @unit(s) = default(uniform(200s,500s));

        //  prefix of the sensor network
        string requestedSensorNetworkPrefixName = default("ccnx://uni-bremen.de/ee/iot/");

        // lifetime of a data request interest
        volatile double interestLifetime @unit(s) = default(10s);

        // names of the types of sensor data retrieved and random variable to select on each time
        string sensorDataNames = default("temperature;humidity;vwc;salinity;water;electricity");
        volatile int nextIndexOfSensorDataToRetrieve = default(intuniform(0, 6));

        // maximum historical sensor data reading to hold
        int maxSensorDataReadingsToKeep = default(50);

        // maximum hops allowed for Interest to travel
        int maxHopsAllowed = default(16);

        // statistic signals
        @signal[appContentDownloadDuration](type=simtime_t);
        @signal[appSegmentDownloadDuration](type=simtime_t);
        @signal[appTotalInterestsBytesSent](type=long);
        @signal[appRetransmissionInterestsBytesSent](type=long);
        @signal[appTotalContentObjsBytesReceived](type=long);
        @signal[appTotalDataBytesReceived](type=long);
        @signal[appNetworkInterestRetransmissionCount](type=long);
        @signal[appNetworkInterestInjectedCount](type=long);

        // display icon
        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
