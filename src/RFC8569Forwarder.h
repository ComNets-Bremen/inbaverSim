//
// A CCN forwarder implementing the RFC 8569 using the
// IForwarder interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_RFC8569FORWARDER_H_
#define __INBAVERSIM_RFC8569FORWARDER_H_

#include <omnetpp.h>
#include <cstdlib>
#include <string>
#include <queue>

#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"
#include "Deus.h"
#include "Numen.h"

using namespace omnetpp;
using namespace std;

class Deus;
class Numen;

class RFC8569Forwarder : public cSimpleModule
{
  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();

  private:
    int maximumContentStoreSize;

    Deus *deusModel;
    Numen *numenModel;

    list <FaceEntry *> registeredFaces;
    list <FIBEntry *> fib;
    list <PITEntry *> pit;
    list <CSEntry *> cs;
    long currentCSSize;

    void processApplicationRegistration(AppRegistrationMsg *appRegMsg);
    void processTransportRegistration(TransportRegistrationMsg *transportRegMsg);
    void processInterest(InterestMsg *interestMsg);
    void processContentObj(ContentObjMsg *contentObjMsg);
    void processInterestRtn(InterestRtnMsg *interestRtnMsg);

    FaceEntry *getFaceEntryFromInputGateName(string inputGateName, int gateIndex);
    CSEntry *getCSEntry(string prefixName, string dataName, string versionName, int segmentNum);
    PITEntry *getPITEntry(string prefixName, string dataName, string versionName, int segmentNum);
    FIBEntry *longestPrefixMatchingInFIB(string prefixName);

    void dumpFIB();
    void dumpFaces();
    void dumpCS();
    void dumpPIT();

};

#define RFC8569FORWARDER_SIMMODULEINFO                 simTime() << ">!<" << getParentModule()->getFullName() << ">!<RFC8569Forwarder>!<"


#endif
