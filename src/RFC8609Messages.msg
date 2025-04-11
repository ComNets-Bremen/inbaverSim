//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

//
// The formats of all the messages defined in RFC 8609 for
// communications between CCN nodes.
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

    //====== Others fields used only by simulator

    // msg sizes in bytes
    int headerSize;
    int payloadSize;

}
