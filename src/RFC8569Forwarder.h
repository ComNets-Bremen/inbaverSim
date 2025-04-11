//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#ifndef __INBAVERSIM_RFC8569FORWARDER_H_
#define __INBAVERSIM_RFC8569FORWARDER_H_

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
 * A CCN forwarder implementing the RFC 8569 using the
 * IForwarder interface.
 */
class RFC8569Forwarder : public cSimpleModule
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
    simsignal_t totalInterestsBytesSentSignal;
    simsignal_t totalInterestRtnsBytesSentSignal;
    simsignal_t totalContentObjsBytesSentSignal;
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
