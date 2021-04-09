//
// A CCN forwarder implementing the RFC 8569 using the
// IForwarder interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

//
// TODO auto-generated module
//
simple RFC8569Forwarder like IForwarder
{
    parameters:

        // size of cache in bytes - zero means unlimited caching
        int maximumContentStoreSize @unit(byte) = default(0byte);
        
        @display("i=block/fork");

    gates:
        inout appInOut[];
        inout transportInOut[];
}
