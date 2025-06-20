//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Global singleton module that manages network-wide information and statistics
// for the CCN simulation. Maintains centralized data structures and coordinates
// operations that span multiple nodes, such as network-wide cache hit ratios and
// other performance metrics.
//
// The model focuses on collecting and aggregating network-wide statistics in
// detail, providing a global view of the simulation. However, it abstracts the
// actual mechanisms that would be needed to gather such information in a real
// network, such as management protocols and monitoring systems. It represents a
// simulation convenience rather than modeling a real-world entity, allowing
// direct access to information that would be difficult to obtain in a
// distributed system.
//
simple Demiurge
{
    parameters:

        @display("i=misc/sun;bgb=356,317");
}
