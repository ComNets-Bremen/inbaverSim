//
// A file download client (content requester) application that
// implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_CONTENTDOWNLOADAPP_H_
#define __INBAVERSIM_CONTENTDOWNLOADAPP_H_

#include <omnetpp.h>
#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"
#include "Deus.h"
#include "Numen.h"

using namespace omnetpp;
using namespace std;

class Deus;
class Numen;

class ContentDownloadApp : public cSimpleModule
{
  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();


  private:
    double interContentDownloadInterval;
    string requestedPrefixNames;
    string dataNamePrefix;
    int maxHopsAllowed;
    double interestRetransmitTimeout;

    Deus *deusModel;
    Numen *numenModel;

    vector <string> requestedPrefixList;

    // start content download event
    cMessage *contentDownloadStartEvent;

    // interest retransmission event
    cMessage *interestRetransmitEvent;

    // details of current download
    string requestingPrefixName;
    string requestingDataName;
    int requestedSegNum;
    int totalSegments;
    simtime_t contentDownloadStartTime;

    // stat signals
    simsignal_t contentDownloadDurationSignal;

};

#define CONTENTDOWNLOADAPP_SIMMODULEINFO                       simTime() << ">!<" << getParentModule()->getFullName() << ">!<ContentDownloadApp>!<"
#define CONTENTDOWNLOADAPP_APP_REG_REM_EVENT_CODE              116
#define CONTENTDOWNLOADAPP_START_CONTENT_DOWNLOAD_EVENT_CODE   117
#define CONTENTDOWNLOADAPP_INTEREST_RETRANSMIT_EVENT_CODE      118

#endif
