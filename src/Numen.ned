//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

package inbaversim;

//
// Node-level control module that manages node-specific information and
// coordinates operations between different layers of the CCN protocol stack.
// Maintains local data structures and statistics for monitoring node
// performance.
//
// The model focuses on collecting and managing node-specific statistics and
// facilitating cross-layer communication within a node. However, it abstracts
// the actual implementation mechanisms that would be needed in a real system,
// such as inter-process communication or shared memory. Like Demiurge, it
// represents a simulation convenience rather than modeling a real-world entity,
// providing direct access to information that would typically be distributed
// across different software components.
//
simple Numen
{
    parameters:

        @display("i=misc/globe;bgb=356,317");
}
