//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Configurable sensor application that simulates various environmental and
// utility sensors in IoT networks. Generates realistic sensor readings for
// temperature, humidity, water content, salinity, and utility meters. Supports
// duty cycling and communicates with gateway nodes using CCN protocols.
//
// The model focuses on generating realistic sensor data patterns while
// abstracting the actual sensing hardware and mechanisms. Physical sensor
// characteristics like accuracy, precision, and calibration drift are not
// modeled. Energy consumption is approximated through simple duty cycling
// rather than detailed power models. Sensor data is generated using statistical
// distributions rather than physical environment models.
//
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
        
        //parameters for light intensity sensing
        volatile double lightIntensity = default(uniform(50, 1000)); // range from 50-1000 lux
        
        //parameters for motion sensor sensing
        volatile int motion = default(intuniform(0, 1)); // 0: No motion , 1: motion Detected
        
        // parameters for water meter sensing (waterIncrease must be based on wakeupInterval)
        double waterStart = default(uniform(10000, 20000)); // stating meter reading in liters
        volatile double waterIncrease = default(uniform(0.04, 0.06)); // meter increae amount in liters
        
        // parameters for electricity meter sensing (electricIncrease must be based on wakeupInterval)
        double electricStart = default(uniform(8000, 10000)); // starting meter reading, in kWh
        volatile double electricIncrease = default(uniform(0.0025, 0.005)); // meter increae amount in kWh
        
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
