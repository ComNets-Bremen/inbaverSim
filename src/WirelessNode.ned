//
// A wireless CCN node with two network interfaces, one
// WLAN and the other Bluetooth. 
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

module WirelessNode extends Node
{
    parameters:
        @networkNode();

        numWiredTransports = 0;
        numWirelessTransports = 2;
        fwd.typename = "RFC8569Forwarder";

        // set first interface (WLAN)
        wireless[0].typename = "WirelessTransport";
        wireless[0].wirelessTechnology = "WLAN";
        wireless[0].operationMode = "client";
        wireless[0].connectString = "ccn";
        wireless[0].wirelessRange = 10m;
        wireless[0].dataRate = 1e6bps;
        wireless[0].packetErrorRate = 0.0pct;
        wireless[0].scanInterval = 1s;
        wireless[0].headerSize = 32byte;

        // set second interface (Bluetooth)
        wireless[1].typename = "WirelessTransport";
        wireless[1].wirelessTechnology = "Bluetooth";
        wireless[1].operationMode = "direct";
        wireless[1].connectString = "ccn";
        wireless[1].wirelessRange = 10m;
        wireless[1].dataRate = 1e6bps;
        wireless[1].packetErrorRate = 0.0pct;
        wireless[1].scanInterval = 1s;
        wireless[1].headerSize = 32byte;

        // set mobility default
        mobility.typename = "StationaryMobility";

        @display("i=device/palm2;bgb=418,317");

    submodules:



}
