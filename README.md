<p align="left">
  <img src="res/images/inbaverSim.png" width="250"/>
</p>

Network use has evolved over time to be dominated by content distribution and retrieval, while 
networking technology is mainly concerned with connections between named hosts. Accessing 
contents and services require the mapping of what users care about to where the content is 
located. **Content Centric Networking (CCN)** is a new paradigm in networking and a future Internet 
architecture which treats content as the primitive - decoupling location from identity, security 
and access - to retrieve content by name. **inbaverSim** is an OMNeT++ based model framework to
simulate content centric networks. The functionality  and the message formats follow the description
provided in [RFC 8569](https://tools.ietf.org/html/rfc8569) and 
[RFC 8609](https://tools.ietf.org/html/rfc8609)


## Network Topology

**inbaverSim** is able to model the operation of different types of CCN node models. As seens
from the picture below, it can model the following types of nodes.

- Wireless Nodes - Nodes that are able to connect to infrastructure based wireless networks (e.g., WLAN, LTE)
- Wireless Access Routers - Routers that are access points/base stations for wireless nodes
- DTN/OppNets Nodes - Wireless nodes that employ direct communications between nodes
- Wired Nodes - Nodes connected to networks over wired network attachments
- Access Routers - Routers for wired network nodes
- Core Routers - Central CCN routers that connect other networks
- Content Servers - Servers that host content

<p align="center">
  <img src="res/images/net-topology.png" width="500"/>
</p>



## Protocol Stack 

The node architecture of an **inbaverSim** node of any type consist of a 3-layer protoclol 
stack as seen from the following picture.

<p align="center">
  <img src="res/images/proto-stack.png" width="500"/>
</p>

- Application: All CCN based applications are deployed at this layer
- Forwarding: The fowarding mechanism described in [RFC 8569](https://tools.ietf.org/html/rfc8569) or 
any other custom CCN forwarding mechanism is implemented at this layer
- Transport: This layer implements different transports that connect nodes to deliver CCN packets. 
Currently, two transports are supported - wired and wireless transports 

The picture below shows the CCN node model architecture in OMNeT++.

<p align="center">
  <img src="res/images/ccn-node-model.png" width="300"/>
</p>


## Running Simulations


<p align="center">
  <img src="res/images/simple-ccn-network.png" width="300"/>
</p>

