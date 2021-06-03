<p align="left">
  <img src="res/images/inbaverSim.png" width="250"/>
</p>

Network use has evolved over time to be dominated by content distribution and retrieval, while 
networking technology is mainly concerned with connections between named hosts. Accessing 
contents and services require the mapping of what users care about to where the content is 
located. **Content Centric Networking (CCN)** is a new paradigm in networking and a future Internet 
architecture which treats content as the primitive - decoupling location from identity, security 
and access - to retrieve content by name. 

**inbaverSim** is an OMNeT++ based model framework to simulate CCN based networks. The functionality and 
the message formats follow the description provided in [RFC 8569](https://tools.ietf.org/html/rfc8569) 
and [RFC 8609](https://tools.ietf.org/html/rfc8609). 

The sections that follow provide details of **inbaverSim** such as installing, node architecture, etc. 
The text immediately below is a TL;DR summary of how to install and run simulations with **inbaverSim**.

1. Install the latest version of [OMNeT++](https://omnetpp.org/documentation/)
2. Run the OMNeT++ IDE and install the INET model framework
3. Build the INET model framework in `release` mode
4. Import **inbaverSim** repository (this respository) by cloning from [GitHub inbaverSim](https://github.com/ComNets-Bremen/inbaverSim.git)
5. Build **inbaverSim** project
6. Run simulations using the `omnetpp-simple-network.ini`



## Node Models

**inbaverSim** is able to model the operation of different types of CCN nodes. As seen
from the picture below, it can model the following types of nodes.

- **Wireless Nodes** - Nodes that are able to connect to infrastructure based wireless networks (e.g., WLAN, LTE)
- **Wireless Access Routers** - Routers that are access points/base stations for wireless nodes
- **DTN/OppNets Nodes** - Wireless nodes that employ direct communications between nodes
- **Wired Nodes** - Nodes connected to networks over wired network attachments
- **Access Routers** - Routers for wired network nodes
- **Core Routers** - Central CCN routers that connect other networks
- **Content Servers** - Servers that host content

<p align="center">
  <img src="res/images/net-topology.png" width="500"/>
</p>



## Protocol Stack 

The node architecture of an **inbaverSim** node of any type consist of a 3-layer protoclol 
stack as seen from the following picture.

<p align="center">
  <img src="res/images/proto-stack.png" width="500"/>
</p>

- **Application**: All CCN based applications are deployed at this layer
- **Forwarding**: The fowarding mechanism described in [RFC 8569](https://tools.ietf.org/html/rfc8569) or 
any other custom CCN forwarding mechanism is implemented at this layer
- **Transport**: This layer implements different transports that connect nodes to deliver CCN packets. 
Currently, two transports are supported - wired and wireless transports 

The picture below shows the **inbaverSim** node model architecture in OMNeT++.

<p align="center">
  <img src="res/images/ccn-node-model.png" width="300"/>
</p>



## Installing

**inbaverSim** is written for OMNeT++ and additionally uses the INET model framework of 
OMNeT++. Therefore, they have to be installed first to use **inbaverSim**. The detailed
installation procedure is as follows.

1. Install OMNeT++ - OMNeT provides an [Install Guilde](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf) 
to install OMNeT++ in different operating systems. **inbaverSim** has been 
developed using the version 5.6.1 of OMNeT++.

2. Install INET Model Framework - Installing is done through the OMNeT++ IDE. The steps are as follows.
  - Run the OMNeT++ IDE
  - Create a new workspace
  - After creating the workspace, the IDE will prompt asking to install INET
  - Once the INET code base is installed, rebuild INET in `release` mode

3. Install **inbaverSim** and build as follows.
  - Import the code from the [GitHub](https://github.com/ComNets-Bremen/inbaverSim.git) repositorty
  - Build **inbaverSim**




## Running Simulations

The `simulation` folder has sample `.ini` files to run simulations. The procedure of running
a simulation is given below.

1. Setup the `'.ini` file to run simulations as follows.
  - Select a sample `.ini` file from `simulations` folder
  - Open file by double clicking
  - Add/adjust parameters as required (e.g., change `sim-time-limit` to a different duration)

2. Run the simulation as follows.
  - Select `.ini` file
  - Right-click and click on `Run As`->`OMNeT++ Simulation`
  - Select the `Config name` in the new window that will pop up
  - Select the `Run` button to run the simulation

The picture below shows a network created to simulate a CCN network.

<p align="center">
  <img src="res/images/simple-ccn-network.png" width="300"/>
</p>



## Statistics

Every simulation creates statistics during the simulation run. These statistics 
are collected in the `simulations/results` folder. Use the results viewing
capability of the OMNeT++ IDE to view the results.

The picture below shows the content download delays of a single node during a 48-hour 
period.


<p align="center">
  <img src="res/images/download-delay-graph.png" width="300"/>
</p>



## Questions and Comments

If you have any questions or comments about **inbaverSim**, write to us at 
inbaverSim@comnets.uni-bremen.de

