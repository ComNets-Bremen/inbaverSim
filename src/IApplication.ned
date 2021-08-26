//
// Interface of a CCN application.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

moduleinterface IApplication
{
    parameters:
        @display("i=block/app;t=Application,l");

    gates:
        inout forwarderInOut;

}
