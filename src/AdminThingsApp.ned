//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

// A module to simulate an application that interacts with sensor networks, 
// specifically for managing the retrieval and processing of sensor data. 
// The model is designed to simulate the process of collecting sensor data 
// from a network with parameters for the timing and structure of data requests.

simple AdminThingsApp like IApplication
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
        
        // details of the segments
        volatile int segmentSize @unit(byte) = default(24byte);
        volatile int numSegmentsPerFile = default(1);
        
        // duration of validity of a data item
        volatile double cacheTime @unit(s) = default(900s); // 0 means infinite
        
        // names of the types of sensor data retrieved and random variable to select on each time
        string sensorDataNames = default("temperature;humidity;vwc;salinity");
        volatile int nextIndexOfSensorDataToRetrieve = default(intuniform(0, 4));

        // maximum historical sensor data reading to hold
        int maxSensorDataReadingsToKeep = default(50);

        // maximum hops allowed for Interest to travel
        int maxHopsAllowed = default(16);
        
        //Subscription details
        double subscriptionStartTime @unit(s) = default(10s);
        bool subscriptionON = default(true);
        double subscriptionDuration @unit(s);
        
        //Configuration Settings
        double configurationSettingsStartTime @unit(s) = default(2s);
        string configurationParameter = default("TempLow");
        double configurationValue = default(13);

        // statistic signals
        @signal[appContentDownloadDuration](type=simtime_t);
        @signal[appSegmentDownloadDuration](type=simtime_t);
        @signal[appTotalInterestsBytesSent](type=long);
        @signal[appRetransmissionInterestsBytesSent](type=long);        
        @signal[appTotalContentObjsBytesReceived](type=long);
        @signal[appTotalDataBytesReceived](type=long);
        @signal[appNetworkInterestRetransmissionCount](type=long);
        @signal[appNetworkInterestInjectedCount](type=long);
        @signal[appContentFreshnessSignal](type=simtime_t);
        @signal[apptotalContentObjectReceivedCount](type=long);
        @signal[apptotalInterestsSentCount](type=long);
        @signal[appSubContentObjectsReceivedByUserCount](type=long);

        // display icon
        @display("i=block/app");

    gates:
        inout forwarderInOut;
}
