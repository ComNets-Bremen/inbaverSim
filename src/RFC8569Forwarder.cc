//
// A CCN forwarder implementing the RFC 8569 using the
// IForwarder interface.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 31-mar-2021
//
//

#include "RFC8569Forwarder.h"

Define_Module(RFC8569Forwarder);

void RFC8569Forwarder::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        maximumContentStoreSize = par("maximumContentStoreSize");

    } else if (stage == 1) {

        // get Deus model instance
        deusModel = NULL;
        for (int id = 0; id <= getSimulation()->getLastComponentId(); id++) {
            cModule *unknownModel = getSimulation()->getModule(id);
            if (unknownModel == NULL) {
                continue;
            }
            if (dynamic_cast<Deus*>(unknownModel) != NULL) {
                deusModel = dynamic_cast<Deus*>(unknownModel);
                break;
            }
        }

        // get Numen model instance
        numenModel = NULL;
        for (cModule::SubmoduleIterator it(getParentModule()); !it.end(); it++) {
            cModule *unknownModel = *it;
            if (unknownModel == NULL) {
                continue;
            }
            if (dynamic_cast<Numen*>(unknownModel) != NULL) {
                numenModel = dynamic_cast<Numen*>(unknownModel);
                break;
            }
        }

        // when Deus or Numen not found, terminate
        if (deusModel == NULL || numenModel == NULL) {
            EV_FATAL <<  RFC8569FORWARDER_SIMMODULEINFO << "The global Deus instance and/or node specific Numen instance not found.\n";
            throw cRuntimeError("Check log for details");
        }

        currentCSSize = 0;

    } else if (stage == 2) {

    } else {
        EV_FATAL <<  RFC8569FORWARDER_SIMMODULEINFO << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");
    }

}

