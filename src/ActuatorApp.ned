//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Configurable actuator application that simulates the actuation functionality 
// in actuators deployed in IoT networks. Supports duty cycling and communicates 
// with gateway nodes using CCN protocols.
//
//
simple ActuatorApp like IApplication
{
     parameters:
        // Prefix served by the this actuator expecting the gw to request for data - the actuator name is appended at the end
        string actuatorPrefixName = default("ccnx://uni-bremen.de/ee/actuator/");
        
        // Prefix used by this sensor to inform GW its presence and asking to send request for the sensor data
        string gwPrefixName = default("ccnx://uni-bremen.de/ee/gw/");

        // lifetime of a interest sent to the GW
        double interestLifetime @unit(s) = default(2s);

        // details of the segments
        volatile int segmentSize @unit(byte) = default(24byte);
        volatile int numSegmentsPerFile = default(1);
        
        // duration of validity of a data item
        volatile double cacheTime @unit(s) = default(900s); // 0 means infinite
        
        // types of data in which actuator is interested
        string actuatorDataTypes = default("temperature");
        
        // parameters for temperature setting
     //   volatile double temperature = default(22); // celsius
        
        // parameters for bulb setting
      //  volatile bool bulb = default(true); //on
        
        // maximum historical actuator readings to hold
        int maxActuatorDataReadingsToKeep = default(50);
               
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
