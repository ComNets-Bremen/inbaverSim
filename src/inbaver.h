
#ifndef INBAVER_H_
#define INBAVER_H_

#include <omnetpp.h>

#include "inet/mobility/contract/IMobility.h"

using namespace std;
using namespace omnetpp;

class WirelessTransport;
class WiredTransport;
class Demiurge;
class Numen;

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

    //  variables specific to transport type faces
    string transportAddress; // address assigned to face by the transport

} FaceEntry;

typedef struct FIBEntry {
    // CCNx prefix
    string prefixName;

   list <FaceEntry *> forwardedFaces;

} FIBEntry;

typedef struct ArrivalInfo {

    // arrival received face
    FaceEntry *receivedFace;

    // transport address of sender
    string transportAddress;

} ArrivalInfo;

typedef struct PITEntry {
    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    int segmentNum;

    //hop info
    int hopLimit;
    int hopsTravelled;

    // arrival details of received interest
    vector <ArrivalInfo *> arrivalInfoList;

} PITEntry;

typedef struct CSEntry {
    // CCNx name segments
    string prefixName;
    string dataName;
    string versionName;
    long segmentNum;

    // CCNx other fields
    simtime_t cachetime;
    simtime_t expirytime;

    // total segments in the requested data (identified by prefixName + dataName)
    long totalNumSegments;

    // any payload
    string payloadAsString;

    // size in bytes
    long payloadSize;

} CSEntry;

class ExchangedTransportInfo : public omnetpp::cObject
{
  public:
    char *name;
    string transportAddress;
    ExchangedTransportInfo(const char *n):cObject() {name = (char *) malloc(strlen(n) + 1); strcpy(name, n);};
//    const char *getName() {return name;}
    virtual ExchangedTransportInfo *dup() const {ExchangedTransportInfo *t = new ExchangedTransportInfo("ExchangedTransportInfo"); t->transportAddress = transportAddress; return t;};
    ~ExchangedTransportInfo() {free(name);}

};

typedef struct SensorEntry {

    // sensor name (the OMNeT name given)
    string sensorName;

    // last reading
    string lastSensorReading;

    // lifetime of a single reading (also referred to as the cache time)
    double dataLifetime;

    // size of the a single sensor data reading
    int dataSize;

    // most recent sensor readings
    vector <string> lastSensorReadingsList;

    // prefix of the sensor and the transport face ID
    // through witch the sensor is reachable at the
    // forwarding layer
    string sensorPrefixName;
    long faceID;
    int faceType;
    bool prefixRegistered;

} SensorEntry;


typedef struct SensorDataEntry {

    // sensor data name
    string sensorDataName;

    // last reading
    string lastDataReading;

    // last reading time
    double lastDataReadingTime;

    // lifetime of a single reading (cache time)
    double dataLifetime;

    // most recent sensor data readings
    vector <string> lastDataReadingsList;

    // most recent sensor data reading times
    vector <double> lastDataReadingTimesList;

} SensorDataEntry;


#endif /* INBAVER_H_ */
