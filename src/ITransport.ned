//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Interface of a CCN transport.
//
moduleinterface ITransport
{
    parameters:
        @display("i=block/transport;t=Transport,l");

    gates:
        inout forwarderInOut;
        inout physicalInOut;
        input radioIn @directIn;
}
