//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Interface of a CCN forwarder.
//
moduleinterface IForwarder
{
    parameters:
        @display("i=block/fork;t=Forwarding,l");

    gates:
        inout appInOut[];
        inout transportInOut[];
}
