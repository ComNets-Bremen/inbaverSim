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
      int currentFaceCounter;
      int arrivalFaceIndex;
      long currentCSSize;

      long hitCount;
      long missCount;
      long interestCount;
      long contentObjectCount;
      long firstCacheEmitCount;
      long tracerouteRqstCount;
      long tracerouteRplCount;

      long lastPITSize;
      long lastFIBSize;
      long lastCSSize;

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
      FaceEntry *getFaceEntryFromIndex(int faceIndex);
      CSEntry *getCSEntry(string prefixName, string dataName, string versionName, int segmentNum);
      PITEntry *getPITEntry(string prefixName, string dataName, string versionName, int segmentNum);
      FIBEntry *updateFIB(string prefixName, FaceEntry *faceEntry);
      FIBEntry *longestPrefixMatchingInFIB(string prefixName);
      PITEntry *getPITEntryUsingRPT(int rpt);
      int charToFaceIndex(char c);
      const char *faceIndexToChar(int faceIndex);

      void dumpFIB();
      void dumpFaces();
      void dumpCS();
      void dumpPIT();
      void updateCSEntry();
      void updatePITEntry();

      simsignal_t totalInterestsBytesReceivedSignal;
      simsignal_t totalInterestRtnsBytesReceivedSignal;
      simsignal_t totalContentObjsBytesReceivedSignal;
      simsignal_t totalTracerouteRqstsBytesReceivedSignal;
      simsignal_t totalTracerouteRplsBytesReceivedSignal;

      simsignal_t totalInterestsBytesSentSignal;
      simsignal_t totalInterestRtnsBytesSentSignal;
      simsignal_t totalContentObjsBytesSentSignal;
      simsignal_t totalTracerouteRqstsBytesSentSignal;
      simsignal_t totalTracerouteRplsBytesSentSignal;

      simsignal_t cacheSizeBytesSignal;
      simsignal_t cacheAdditionsBytesSignal;
      simsignal_t cacheRemovalsBytesSignal;

      simsignal_t fibEntryCountSignal;
      simsignal_t pitEntryCountSignal;

      simsignal_t cacheHitRatioSignal;
      simsignal_t cacheMissRatioSignal;

      simsignal_t networkCacheHitRatioSignal;
      simsignal_t networkCacheMissRatioSignal;

      simsignal_t interestToContentRatioSignal;
      simsignal_t totalTrafficSignal;
      simsignal_t totalTrafficBytesSignal;

      simsignal_t requestToReplyRatioSignal;

      simsignal_t totalExpiredPITCountSignal;
      simsignal_t totalSatisfiedPITCountSignal;
      simsignal_t durationOfPITEntrySignal;

      simsignal_t totalCacheEntriesCountSignal;
      simsignal_t durationOfFirstCacheEmitSignal;
      simsignal_t delayInRetrievingContent;

      simsignal_t totalNetworkPITEntryCountEmitSignal;
      simsignal_t totalNetworkFIBEntryCountEmitSignal;
      simsignal_t totalNetworkCSEntryCountEmitSignal;

      cMessage *statGenReminderEvent;
};

#define RFC8569WITHPINGFWD_STAT_GEN_REM_EVENT_CODE 271

#define CHAR_BASE "!"

#define MAX_FACES 94

#endif