void RFC8569Forwarder::handleMessage(cMessage *msg)
{
    cGate *arrivalGate;
    char gateName[32];
    AppRegistrationMsg *appRegMsg = NULL;
    TransportRegistrationMsg *transportRegMsg = NULL;
    InterestMsg *interestMsg = NULL;
    ContentObjMsg *contentObjMsg = NULL;
    InterestRtnMsg *interestRtnMsg = NULL;

    // self messages
    if (msg->isSelfMessage()) {
        delete msg;

    } else {

        // handle app registration msg from app
        if (strstr(gateName, "appInOut") != NULL && (appRegMsg = dynamic_cast<AppRegistrationMsg*>(msg)) != NULL) {

            processApplicationRegistration(appRegMsg);

        // handle transport registration msg from network interface (wireless or wired)
        } else if (strstr(gateName, "transportInOut") != NULL && (transportRegMsg = dynamic_cast<TransportRegistrationMsg*>(msg)) != NULL) {

            processTransportRegistration(transportRegMsg);

        // process received interest msg
        } else if ((interestMsg = dynamic_cast<InterestMsg*>(msg)) != NULL) {

            processInterest(interestMsg);

        // process received content obj msg
        } else if ((contentObjMsg = dynamic_cast<ContentObjMsg*>(msg)) != NULL) {

            processContentObj(contentObjMsg);

        // process received interest rtn msg
        } else if ((interestRtnMsg = dynamic_cast<InterestRtnMsg*>(msg)) != NULL) {

            processInterestRtn(interestRtnMsg);

        } else {

            EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }

    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " handle msg done " << " \n";


    return;

}

void RFC8569Forwarder::processApplicationRegistration(AppRegistrationMsg *appRegMsg)
{
    cGate *arrivalGate = appRegMsg->getArrivalGate();
    char gateName[64];

    // create face entry
    FaceEntry *faceEntry = new FaceEntry;
    faceEntry->faceID = appRegMsg->getAppID();
    faceEntry->faceType = ApplicationTypeFace;
    faceEntry->faceDescription = appRegMsg->getAppDescription();
    faceEntry->inputGateName = string(arrivalGate->getName());
    faceEntry->baseGateName = string(arrivalGate->getBaseName());
    sprintf(gateName, "%s$o", arrivalGate->getBaseName());
    faceEntry->outputGateName = string(gateName);
    if (arrivalGate->isVector()) {
        faceEntry->gateIndex = arrivalGate->getIndex();
    } else {
        faceEntry->gateIndex = -1;
    }
    registeredFaces.push_back(faceEntry);

    // when app hosts prefixes, add to FIB and setup for late dissemination
    if (appRegMsg->getContentServerApp()) {
        for (int i = 0; i < appRegMsg->getHostedPrefixNamesArraySize(); i++) {
            string prefixName = appRegMsg->getHostedPrefixNames(i);
            FIBEntry *fibEntry = new FIBEntry;
            fibEntry->prefixName = prefixName;
            fibEntry->forwardedFaces.push_back(faceEntry);

            fib.push_back(fibEntry);
        }
    }

    delete appRegMsg;
}

void RFC8569Forwarder::processTransportRegistration(TransportRegistrationMsg *transportRegMsg)
{
    cGate *arrivalGate = transportRegMsg->getArrivalGate();
    char gateName[48];
    char tempStr[32];

    // create face entry
    FaceEntry *faceEntry = new FaceEntry;
    faceEntry->faceID = transportRegMsg->getTransportID();
    faceEntry->faceType = TransportTypeFace;
    faceEntry->faceDescription = transportRegMsg->getTransportDescription();
    faceEntry->transportAddress = transportRegMsg->getTransportAddress();
    faceEntry->inputGateName = string(arrivalGate->getName());
    faceEntry->baseGateName = string(arrivalGate->getBaseName());
    sprintf(gateName, "%s$o", arrivalGate->getBaseName());
    faceEntry->outputGateName = string(gateName);
    if (arrivalGate->isVector()) {
        faceEntry->gateIndex = arrivalGate->getIndex();
    } else {
        faceEntry->gateIndex = -1;
    }
    registeredFaces.push_back(faceEntry);

    // append face to the default FIB entry
    bool found = false;
    FIBEntry *fibEntry = NULL;
    list<FIBEntry*>::iterator iteratorFIBEntry = fib.begin();
    while (iteratorFIBEntry != fib.end()) {
        fibEntry = *iteratorFIBEntry;
        if (strcmp(fibEntry->prefixName.c_str(), "default") == 0) {
            found = true;
            break;
        }

        iteratorFIBEntry++;
    }
    if (!found) {
        fibEntry = new FIBEntry;
        fibEntry->prefixName = "default";
        fib.push_back(fibEntry);
    }
    fibEntry->forwardedFaces.push_back(faceEntry);

    delete transportRegMsg;
}

void RFC8569Forwarder::processInterest(InterestMsg *interestMsg)
{
    // get arrival gate details
    cGate *arrivalGate = interestMsg->getArrivalGate();
    int arrivalGateIndex =  (arrivalGate->isVector() ? arrivalGate->getIndex() : (-1));
    FaceEntry *arrivalFaceEntry = getFaceEntryFromInputGateName(arrivalGate->getName(), arrivalGateIndex);

    // can interest be served from CS?
    CSEntry *csEntry = getCSEntry(interestMsg->getPrefixName(), interestMsg->getDataName(),
                                    interestMsg->getVersionName(), interestMsg->getSegmentNum());
    if (csEntry != NULL) {

        ContentObjMsg *contentObjMsg = new ContentObjMsg("ContentObj");
        contentObjMsg->setPrefixName(csEntry->prefixName.c_str());
        contentObjMsg->setDataName(csEntry->dataName.c_str());
        contentObjMsg->setVersionName(csEntry->versionName.c_str());
        contentObjMsg->setSegmentNum(csEntry->segmentNum);
        contentObjMsg->setCachetime(csEntry->cachetime);
        contentObjMsg->setExpirytime(csEntry->expirytime);
        contentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
        contentObjMsg->setPayloadSize(csEntry->payloadSize);
        contentObjMsg->setTotalNumSegments(csEntry->totalNumSegments);
        contentObjMsg->setPayloadAsString(csEntry->payloadAsString.c_str());
        contentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + csEntry->payloadSize);

        // send content obj
        cGate *sendingGate = gate(arrivalFaceEntry->outputGateName.c_str(), arrivalFaceEntry->gateIndex);
        send(contentObjMsg, sendingGate);

        //if (strstr(getParentModule()->getFullName(), "ContentServer02") != NULL)
        EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " content entry found  " << "\n";


        delete interestMsg;
        return;
    }

    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " no content entry found  " << "\n";

    // has interest been seen before? (i.e., is it in PIT?)
    PITEntry *pitEntry = getPITEntry(interestMsg->getPrefixName(), interestMsg->getDataName(),
                                    interestMsg->getVersionName(), interestMsg->getSegmentNum());
    if (pitEntry != NULL) {

        EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << "  PIT entry found  " << "\n";

        // if PIT entry exists, check if the same Interest was received through the same face
        bool found = false;
        for (int i = 0; i < pitEntry->receivedFaces.size(); i++) {
            if(pitEntry->receivedFaces[i]->faceID == arrivalFaceEntry->faceID) {
                found = true;
                break;
            }
        }
        if (!found) {
            EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " no face in PIT entry found  " << "\n";

            pitEntry->receivedFaces.push_back(arrivalFaceEntry);
        }

        delete interestMsg;
        return;
    }

    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " no PIT entry found  " << "\n";

    // discard interest if it has reached the maximum hop count
    if ((interestMsg->getHopsTravelled() + 1) > interestMsg->getMaxHopsAllowed()) {

        //if (strstr(getParentModule()->getFullName(), "ContentServer02") != NULL)
        EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " interest exceeded hops  " << "\n";

        delete interestMsg;
        return;
    }

    // save interest in PIT - create new PIT entry
    pitEntry = new PITEntry;
    pitEntry->prefixName = interestMsg->getPrefixName();
    pitEntry->dataName = interestMsg->getDataName();
    pitEntry->versionName = interestMsg->getVersionName();
    pitEntry->segmentNum = interestMsg->getSegmentNum();
    pitEntry->senderAddress = interestMsg->getSourceAddress();
    pitEntry->receivedFaces.push_back(arrivalFaceEntry);
    pit.push_back(pitEntry);

    // find which FIB entry to use to forward the Interest
    FIBEntry *fibEntry = longestPrefixMatchingInFIB(interestMsg->getPrefixName());

    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " doing longestPrefixMatchingInFIB, at least the default must be returned  " << "\n";

    // forward interest to all the faces listed in the FIB entry
    FaceEntry *faceEntry = NULL;
    list<FaceEntry*>::iterator iteratorFaceEntry = fibEntry->forwardedFaces.begin();
    while (iteratorFaceEntry != fibEntry->forwardedFaces.end()) {
        faceEntry = *iteratorFaceEntry;

        // select face if it is not the arrival face
        if (faceEntry->faceID != arrivalFaceEntry->faceID) {

            InterestMsg *newInterestMsg = new InterestMsg("Interest");
            newInterestMsg->setHopLimit(interestMsg->getHopLimit() - 1);
            newInterestMsg->setLifetime(interestMsg->getLifetime());
            newInterestMsg->setPrefixName(interestMsg->getPrefixName());
            newInterestMsg->setDataName(interestMsg->getDataName());
            newInterestMsg->setVersionName(interestMsg->getVersionName());
            newInterestMsg->setSegmentNum(interestMsg->getSegmentNum());
            newInterestMsg->setHeaderSize(INBAVER_INTEREST_MSG_HEADER_SIZE);
            newInterestMsg->setPayloadSize(interestMsg->getPayloadSize());
            newInterestMsg->setHopsTravelled(interestMsg->getHopsTravelled() + 1);
            newInterestMsg->setByteLength(INBAVER_INTEREST_MSG_HEADER_SIZE + 0);

            cGate *sendingGate = gate(faceEntry->outputGateName.c_str(), faceEntry->gateIndex);

            send(newInterestMsg, sendingGate);

            EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " sending interest out  " << "\n";

        }

        iteratorFaceEntry++;
    }

    delete interestMsg;
    return;
}

