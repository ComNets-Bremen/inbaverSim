//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Module interface that defines the common structure for all CCN transport
// components. Provides standardized connection points to both the forwarding
// layer and physical network interfaces, enabling the transmission of CCN
// messages over various link technologies.
//
// The model defines the interfaces between the transport layer and both the
// forwarding layer and physical interfaces in detail. However, it abstracts the
// internal transport implementation details. The focus is on the standardized
// way the transport layer interacts with other components rather than specifying
// how the transport mechanisms work internally. This allows for different
// transport implementations (wired, wireless, etc.) while maintaining
// compatibility with the forwarding layer.
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
