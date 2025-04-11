//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Module interface that defines the common structure for CCN forwarding
// components. Specifies the connection points to both applications and
// transport layers, enabling the forwarding of Interest and Content Object
// messages between network interfaces and applications.
//
// The model defines the interfaces between the forwarding layer and both
// applications and transports in detail. However, it abstracts the internal
// forwarding implementation details. The focus is on the standardized way the
// forwarding layer interacts with other components rather than specifying how
// the forwarding decisions are made or how the data structures are implemented.
// This allows for different forwarding implementations while maintaining
// compatibility with applications and transports.
//
moduleinterface IForwarder
{
    parameters:
        @display("i=block/fork;t=Forwarding,l");

    gates:
        inout appInOut[];
        inout transportInOut[];
}
