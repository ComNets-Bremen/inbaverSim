//
// Formats of messages used by transports
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

//
// Transport message that encapsulates CCNx messages
//
packet TransportMsg
{
    // source, destination addresses
    string sourceAddress;
    bool broadcastMsg;
    string destinationAddress;

    // msg sizes in bytes
    int headerSize;
    int payloadSize;

}