void RFC8569Forwarder::processContentObj(ContentObjMsg *contentObjMsg)
{

    // check if the content obj is already in content store
    CSEntry *csEntry = getCSEntry(contentObjMsg->getPrefixName(), contentObjMsg->getDataName(),
                                    contentObjMsg->getVersionName(), contentObjMsg->getSegmentNum());
    if (csEntry != NULL) {
        EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " content already in CS " << "\n";
        delete contentObjMsg;
        return;
    }

    // before adding content to CS, check if size exceeded
    if (maximumContentStoreSize > 0) {
        while ((currentCSSize + contentObjMsg->getPayloadSize()) > maximumContentStoreSize) {
            CSEntry *removingCSEntry = cs.front();
            cs.pop_front();
            currentCSSize -= removingCSEntry->payloadSize;
            delete removingCSEntry;
        }
    }

    // add content to CS
    csEntry = new CSEntry;
    csEntry->prefixName = contentObjMsg->getPrefixName();
    csEntry->dataName = contentObjMsg->getDataName();
    csEntry->versionName = contentObjMsg->getVersionName();
    csEntry->segmentNum = contentObjMsg->getSegmentNum();
    csEntry->cachetime = contentObjMsg->getCachetime();
    csEntry->expirytime = contentObjMsg->getExpirytime();
    csEntry->totalNumSegments = contentObjMsg->getTotalNumSegments();
    csEntry->payloadAsString = contentObjMsg->getPayloadAsString();
    csEntry->payloadSize = contentObjMsg->getPayloadSize();
    cs.push_back(csEntry);
    currentCSSize += contentObjMsg->getPayloadSize();

    // forward to the faces in the
    PITEntry *pitEntry = getPITEntry(contentObjMsg->getPrefixName(), contentObjMsg->getDataName(),
                            contentObjMsg->getVersionName(), contentObjMsg->getSegmentNum());
    if (pitEntry == NULL) {
        delete contentObjMsg;
        return;
    }

    for (int i = 0; i < pitEntry->receivedFaces.size(); i++) {
        FaceEntry *faceEntry = pitEntry->receivedFaces[i];

        ContentObjMsg *newContentObjMsg = new ContentObjMsg("ContentObj");
        newContentObjMsg->setPrefixName(contentObjMsg->getPrefixName());
        newContentObjMsg->setDataName(contentObjMsg->getDataName());
        newContentObjMsg->setVersionName(contentObjMsg->getVersionName());
        newContentObjMsg->setSegmentNum(contentObjMsg->getSegmentNum());
        newContentObjMsg->setCachetime(contentObjMsg->getCachetime());
        newContentObjMsg->setExpirytime(contentObjMsg->getExpirytime());
        newContentObjMsg->setHeaderSize(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE);
        newContentObjMsg->setPayloadSize(contentObjMsg->getPayloadSize());
        newContentObjMsg->setTotalNumSegments(contentObjMsg->getTotalNumSegments());
        newContentObjMsg->setPayloadAsString(contentObjMsg->getPayloadAsString());
        newContentObjMsg->setByteLength(INBAVER_CONTENT_OBJECT_MSG_HEADER_SIZE + contentObjMsg->getPayloadSize());

        // send content obj
        cGate *sendingGate = gate(faceEntry->outputGateName.c_str(), faceEntry->gateIndex);
        send(newContentObjMsg, sendingGate);

    }

    pitEntry->receivedFaces.clear();
    pit.remove(pitEntry);
    delete pitEntry;


    delete contentObjMsg;
}

