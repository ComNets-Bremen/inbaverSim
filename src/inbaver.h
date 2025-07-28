//
// Copyright (C) 2025 Asanga Udugama (udugama@uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

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
#define INVAVER_TRACEROUTE_RQST_MSG_HEADER_SIZE            16
#define INBAVER_TRACEROUTE_RPL_MSG_HEADER_SIZE             16


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

    //Expiry time
    simtime_t expirytime;

    //Reflexive Forwarding fields
    int forwardPendingToken_old;
    int forwardPendingToken_new;
    int reversePendingToken_old;
    int reversePendingToken_new;
    string reflexiveNamePrefix;

    //hop info
    int hopLimit;
    int hopsTravelled;

    // arrival details of received interest
    vector <ArrivalInfo *> arrivalInfoList;

    //Creation Time
    simtime_t creationtime;

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

    // last reading ID
    int lastSensorReadingID;

    // last reading recorded time
    simtime_t lastSensorReadingRecordedTime;

    // lifetime of a single reading (also referred to as the cache time)
    double dataLifetime;

    // size of the a single sensor data reading
    int dataSize;

    // most recent sensor readings
    vector <string> lastSensorReadingsList;

    // ids of sensor readings
    vector <int> lastSensorReadingsListID;

    // prefix of the sensor and the transport face ID
    // through witch the sensor is reachable at the
    // forwarding layer
   // string sensorPrefixName;
    long faceID;
    int faceType;
    bool prefixRegistered;

    //Registration Id
    string registrationId;

    //FPT
    int initialFPT;

    //Prefixname
    string initialPrefixName;

    //Dataname
    string initialDataName;

} SensorEntry;

typedef struct ActuatorEntry {

    // actuator name (the OMNeT name given)
    string actuatorName;

    // last reading
    bool lastActuatorState;

    // last reading recorded time
    simtime_t lastActuatorReadingRecordedTime;

    //Data types in which actuator is interested
    string actuatorDataTypes;

    // lifetime of a single reading (also referred to as the cache time)
    double dataLifetime;

    // size of the a single sensor data reading
    int dataSize;

    // most recent actuator readings
    vector <bool> lastActuatorReadingsList;

    // prefix of the actuator and the transport face ID
    // through which the sensor is reachable at the
    // forwarding layer
   // string actuator subscription status;
    long faceID;
    int faceType;
    bool subscriptionStatus;

    //FPT
    int initialFPT;

    //ReflPrefixname
    string initialReflPrefixName;

    //Dataname
   // string initialDataName;

} ActuatorEntry;

typedef struct SubscriptionData{

    //Subscribed User
    string subscribedUser;

    //Subscribed Data
    string subscribedData;

    //Subscribed Duration
    double subscribedDuration;

    //Subscription Expire Time
    double subscribedDurationExpiry;

    //FPT of main Interest
    int subscriptionFPT;

    //Prefix
    string reflexivePrefix;

    //Prefix name and Data name
    string subscriptionPrefixName;
    string subscriptionDataName;


}SubscriptionData;

typedef struct SubscriptionExpiryData{

    //Subscribed Prefix Name
    string subscriptionExpPrefixName;

    //Subscribed Data name
    string subscriptionExpDataName;

    //Subscription Expiry
    double subscriptionExpiryTime;

    //FPT of main Interest
    int subscriptionExpFPT;


}SubscriptionExpiryData;

typedef struct SensorDataEntry {

    // sensor data name
    string sensorDataName;

    // last reading
    string lastDataReading;

    // last reading ID
    int lastDataReadingID;

    // last reading time
    double lastDataReadingTime;

    // lifetime of a single reading (cache time)
    double dataLifetime;

    // most recent sensor data readings
    vector <string> lastDataReadingsList;

    // most recent sensor data reading times
    vector <double> lastDataReadingTimesList;

} SensorDataEntry;

typedef struct ActuatorDataEntry {

    // sensor data name
    string actuatorDataName;

    // current setting
    bool actuatorState;

    // current setting time(i.e, when was this setting done)
    double lastActuatorStateTime;

    // most recent actuator data readings
    vector <bool> lastActuatorStateList;

    // most recent actuator data reading times
    vector <double> lastActuatorStateTimesList;

} ActuatorDataEntry;


#define DUMP_MODULE_INFO(str, msg)       cout << simTime() << " " \
                                          << getParentModule()->getFullName() << " " \
                                          << this->getFullName() << " " << str << " " \
                                          << msg->getName() << "\n"


#endif /* INBAVER_H_ */
