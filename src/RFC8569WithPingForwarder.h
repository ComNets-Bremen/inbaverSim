//
// Created by Finke, Tarek & Wieker, Lars as part of the Network Simulation Project
// Extension to RFC8569Forwarder by Asanga Kudugama from 2021
//
// Adds the traceroute capability specified in RFC9507
// 

#ifndef __INBAVERSIM_RFC8569WITHPINGFORWARDER_H_
#define __INBAVERSIM_RFC8569WITHPINGFORWARDER_H_


#include <omnetpp.h>
#include <cstdlib>
#include <string>
#include <queue>

#include "Demiurge.h"
#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"
#include "Numen.h"

using namespace omnetpp;
using namespace std;

class Demiurge;
class Numen;

/**
 * Adds the traceroute capability under RFC9507
 */
class RFC8569WithPingForwarder : public cSimpleModule
{
    protected:
      virtual void initialize(int stage);
      virtual void handleMessage(cMessage *msg);
      virtual int numInitStages() const {return 3;}
      virtual void finish();

    private:
      int maximumContentStoreSize;

      Demiurge *demiurgeModel;
      Numen *numenModel;

      list <FaceEntry *> registeredFaces;
      list <FIBEntry *> fib;
      list <PITEntry *> pit;
      list <CSEntry *> cs;
      long currentCSSize;

      long hitCount;
      long missCount;

      void processApplicationRegistration(AppRegistrationMsg *appRegMsg);
      void processPrefixRegistration(PrefixRegistrationMsg *prefixRegMsg);
      void processTransportRegistration(TransportRegistrationMsg *transportRegMsg);
      void processInterest(InterestMsg *interestMsg);
      void processContentObj(ContentObjMsg *contentObjMsg);
      void processInterestRtn(InterestRtnMsg *interestRtnMsg);
      void processTracerouteRqst(TracerouteRqstMsg *tracerouteRqstMsg);
      void processTracerouteRpl(TracerouteRplMsg *tracerouteRplMsg);

      FaceEntry *getFaceEntryFromInputGateName(string inputGateName, int gateIndex);
      FaceEntry *getFaceEntryFromFaceID(long faceID);
      CSEntry *getCSEntry(string prefixName, string dataName, string versionName, int segmentNum);
      PITEntry *getPITEntry(string prefixName, string dataName, string versionName, int segmentNum);
      FIBEntry *updateFIB(string prefixName, FaceEntry *faceEntry);
      FIBEntry *longestPrefixMatchingInFIB(string prefixName);

      void dumpFIB();
      void dumpFaces();
      void dumpCS();
      void dumpPIT();

      simsignal_t totalInterestsBytesReceivedSignal;
      simsignal_t totalInterestRtnsBytesReceivedSignal;
      simsignal_t totalContentObjsBytesReceivedSignal;
      simsignal_t totalTracerouteRqstBytesReceivedSignal;
      simsignal_t totalTracerouteRplBytesReceivedSignal;
      simsignal_t totalInterestsBytesSentSignal;
      simsignal_t totalInterestRtnsBytesSentSignal;
      simsignal_t totalContentObjsBytesSentSignal;
      simsignal_t totalTracerouteRqstBytesSentSignal;
      simsignal_t totalTracerouteRplBytesSentSignal;
      simsignal_t cacheSizeBytesSignal;
      simsignal_t cacheAdditionsBytesSignal;
      simsignal_t cacheRemovalsBytesSignal;
      simsignal_t fibEntryCountSignal;
      simsignal_t pitEntryCountSignal;
      simsignal_t cacheHitRatioSignal;
      simsignal_t cacheMissRatioSignal;
      simsignal_t networkCacheHitRatioSignal;
      simsignal_t networkCacheMissRatioSignal;
};

#endif