void RFC8569Forwarder::processInterestRtn(InterestRtnMsg *interestRtnMsg)
{
    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " got upper layer interest rtn "  << " \n";

    // Is there a PIT entry for the returning interest?
    PITEntry *pitEntry = getPITEntry(interestRtnMsg->getPrefixName(), interestRtnMsg->getDataName(),
                                       interestRtnMsg->getVersionName(), interestRtnMsg->getSegmentNum());

    // when no PIT entry exist, silently discard and return
    if (pitEntry == NULL) {
        delete interestRtnMsg;
        return;
    }

    // send interest return to all faces in PIT entry
    for (int i = 0; i < pitEntry->receivedFaces.size(); i++) {
        FaceEntry *faceEntry = pitEntry->receivedFaces[i];

        // create message
        InterestRtnMsg *newInterestRtnMsg = new InterestRtnMsg("InterestRtn");
        newInterestRtnMsg->setReturnCode(interestRtnMsg->getReturnCode());
        newInterestRtnMsg->setPrefixName(interestRtnMsg->getPrefixName());
        newInterestRtnMsg->setDataName(interestRtnMsg->getDataName());
        newInterestRtnMsg->setVersionName(interestRtnMsg->getVersionName());
        newInterestRtnMsg->setSegmentNum(interestRtnMsg->getSegmentNum());
        newInterestRtnMsg->setHeaderSize(interestRtnMsg->getHeaderSize());
        newInterestRtnMsg->setPayloadSize(interestRtnMsg->getPayloadSize());
        newInterestRtnMsg->setByteLength(interestRtnMsg->getByteLength());

        // send
        cGate *sendingGate = gate(faceEntry->outputGateName.c_str(), faceEntry->gateIndex);
        send(newInterestRtnMsg, sendingGate);

    }

    // clear & remove PIT entry
    pitEntry->receivedFaces.clear();
    pit.remove(pitEntry);
    delete pitEntry;

    delete interestRtnMsg;
}

