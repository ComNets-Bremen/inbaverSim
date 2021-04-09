//
// A file hosting (content server) application that
// implements the IApplication interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#ifndef __INBAVERSIM_CONTENTHOSTAPP_H_
#define __INBAVERSIM_CONTENTHOSTAPP_H_

#include <omnetpp.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <iostream>

#include "inbaver.h"
#include "RFC8609Messages_m.h"
#include "InternalMessages_m.h"
#include "Deus.h"

using namespace omnetpp;
using namespace std;

class Deus;

class ContentHostApp : public cSimpleModule
{

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const {return 3;}
    virtual void finish();

  private:
    string hostedPrefixNames;
    string dataNamePrefix;
    int segmentSize;
    int numSegmentsPerFile;
    double cacheTime;

    Deus *deusModel;

    // list of prefixes hosted
    vector <string> hostedPrefixList;

    // list to keep already requested file details
    vector <HostedContentEntry *> hostedContentList;

};

#define CONTENTHOSTAPP_SIMMODULEINFO             simTime() << ">!<" << getParentModule()->getFullName() << ">!<ContentHostApp>!<"
#define CONTENTHOSTAPP_APP_REG_REM_EVENT_CODE    116

#endif
