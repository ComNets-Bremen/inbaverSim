//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
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

