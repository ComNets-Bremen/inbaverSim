//
// Interface of a CCN forwarder.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//
package inbaversim;

moduleinterface IForwarder
{
    parameters:
        @display("i=block/fork");

    gates:
        inout appInOut[];
        inout transportInOut[];
}
