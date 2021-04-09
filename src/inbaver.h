
#ifndef INBAVER_H_
#define INBAVER_H_

#include <omnetpp.h>

#include "inet/mobility/contract/IMobility.h"

#include "WirelessTransport.h"
#include "WiredTransport.h"
#include "Deus.h"
#include "Numen.h"

using namespace std;


typedef struct WirelessTransportInfo {
    string macAddress;
    WirelessTransport *wirelessTransportModel;
    inet::IMobility *mobilityModel;
    cModule *nodeModel;
    Numen *numenModel;
    string wirelessTechnology;
    string operationMode;
    string connectString;

} WirelessTransportInfo;


typedef struct WiredTransportInfo {
    string macAddress;
    WiredTransport *wiredTransportModel;
    cModule *nodeModel;
    Numen *numenModel;
    inet::IMobility *mobilityModel;

} WiredTransportInfo;


typedef struct NodeInfo {
    long nodeID;
    cModule *nodeModel;
    Numen *numenModel;
    inet::IMobility *mobilityModel;
    list<WirelessTransportInfo*> wirelessTransportInfoList;
    list<WiredTransportInfo*> wiredTransportInfoList;

} NodeInfo;



typedef struct SameWirelessGroup {
    string wirelessTechnology;
    string operationMode;
    string connectString;

    list<WirelessTransportInfo*> wirelessTransportInfoList;

} SameWirelessGroup;



#define INBAVER_INTEREST_MSG_HEADER_SIZE                   16
#define INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE             16
#define INBAVER_INTEREST_RTN_MSG_HEADER_SIZE               16


typedef struct HostedContentEntry {
    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int totalNumSegments;

} HostedContentEntry;

enum FaceTypes
{
    ApplicationTypeFace = 1,
    TransportTypeFace = 2,
    ServiceTypeFace = 3
};

typedef struct FaceEntry {

    // the ID sent by application or interface or service
    long faceID;

    // type of face
    int faceType;  // application or transport or service
    string faceDescription; // a description assigned

    // gate names associated with the transport or application
    string baseGateName;
    string inputGateName; // $i appended to baseGateName
    string outputGateName; // $o appended to baseGateName
    int gateIndex;

    // variables specific to transport type faces
    string transportAddress; // MAC like address

} FaceEntry;

typedef struct FIBEntry {
    // CCNx prefix
    string prefixName;

   list <FaceEntry *> forwardedFaces;

} FIBEntry;

typedef struct PITEntry {
    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;

    // sender details
    string senderAddress;

    // interest received interface or application
    vector <FaceEntry *> receivedFaces;

} PITEntry;

typedef struct CSEntry {
    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;

    // CCNx other fields
    simtime_t cachetime;
    simtime_t expirytime;

    // total segments in the requested data (identified by prefixName + dataName)
    int totalNumSegments;

    // any payload
    string payloadAsString;

    // size in bytes
    int payloadSize;

} CSEntry;









#endif /* INBAVER_H_ */