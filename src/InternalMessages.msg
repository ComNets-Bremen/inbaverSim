//
// The formats of all the messages used internally by a node to
// communication between models.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

//
// Format of app registration msg (sent by any app to forwarder).
//
packet AppRegistrationMsg {
    
    // ID that uniquely identifies the registering app
    long appID;
    
    // is it a content server or a client application?
    bool contentServerApp;
    
    // if content server, the prefixs for which content exist
    string hostedPrefixNames[];
    
    // a description
    string appDescription;
}


//
// Format of transport registration msg (sent by any transport to forwarder).
//
packet TransportRegistrationMsg {
    
    // ID that uniquely identifies the registering transport
    long transportID;
        
    // a description
    string transportDescription;
    
    // transport address (MAC like address)
    string transportAddress;

}

