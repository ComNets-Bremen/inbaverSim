//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// An application that serves sensor data to the outside world
// and is able to receive and store sensor data from the local
// IoT. It implements the IApplication interface.
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

        // lifetime of a interest sent to the sensor network
        double interestLifetime @unit(s) = default(2s);

        // maximum historical sensor reading to hold
        int maximumSensorReadingsToKeep = default(50);

        // statistic signals
        @signal[appTotalInterestsBytesReceived](type=long);
        @signal[appTotalContentObjsBytesSent](type=long);
        @signal[appTotalDataBytesSent](type=long);


        @display("i=block/app");

    gates:
        inout forwarderInOut;

}