FaceEntry *RFC8569Forwarder::getFaceEntryFromInputGateName(string inputGateName, int gateIndex)
{
    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " get face from gate "  << " \n";

    FaceEntry *faceEntry = NULL;

    list<FaceEntry*>::iterator iteratorFaceEntry = registeredFaces.begin();
    while (iteratorFaceEntry != registeredFaces.end()) {
        faceEntry = *iteratorFaceEntry;
        if (strstr(faceEntry->inputGateName.c_str(), inputGateName.c_str()) != NULL && faceEntry->gateIndex == gateIndex) {
            return faceEntry;
        }
        iteratorFaceEntry++;
    }

    EV_FATAL <<  RFC8569FORWARDER_SIMMODULEINFO << "Something is radically wrong - face entry not found\n";
    throw cRuntimeError("Check log for details");
}

CSEntry *RFC8569Forwarder::getCSEntry(string prefixName, string dataName, string versionName, int segmentNum)
{
    CSEntry *csEntry = NULL;

    list<CSEntry*>::iterator iteratorCSEntry = cs.begin();
    bool found = false;
    while (iteratorCSEntry != cs.end()) {
        csEntry = *iteratorCSEntry;

        if (strcmp(prefixName.c_str(), csEntry->prefixName.c_str()) == 0 && strcmp(dataName.c_str(), csEntry->dataName.c_str()) == 0
                && strcmp(versionName.c_str(), csEntry->versionName.c_str()) == 0 && segmentNum == csEntry->segmentNum) {
            found = true;
            break;
        }
        iteratorCSEntry++;
    }

    if (found) {
        return csEntry;
    } else {
        return NULL;
    }
}

PITEntry *RFC8569Forwarder::getPITEntry(string prefixName, string dataName, string versionName, int segmentNum)
{
    EV_INFO << RFC8569FORWARDER_SIMMODULEINFO << " get PIT entry "  << " \n";

    PITEntry *pitEntry = NULL;

    list<PITEntry*>::iterator iteratorPITEntry = pit.begin();
    while (iteratorPITEntry != pit.end()) {
        pitEntry = *iteratorPITEntry;
        if (strcmp(prefixName.c_str(), pitEntry->prefixName.c_str()) == 0 && strcmp(dataName.c_str(), pitEntry->dataName.c_str()) == 0
                 && strcmp(versionName.c_str(), pitEntry->versionName.c_str()) == 0 && segmentNum == pitEntry->segmentNum) {
            return pitEntry;

        }
        iteratorPITEntry++;
    }

    return pitEntry;
}

// TODO: re-implement a proper LPM
FIBEntry *RFC8569Forwarder::longestPrefixMatchingInFIB(string prefixName)
{
    FIBEntry *fibEntry, *defaultFIBEntry = NULL;

    // search for FIB entry for given prefix
    list<FIBEntry*>::iterator iteratorFIBEntry = fib.begin();
    while (iteratorFIBEntry != fib.end()) {
        fibEntry = *iteratorFIBEntry;
        if (strcmp(fibEntry->prefixName.c_str(), prefixName.c_str()) == 0) {
            return fibEntry;
        } else if (strcmp(fibEntry->prefixName.c_str(), "default") == 0) {
            defaultFIBEntry = fibEntry;
        }
        iteratorFIBEntry++;
    }

    // return the default entry
    return defaultFIBEntry;
}


void RFC8569Forwarder::finish()
{
    // remove fib
    // remove pit
    // remove faces
    // remove cs
}

