//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Module interface that defines the common structure for all CCN applications.
// Provides a standardized connection point to the forwarding layer, allowing
// applications to send and receive CCN messages through a unified interface.
//
// The model defines the interface between applications and the CCN forwarding
// layer in detail. However, it abstracts the internal application architecture
// and implementation details. Application-specific protocols, data formats, and
// user interfaces are not part of this interface definition. The focus is on the
// standardized way applications interact with the CCN network layer rather than
// their specific functionality.
//
moduleinterface IApplication
{
    parameters:
        @display("i=block/app;t=Application,l");

    gates:
        inout forwarderInOut;

}
