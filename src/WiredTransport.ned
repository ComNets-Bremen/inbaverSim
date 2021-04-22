//
// A wired transport that implements the ITransport
// interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

package inbaversim;

simple WiredTransport like ITransport
{
    parameters:

        // wired technology -> Ethernet, Fibre Optic
        string wiredTechnology = default("Ethernet");

        // bandwidth in bits per second
        double dataRate @unit(bps) = default(1e6bps);

        // packet error rate
        double packetErrorRate @unit(pct) = default(0.0pct);

        // header size
        int headerSize @unit(byte) = default(32byte);

        // statistic signals
        @signal[transportTotalWiredBytesReceived](type=long);
        @signal[transportTotalWiredBytesSent](type=long);

        // display icon
        @display("i=block/transport");

    gates:
        inout forwarderInOut;
        inout physicalInOut;
        input radioIn @directIn;

}
