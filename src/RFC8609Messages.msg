//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

//
// The formats of all the messages defined in RFC 8609 and the 
// Reflexive Forwarding draft for communications between CCN 
// nodes.
//

//
// Format of a CCNx Interest packet
//
packet InterestMsg
{

    //====== Interest Header (IntHdr)

    // number of hops left to travel
    int hopLimit;

    // lifetime of interest
    simtime_t lifetime;

    //====== Body Name (BodyName)

    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;

    //====== Validation (Validation)

    // no security aspects incorporated yet
    
    //Fields for Reflexive Interest
    string reflexiveNamePrefix;
    int forwardPendingToken;
    int reversePendingToken;


    //====== Others fields used only by simulator 

    // msg sizes in bytes
    int headerSize;
    int payloadSize;

    // hops
    int hopsTravelled;
    
    // arrival face ID and type at the forwarding (CCN) layer
    long arrivalFaceID;
    int arrivalFaceType;
}


//
// Format of a CCNx Content Object packet.
//
packet ContentObjMsg
{

    //====== ContentObj Header (ConObjHdr)

    // cache time of content
    simtime_t cachetime;

    //====== Body Opt Name (BodyOptName)

    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;

    // expiry time of con obj
    simtime_t expirytime;

    //====== Validation (Validation)
    
    //Fields for Reflexive Interest
    int reversePendingToken;

    // no security aspects incorporated yet


    //====== Others fields used only by simulator 

    // msg sizes in bytes
    int headerSize;
    int payloadSize;

    // total segments in the requested data (identified by prefixName + dataName)
    int totalNumSegments;

    // any payload
    string payloadAsString;

}

enum ReturnCodeTypes
{
    ReturnCodeTypeNoRoute = 0;
    ReturnCodeTypeHopLimitExceeded = 1;
    ReturnCodeTypeMTUTooLarge = 2;
    ReturnCodeTypeNoResources = 3;
    ReturnCodeTypePathError = 4;
    ReturnCodeTypeProhibited = 5;
    ReturnCodeTypeCongestion = 6;
    ReturnCodeTypeReturnCodeTypeUnsupportedHash = 7;
    ReturnCodeTypeMalformedInterest = 8;
}

//
// Format of a CCNx Interest Return packet.
//
packet InterestRtnMsg
{

    //====== Return Code 

    // return reason
    int returnCode @enum(ReturnCodeTypes);

    //====== Interest

    // will be encapsulated

    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;
    
    //Fields for Reflexive Interest
    string reflexiveNamePrefix;
    int forwardPendingToken;
    int reversePendingToken;

    //====== Others fields used only by simulator 

    // msg sizes in bytes
    int headerSize;
    int payloadSize;
    

}

//
// Format of Traceroute Request interest packet
//
packet TracerouteRqstMsg
{
    //====== Traceroute Request Header (IntHdr)

    // number of hops left to travel
    int hopLimit;

    // lifetime of interest
    simtime_t lifetime;    
    
    // Pathsteering
    string pathlabel;
    
    //====== Body Name (BodyName)

    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;
    

    //====== Validation (Validation)

    // no security aspects incorporated yet
    
    
    //====== Others fields used only by simulator
    
    // msg sizes in bytes
    int headerSize;
    int payloadSize;

    // hops
    int hopsTravelled;

    // arrival face ID and type at the forwarding (CCN) layer
    long arrivalFaceID;
    int arrivalFaceType;
}


enum TracerouteReplyCodes
{
	ReturnCodeForwarderNameMatch = 0;
	ReturnCodeCSHit = 1;
	ReturnCodeFIBLocalHit = 2;
	ReturnCodeHopLimitExceeded = 4;    
}


//
// Format of Traceroute Request reply packet
//
packet TracerouteRplMsg
{    
    //====== TracerouteRpl Header (TraRplHdr)

    // cache time of content
    simtime_t cachetime;
    
     // Pathsteering
    string pathlabel;
    
    // Reply Code
    int tracerouteReplyCode @enum(TracerouteReplyCodes);

    //====== Body Opt Name (BodyOptName)

    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;

    // expiry time of con obj
    simtime_t expirytime;

    //====== Validation (Validation)

    // no security aspects incorporated yet


    //====== Others fields used only by simulator

    // msg sizes in bytes
    int headerSize;
    int payloadSize;

    // total segments in the requested data (identified by prefixName + dataName)
    int totalNumSegments;

    // any payload
    string payloadAsString;
}
