//
// Copyright (C) 2021 Asanga Udugama (adu@comnets.uni-bremen.de)
//
// SPDX-License-Identifier: GPL-3.0-or-later
//


#include "RFC8569Forwarder.h"

Define_Module(RFC8569Forwarder);

void RFC8569Forwarder::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        maximumContentStoreSize = par("maximumContentStoreSize");

    } else if (stage == 1) {

        // get Demiurge model instance
        demiurgeModel = NULL;
        for (int id = 0; id <= getSimulation()->getLastComponentId(); id++) {
            cModule *unknownModel = getSimulation()->getModule(id);
            if (unknownModel == NULL) {
                continue;
            }
            if (dynamic_cast<Demiurge*>(unknownModel) != NULL) {
                demiurgeModel = dynamic_cast<Demiurge*>(unknownModel);
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

        // when Demiurge or Numen not found, terminate
        if (demiurgeModel == NULL || numenModel == NULL) {
            EV_FATAL << simTime() << "The global Demiurge instance and/or node specific Numen instance not found.\n";
            throw cRuntimeError("Check log for details");
        }

        currentCSSize = 0;
        hitCount = 0;
        missCount = 0;

    } else if (stage == 2) {

        // init stat signals
        totalInterestsBytesReceivedSignal = registerSignal("fwdTotalInterestsBytesReceived");
        totalInterestRtnsBytesReceivedSignal = registerSignal("fwdTotalInterestRtnsBytesReceived");
        totalContentObjsBytesReceivedSignal = registerSignal("fwdTotalContentObjsBytesReceived");
        totalInterestsBytesSentSignal = registerSignal("fwdTotalInterestsBytesSent");
        totalInterestRtnsBytesSentSignal = registerSignal("fwdTotalInterestRtnsBytesSent");
        totalContentObjsBytesSentSignal = registerSignal("fwdTotalContentObjsBytesSent");
        cacheSizeBytesSignal = registerSignal("fwdCacheSizeBytes");
        cacheAdditionsBytesSignal = registerSignal("fwdCacheAdditionsBytes");
        cacheRemovalsBytesSignal = registerSignal("fwdCacheRemovalsBytes");
        fibEntryCountSignal = registerSignal("fwdFIBEntryCount");
        pitEntryCountSignal = registerSignal("fwdPITEntryCount");
        cacheHitRatioSignal = registerSignal("fwdCacheHitRatio");
        cacheMissRatioSignal = registerSignal("fwdCacheMissRatio");
        networkCacheHitRatioSignal = registerSignal("fwdNetworkCacheHitRatio");
        networkCacheMissRatioSignal = registerSignal("fwdNetworkCacheMissRatio");

    } else {
        EV_FATAL << simTime() << "Something is radically wrong\n";
        throw cRuntimeError("Check log for details");
    }

}

void RFC8569Forwarder::handleMessage(cMessage *msg)
{
    cGate *arrivalGate;
    char gateName[32];
    AppRegistrationMsg *appRegMsg = NULL;
    PrefixRegistrationMsg *prefixRegMsg = NULL;
    TransportRegistrationMsg *transportRegMsg = NULL;
    InterestMsg *interestMsg = NULL;
    ContentObjMsg *contentObjMsg = NULL;
    InterestRtnMsg *interestRtnMsg = NULL;
    NeighbourListMsg *neighbourListMsg;

    // get arrival gate details
    arrivalGate = msg->getArrivalGate();
    strcpy(gateName, arrivalGate->getName());

    // self messages
    if (msg->isSelfMessage()) {
        delete msg;

    } else {

        // handle app registration msg from app
        if (strstr(gateName, "appInOut") != NULL && (appRegMsg = dynamic_cast<AppRegistrationMsg*>(msg)) != NULL) {

            processApplicationRegistration(appRegMsg);

        // register a new prefix requested by an application
        } else if (strstr(gateName, "appInOut") != NULL && (prefixRegMsg = dynamic_cast<PrefixRegistrationMsg*>(msg)) != NULL) {

                processPrefixRegistration(prefixRegMsg);

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

        // process neighbour list msg when transport is in direct mode
        } else if ((neighbourListMsg = dynamic_cast<NeighbourListMsg*>(msg)) != NULL) {

            // TODO: handle neighbour list messages

            delete msg;

        } else {

            EV_INFO << simTime() << "Received unexpected packet"
                    << endl;
            delete msg;
        }
    }

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
    snprintf(gateName, sizeof(gateName), "%s$o", arrivalGate->getBaseName());
    faceEntry->outputGateName = string(gateName);
    if (arrivalGate->isVector()) {
        faceEntry->gateIndex = arrivalGate->getIndex();
    } else {
        faceEntry->gateIndex = -1;
    }
    registeredFaces.push_back(faceEntry);

    EV_INFO << simTime() << " Received application registration: "
            << appRegMsg->getAppID()
            << endl;

    // when app hosts prefixes, add to FIB and setup for late dissemination
    if (appRegMsg->getContentServerApp()) {
        for (int i = 0; i < appRegMsg->getHostedPrefixNamesArraySize(); i++) {
            string prefixName = appRegMsg->getHostedPrefixNames(i);
            updateFIB(prefixName, faceEntry);

        }
    }

    delete appRegMsg;
}

void RFC8569Forwarder::processPrefixRegistration(PrefixRegistrationMsg *prefixRegMsg)
{
    FaceEntry *faceEntry = getFaceEntryFromFaceID(prefixRegMsg->getFaceID());

    updateFIB(prefixRegMsg->getPrefixName(), faceEntry);

    delete prefixRegMsg;
}

void RFC8569Forwarder::processTransportRegistration(TransportRegistrationMsg *transportRegMsg)
{
    cGate *arrivalGate = transportRegMsg->getArrivalGate();
    char gateName[48];

    // create face entry
    FaceEntry *faceEntry = new FaceEntry;
    faceEntry->faceID = transportRegMsg->getTransportID();
    faceEntry->faceType = TransportTypeFace;
    faceEntry->faceDescription = transportRegMsg->getTransportDescription();
    faceEntry->transportAddress = transportRegMsg->getTransportAddress();
    faceEntry->inputGateName = string(arrivalGate->getName());
    faceEntry->baseGateName = string(arrivalGate->getBaseName());
    snprintf(gateName, sizeof(gateName), "%s$o", arrivalGate->getBaseName());
    faceEntry->outputGateName = string(gateName);
    if (arrivalGate->isVector()) {
        faceEntry->gateIndex = arrivalGate->getIndex();
    } else {
        faceEntry->gateIndex = -1;
    }
    registeredFaces.push_back(faceEntry);

    EV_INFO << simTime() << " Received transport registration: "
            << transportRegMsg->getTransportID()
            << endl;

    // append face to the default FIB entry (ccnx://)
    updateFIB("ccnx://", faceEntry);

    delete transportRegMsg;
}

void RFC8569Forwarder::processInterest(InterestMsg *interestMsg)
{

//    cout << simTime() << " " << getFullPath()
//            << " interest received: "
//            << interestMsg->getPrefixName() << " "
//            << interestMsg->getDataName() << endl;

    // get arrival gate details
    cGate *arrivalGate = interestMsg->getArrivalGate();
    int arrivalGateIndex =  (arrivalGate->isVector() ? arrivalGate->getIndex() : (-1));
    FaceEntry *arrivalFaceEntry = getFaceEntryFromInputGateName(arrivalGate->getName(), arrivalGateIndex);

    EV_INFO << simTime() << " Received Interest: "
            << arrivalFaceEntry->faceID
            << " " << interestMsg->getPrefixName()
            << " " << interestMsg->getDataName()
            << " " << interestMsg->getVersionName()
            << " " << interestMsg->getSegmentNum()
            << endl;

    dumpFaces();

    // generate stats
    if (arrivalFaceEntry->faceType == TransportTypeFace) {
        emit(totalInterestsBytesReceivedSignal, (long) interestMsg->getByteLength());
    }

    // check and get Face and transport address info of sender of Interest,
    ExchangedTransportInfo *arrivalTransportInfo = NULL;
    if (interestMsg->hasObject("ExchangedTransportInfo")) {
        arrivalTransportInfo = check_and_cast<ExchangedTransportInfo*>(interestMsg->getObject("ExchangedTransportInfo"));
        interestMsg->removeObject("ExchangedTransportInfo");
    }


    // lookup in CS for the requested Content Obj
    CSEntry *csEntry = getCSEntry(interestMsg->getPrefixName(), interestMsg->getDataName(),
                                    interestMsg->getVersionName(), interestMsg->getSegmentNum());

    // when Content Obj is in CS, send it to the Interest sender
    if (csEntry != NULL) {

        // generate hit stats
        hitCount++;
        emit(cacheHitRatioSignal, (double) hitCount / (hitCount + missCount));
        emit(cacheMissRatioSignal, (double) missCount / (hitCount + missCount));
        demiurgeModel->incrementNetworkCacheHitCount();
        emit(networkCacheHitRatioSignal, demiurgeModel->getNetworkCacheHitRatio());
        emit(networkCacheMissRatioSignal, demiurgeModel->getNetworkCacheMissRatio());

        // make content obj msg from cache entry
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

        // add the destination transport detail, if available
        if (arrivalTransportInfo != NULL) {
            contentObjMsg->addObject(arrivalTransportInfo);
        }

        EV_INFO << simTime() << " Sending ContentObj from Cache: "
                << csEntry->prefixName
                << " " << csEntry->dataName
                << " " << csEntry->versionName
                << " " << csEntry->segmentNum
                << " " << csEntry->expirytime
                << " " << csEntry->payloadSize
                << " " << csEntry->totalNumSegments
                << endl;

        // send content obj
        cGate *sendingGate = gate(arrivalFaceEntry->outputGateName.c_str(), arrivalFaceEntry->gateIndex);
        send(contentObjMsg, sendingGate);

        // generate stats
        if (arrivalFaceEntry->faceType == TransportTypeFace) {
            emit(totalContentObjsBytesSentSignal, (long) contentObjMsg->getByteLength());
        }

        // remove Interest
        delete interestMsg;
        return;

    } else {

        // generate miss stat
        missCount++;
        emit(cacheHitRatioSignal, (double) hitCount / (hitCount + missCount));
        emit(cacheMissRatioSignal, (double) missCount / (hitCount + missCount));
        demiurgeModel->incrementNetworkCacheMissCount();
        emit(networkCacheHitRatioSignal, demiurgeModel->getNetworkCacheHitRatio());
        emit(networkCacheMissRatioSignal, demiurgeModel->getNetworkCacheMissRatio());

    }


    // is there a PIT entry already for previous Interests received
    // for same content?
    PITEntry *pitEntry = getPITEntry(interestMsg->getPrefixName(), interestMsg->getDataName(),
                                    interestMsg->getVersionName(), interestMsg->getSegmentNum());

    dumpPIT();



    // when there is already a PIT entry, means previous Interests were
    // received, so add the current Interest to the PIT entry
    if (pitEntry != NULL) {

        // Check if the same Interest was received through the same Face and transport address
        bool found = false;
        for (int i = 0; i < pitEntry->arrivalInfoList.size(); i++) {
            if (pitEntry->arrivalInfoList[i]->receivedFace->faceID == arrivalFaceEntry->faceID) {
                if (arrivalTransportInfo != NULL) {
                    if (arrivalTransportInfo->transportAddress == pitEntry->arrivalInfoList[i]->receivedFace->transportAddress) {
                        found = true;
                        break;
                    }
                } else {
                    found = true;
                    break;
                }
            }
        }

        // when same Interest was not received from same Face and same transport address
        // then add it to PIT entry
        if (!found) {
            ArrivalInfo *arrivalInfo = new ArrivalInfo();
            arrivalInfo->receivedFace = arrivalFaceEntry;
            if (arrivalTransportInfo != NULL) {
                arrivalInfo->transportAddress = arrivalTransportInfo->transportAddress;
            } else {
                arrivalInfo->transportAddress = "";
            }

            EV_INFO << simTime() << "PIT entry exists. Adding new arrival Face: "
                    << pitEntry->prefixName
                    << " " << pitEntry->dataName
                    << " " << pitEntry->versionName
                    << " " << pitEntry->segmentNum
                    << " " << pitEntry->hopLimit
                    << " " << pitEntry->hopsTravelled
                    << " " << arrivalFaceEntry->faceID
                    << " " << arrivalInfo->transportAddress
                    << endl;

            pitEntry->arrivalInfoList.push_back(arrivalInfo);
        }

        // discard Interest
        delete interestMsg;
        return;
    }

    // discard interest if it has reached the maximum hop count
    // only if they arrive from transport type faces (not applications)
    if (arrivalFaceEntry->faceType == TransportTypeFace && (interestMsg->getHopLimit() - 1) <= 0) {

        EV_INFO << simTime() << "Hop limit exceeded. Discarding Interest: "
                << interestMsg->getHopLimit()
                << " " << interestMsg->getHopsTravelled()
                << " " << arrivalFaceEntry->faceDescription
                << endl;

        delete interestMsg;
        return;
    }



    // save Interest in PIT - create new PIT entry
    pitEntry = new PITEntry;
    pitEntry->prefixName = interestMsg->getPrefixName();
    pitEntry->dataName = interestMsg->getDataName();
    pitEntry->versionName = interestMsg->getVersionName();
    pitEntry->segmentNum = interestMsg->getSegmentNum();
    pitEntry->hopLimit = interestMsg->getHopLimit() - 1;
    pitEntry->hopsTravelled = interestMsg->getHopsTravelled() + 1;

    ArrivalInfo *arrivalInfo = new ArrivalInfo();
    arrivalInfo->receivedFace = arrivalFaceEntry;
    if (arrivalTransportInfo != NULL) {
        arrivalInfo->transportAddress = arrivalTransportInfo->transportAddress;
    } else {
        arrivalInfo->transportAddress = "";
    }
    pitEntry->arrivalInfoList.push_back(arrivalInfo);

    EV_INFO << simTime() << "Adding PIT entry: "
            << pitEntry->prefixName
            << " " << pitEntry->dataName
            << " " << pitEntry->versionName
            << " " << pitEntry->segmentNum
            << " " << pitEntry->hopLimit
            << " " << pitEntry->hopsTravelled
            << " " << arrivalFaceEntry->faceID
            << " " << arrivalInfo->transportAddress
            << endl;

    pit.push_back(pitEntry);

    // gen stats
    emit(pitEntryCountSignal, (long) pit.size());


    // find which FIB entry to use to forward the Interest
    FIBEntry *fibEntry = longestPrefixMatchingInFIB(interestMsg->getPrefixName());

    EV_INFO << simTime() << "Longest Prefix Matching and found: "
            << fibEntry->prefixName << " " << fibEntry->forwardedFaces.size() << endl;

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

            // set the arrival face, if the interest is sent to an application face
            if (arrivalFaceEntry->faceType == TransportTypeFace
                    && faceEntry->faceType == ApplicationTypeFace) {
                newInterestMsg->setArrivalFaceID(arrivalFaceEntry->faceID);
                newInterestMsg->setArrivalFaceType(TransportTypeFace);
            } else {
                newInterestMsg->setArrivalFaceID(-1);
                newInterestMsg->setArrivalFaceType(-1);
            }

            cGate *sendingGate = gate(faceEntry->outputGateName.c_str(), faceEntry->gateIndex);

            EV_INFO << simTime() << "Sending Interest to Face: "
                    << interestMsg->getPrefixName()
                    << " " << interestMsg->getDataName()
                    << " " << interestMsg->getVersionName()
                    << " " << interestMsg->getSegmentNum()
                    << " " << faceEntry->faceID << endl;

            send(newInterestMsg, sendingGate);

            // generate stats
            if (faceEntry->faceType == TransportTypeFace) {
                emit(totalInterestsBytesSentSignal, (long) newInterestMsg->getByteLength());
            }
        }

        iteratorFaceEntry++;
    }


    // discard Interest
    delete interestMsg;
    return;
}

void RFC8569Forwarder::processContentObj(ContentObjMsg *contentObjMsg)
{
//    cout << simTime() << " " << getFullPath()
//            << " content obj received: "
//            << contentObjMsg->getPrefixName() << " "
//            << contentObjMsg->getDataName() << endl;

    // get arrival gate details
    cGate *arrivalGate = contentObjMsg->getArrivalGate();
    int arrivalGateIndex =  (arrivalGate->isVector() ? arrivalGate->getIndex() : (-1));
    FaceEntry *arrivalFaceEntry = getFaceEntryFromInputGateName(arrivalGate->getName(), arrivalGateIndex);

    EV_INFO << simTime() << "Received ContentObj: "
            << arrivalFaceEntry->faceID
            << " " << contentObjMsg->getPrefixName()
            << " " << contentObjMsg->getDataName()
            << " " << contentObjMsg->getVersionName()
            << " " << contentObjMsg->getSegmentNum()
            << endl;

    // generate stats
    if (arrivalFaceEntry->faceType == TransportTypeFace) {
        emit(totalContentObjsBytesReceivedSignal, (long) contentObjMsg->getByteLength());
    }

    // check if the content obj is already in CS
    CSEntry *csEntry = getCSEntry(contentObjMsg->getPrefixName(), contentObjMsg->getDataName(),
                                    contentObjMsg->getVersionName(), contentObjMsg->getSegmentNum());

    // when the Content Obj is in CS, then there should not be any PIT entry
    // so, simply disregard the Content Obj
    if (csEntry != NULL) {

        EV_INFO << simTime() << "ContentObj already in Cache: "
                << " " << contentObjMsg->getPrefixName()
                << " " << contentObjMsg->getDataName()
                << " " << contentObjMsg->getVersionName()
                << " " << contentObjMsg->getSegmentNum()
                << endl;

        delete contentObjMsg;
        return;
    }

    // before adding content to CS, check if size will exceed the limit
    // when so, remove cache entries until the new content can be added
    if (maximumContentStoreSize > 0) {
        long removedBytes = 0;
        while ((currentCSSize + contentObjMsg->getPayloadSize()) > maximumContentStoreSize) {
            CSEntry *removingCSEntry = cs.front();
            cs.pop_front();
            currentCSSize -= removingCSEntry->payloadSize;
            removedBytes += removingCSEntry->payloadSize;

            EV_INFO << simTime() << "Cache is full, cannot insert, removing: "
                    << " " << removingCSEntry->prefixName
                    << " " << removingCSEntry->dataName
                    << " " << removingCSEntry->versionName
                    << " " << removingCSEntry->segmentNum
                    << " " << removingCSEntry->payloadSize
                    << " " << currentCSSize
                    << endl;

            delete removingCSEntry;
        }

        // generate stats
        if (removedBytes > 0) {
            emit(cacheRemovalsBytesSignal, removedBytes);
            emit(cacheSizeBytesSignal, currentCSSize);
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

    EV_INFO << simTime() << "Added Cache entry: "
            << " " << csEntry->prefixName
            << " " << csEntry->dataName
            << " " << csEntry->versionName
            << " " << csEntry->segmentNum
            << " " << csEntry->payloadSize
            << " " << currentCSSize
            << endl;

    // generate stats
    emit(cacheAdditionsBytesSignal, csEntry->payloadSize);
    emit(cacheSizeBytesSignal, currentCSSize);

    // find the PIT entry, if there is one saved
    PITEntry *pitEntry = getPITEntry(contentObjMsg->getPrefixName(), contentObjMsg->getDataName(),
                            contentObjMsg->getVersionName(), contentObjMsg->getSegmentNum());

    // when there is no PIT entry, simply drop the Content Obj
    // because nobody to forward it to
    if (pitEntry == NULL) {

        EV_INFO << simTime() << "PIT entry not found: "
                    << " " << contentObjMsg->getPrefixName()
                    << " " << contentObjMsg->getDataName()
                    << " " << contentObjMsg->getVersionName()
                    << " " << contentObjMsg->getSegmentNum()
                    << endl;

        delete contentObjMsg;
        return;
    }

    // when the PIT entry exists, send Content Obj to all Interests that were
    // recoded in the PIT (i.e., Faces and transport addresses)
    for (int i = 0; i < pitEntry->arrivalInfoList.size(); i++) {
        ArrivalInfo *arrivalInfo = pitEntry->arrivalInfoList[i];

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

        // add the transport address if it exists
        if (arrivalInfo->transportAddress.size() > 0) {
            ExchangedTransportInfo *arrivalTransportInfo = new ExchangedTransportInfo("ExchangedTransportInfo");
            arrivalTransportInfo->transportAddress = arrivalInfo->transportAddress;
            newContentObjMsg->addObject(arrivalTransportInfo);
        }

        EV_INFO << simTime() << "Sending ContentObj to Face: "
                << newContentObjMsg->getPrefixName()
                << " " << newContentObjMsg->getDataName()
                << " " << newContentObjMsg->getVersionName()
                << " " << newContentObjMsg->getSegmentNum()
                << " " << arrivalInfo->receivedFace->faceID
                << endl;

        // send content obj
        cGate *sendingGate = gate(arrivalInfo->receivedFace->outputGateName.c_str(), arrivalInfo->receivedFace->gateIndex);
        send(newContentObjMsg, sendingGate);

        // generate stats
        if (arrivalInfo->receivedFace->faceType == TransportTypeFace) {
            emit(totalContentObjsBytesSentSignal, (long) newContentObjMsg->getByteLength());
        }
    }

    // remove the PIT entry as it was served
    pitEntry->arrivalInfoList.clear();
    pit.remove(pitEntry);
    delete pitEntry;

    // gen stats
    emit(pitEntryCountSignal, (long) pit.size());

    // remove the Content Obj as it was saved and also sent to
    // the Interest senders
    delete contentObjMsg;
}

void RFC8569Forwarder::processInterestRtn(InterestRtnMsg *interestRtnMsg)
{
    // get arrival gate details
    cGate *arrivalGate = interestRtnMsg->getArrivalGate();
    int arrivalGateIndex =  (arrivalGate->isVector() ? arrivalGate->getIndex() : (-1));
    FaceEntry *arrivalFaceEntry = getFaceEntryFromInputGateName(arrivalGate->getName(), arrivalGateIndex);

    EV_INFO << simTime() << "Received InterestRtn: "
            << arrivalFaceEntry->faceID
            << " " << interestRtnMsg->getPrefixName()
            << " " << interestRtnMsg->getDataName()
            << " " << interestRtnMsg->getVersionName()
            << " " << interestRtnMsg->getSegmentNum()
            << endl;

    // generate stats
    if (arrivalFaceEntry->faceType == TransportTypeFace) {
        emit(totalInterestRtnsBytesReceivedSignal, (long) interestRtnMsg->getByteLength());
    }

    // Is there a PIT entry for the returning interest?
    PITEntry *pitEntry = getPITEntry(interestRtnMsg->getPrefixName(), interestRtnMsg->getDataName(),
                                       interestRtnMsg->getVersionName(), interestRtnMsg->getSegmentNum());

    // when no PIT entry exist, discard Interest
    if (pitEntry == NULL) {

        EV_INFO << simTime() << "PIT entry not found: "
                    << " " << interestRtnMsg->getPrefixName()
                    << " " << interestRtnMsg->getDataName()
                    << " " << interestRtnMsg->getVersionName()
                    << " " << interestRtnMsg->getSegmentNum()
                    << endl;

        delete interestRtnMsg;
        return;
    }

    // when the PIT entry exists, send Interest Return to all Interest senders
    // that were recoded in the PIT (i.e., Faces and transport addresses)
    for (int i = 0; i < pitEntry->arrivalInfoList.size(); i++) {
        ArrivalInfo *arrivalInfo = pitEntry->arrivalInfoList[i];

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

        // add the transport address if it exists
        if (arrivalInfo->transportAddress.size() > 0) {
            ExchangedTransportInfo *arrivalTransportInfo = new ExchangedTransportInfo("ExchangedTransportInfo");
            arrivalTransportInfo->transportAddress = arrivalInfo->transportAddress;
            newInterestRtnMsg->addObject(arrivalTransportInfo);
        }

        EV_INFO << simTime() << "Sending InterestRtn to Face: "
                << interestRtnMsg->getPrefixName()
                << " " << interestRtnMsg->getDataName()
                << " " << interestRtnMsg->getVersionName()
                << " " << interestRtnMsg->getSegmentNum()
                << " " << arrivalInfo->receivedFace->faceID
                << endl;

        // send Interest Return
        cGate *sendingGate = gate(arrivalInfo->receivedFace->outputGateName.c_str(), arrivalInfo->receivedFace->gateIndex);
        send(newInterestRtnMsg, sendingGate);

        // generate stats
        if (arrivalInfo->receivedFace->faceType == TransportTypeFace) {
            emit(totalInterestRtnsBytesSentSignal, (long) newInterestRtnMsg->getByteLength());
        }
    }

    // remove the PIT entry as it was served
    pitEntry->arrivalInfoList.clear();
    pit.remove(pitEntry);
    delete pitEntry;

    // gen stats
    emit(pitEntryCountSignal, (long) pit.size());

    // remove the Interest Return as it was sent to the Interest senders
    delete interestRtnMsg;
}

FaceEntry *RFC8569Forwarder::getFaceEntryFromInputGateName(string inputGateName, int gateIndex)
{
    FaceEntry *faceEntry = NULL;

    list<FaceEntry*>::iterator iteratorFaceEntry = registeredFaces.begin();
    while (iteratorFaceEntry != registeredFaces.end()) {
        faceEntry = *iteratorFaceEntry;
        if (strstr(faceEntry->inputGateName.c_str(), inputGateName.c_str()) != NULL && faceEntry->gateIndex == gateIndex) {
            return faceEntry;
        }
        iteratorFaceEntry++;
    }

    EV_FATAL << simTime() << " Something is radically wrong - face entry not found - gate name: "
            << inputGateName
            << ", index: " << gateIndex
            << endl;
    throw cRuntimeError("Check log for details");
}

FaceEntry *RFC8569Forwarder::getFaceEntryFromFaceID(long faceID)
{
    FaceEntry *faceEntry = NULL;

    list<FaceEntry*>::iterator iteratorFaceEntry = registeredFaces.begin();
    while (iteratorFaceEntry != registeredFaces.end()) {
        faceEntry = *iteratorFaceEntry;
        if (faceEntry->faceID == faceID) {
            return faceEntry;
        }
        iteratorFaceEntry++;
    }

    EV_FATAL << simTime() << " Something is radically wrong - face entry not found - face ID: "
            << faceID
            << endl;
    throw cRuntimeError("Check log for details");
}

CSEntry *RFC8569Forwarder::getCSEntry(string prefixName, string dataName, string versionName, int segmentNum)
{
    list<CSEntry*>::iterator iteratorCSEntry = cs.begin();
    while (iteratorCSEntry != cs.end()) {
        CSEntry *csEntry = *iteratorCSEntry;

        if (strcmp(prefixName.c_str(), csEntry->prefixName.c_str()) == 0 && strcmp(dataName.c_str(), csEntry->dataName.c_str()) == 0
                && strcmp(versionName.c_str(), csEntry->versionName.c_str()) == 0 && segmentNum == csEntry->segmentNum) {
            return csEntry;
        }
        iteratorCSEntry++;
    }

    return NULL;
}

PITEntry *RFC8569Forwarder::getPITEntry(string prefixName, string dataName, string versionName, int segmentNum)
{
    list<PITEntry*>::iterator iteratorPITEntry = pit.begin();
    while (iteratorPITEntry != pit.end()) {
        PITEntry *pitEntry = *iteratorPITEntry;
        if (strcmp(prefixName.c_str(), pitEntry->prefixName.c_str()) == 0 && strcmp(dataName.c_str(), pitEntry->dataName.c_str()) == 0
                 && strcmp(versionName.c_str(), pitEntry->versionName.c_str()) == 0 && segmentNum == pitEntry->segmentNum) {
            return pitEntry;

        }
        iteratorPITEntry++;
    }

    return NULL;
}

FIBEntry *RFC8569Forwarder::updateFIB(string prefixName, FaceEntry *faceEntry)
{
    // NOTE: default FIB entry has the prefix - ccnx://

    FIBEntry *fibEntry = NULL;

    bool found = false;
    list<FIBEntry*>::iterator iteratorFIBEntry = fib.begin();
    while (iteratorFIBEntry != fib.end()) {
        fibEntry = *iteratorFIBEntry;
        if (strcmp(fibEntry->prefixName.c_str(), prefixName.c_str()) == 0) {
            found = true;
            break;
        } else if (strcmp(fibEntry->prefixName.c_str(), prefixName.c_str()) < 0) {
            break;
        }

        iteratorFIBEntry++;
    }

    // if the pointer reached the end of the FIB list
    if (iteratorFIBEntry == fib.end()) {

        // append the FIB entry to the end
        fibEntry = new FIBEntry;
        fibEntry->prefixName = prefixName;
        fibEntry->forwardedFaces.push_back(faceEntry);
        fib.push_back(fibEntry);

    // if a prefix found that is lower than what is to be added
    } else if(!found) {

        // insert the FIB entry before the found entry
        fibEntry = new FIBEntry;
        fibEntry->prefixName = prefixName;
        fibEntry->forwardedFaces.push_back(faceEntry);
        fib.insert((--iteratorFIBEntry), fibEntry);

    // if exact prefix found
    } else if(found) {

        // append the face entry to found FIB entry
        fibEntry->forwardedFaces.push_back(faceEntry);

    }

    EV_INFO << simTime() << " Updated FIB: "
            << prefixName
            << " " << faceEntry->faceID
            << " " << faceEntry->faceType
            << endl;

    // generate stats
    emit(fibEntryCountSignal, (long) fib.size());

    dumpFIB();

    return fibEntry;
}

FIBEntry *RFC8569Forwarder::longestPrefixMatchingInFIB(string prefixName)
{
    // NOTE: default FIB entry (ccnx://) is assumed to be always present

    // create list to hold prefixes to check and append first prefix to check
    vector <string> prefixPartsList;
    prefixPartsList.push_back(prefixName);

    // create part prefixes and append to list
    char *prefixPart = new char[prefixName.size() + 1];
    strcpy(prefixPart, prefixName.c_str());
    for (int i = (prefixName.size() - 1); i >= 0; i--) {
        if (prefixPart[i] == '/') {
            prefixPart[i + 1] = '\0';
            if (strlen(prefixPart) > 0) {
                prefixPartsList.push_back(string(prefixPart));
            }
        }
    }

    EV_INFO << simTime() << " LPM started for " << prefixName << ", checking for following entries \n";
    for (int i = 0; i < prefixPartsList.size(); i++) {
        EV_INFO << simTime() << " " << prefixPartsList[i] << "\n";
    }
    EV_INFO << simTime() << " Current FIB below " << "\n";
    dumpFIB();

    // search for every prefix part in FIB - from longest to shortest
    FIBEntry *fibEntry;
    bool found = false;
    for (int i = 0; i < prefixPartsList.size(); i++) {

        list<FIBEntry*>::iterator iteratorFIBEntry = fib.begin();
        while (iteratorFIBEntry != fib.end()) {
            fibEntry = *iteratorFIBEntry;
            if (strcmp(fibEntry->prefixName.c_str(), prefixPartsList[i].c_str()) == 0) {
                found = true;
                break;
            }
            iteratorFIBEntry++;
        }

        if (found) {
            return fibEntry;
        }
    }

    EV_FATAL << simTime() << "Something is radically wrong - FIB entry not found - "
            << prefixName
            << ", at least default (ccnx://) must be found "
            << endl;
    throw cRuntimeError("Check log for details ---");
}

void RFC8569Forwarder::finish()
{
    // dump info
//    dumpFaces();
//    dumpFIB();
//    dumpPIT();
//    dumpCS();

    // remove fib
    // remove pit
    // remove faces
    // remove cs
}

void RFC8569Forwarder::dumpFIB()
{
    EV_INFO << simTime() << "==FIB===\n";
    list<FIBEntry*>::iterator iteratorFIBEntry = fib.begin();
    while (iteratorFIBEntry != fib.end()) {
        FIBEntry *fibEntry = *iteratorFIBEntry;
        EV_INFO << simTime() << " " << fibEntry->prefixName << "\n";
        list<FaceEntry*>::iterator iteratorFaceEntry = fibEntry->forwardedFaces.begin();
        while (iteratorFaceEntry != fibEntry->forwardedFaces.end()) {
            FaceEntry *faceEntry = *iteratorFaceEntry;
            EV_INFO << simTime() << "   " << faceEntry->faceID
                    << " " << faceEntry->faceDescription << " "
                    << faceEntry->baseGateName << "\n";
            iteratorFaceEntry++;
        }
        iteratorFIBEntry++;
    }
    EV_INFO << simTime() << "=====\n";
}

void RFC8569Forwarder::dumpFaces()
{
    FaceEntry *faceEntry = NULL;

    EV_INFO << simTime() << "==Faces===\n";
    list<FaceEntry*>::iterator iteratorFaceEntry = registeredFaces.begin();
    while (iteratorFaceEntry != registeredFaces.end()) {
        faceEntry = *iteratorFaceEntry;
        EV_INFO << simTime() << " "
                << faceEntry->faceID << " "
                << faceEntry->faceDescription << " "
                << faceEntry->baseGateName << "\n";

        iteratorFaceEntry++;
    }
    EV_INFO << simTime() << "=====\n";
}

void RFC8569Forwarder::dumpCS()
{
    EV_INFO << simTime() << "==CS===\n";
    EV_INFO << simTime() << " cached entries: " << cs.size() << "\n";

//    list<CSEntry*>::iterator iteratorCSEntry = cs.begin();
//    while (iteratorCSEntry != cs.end()) {
//        CSEntry *csEntry = *iteratorCSEntry;
//        EV_INFO << simTime() << " "
//                << csEntry->prefixName << " "
//                << csEntry->dataName << " "
//                << csEntry->versionName << " "
//                << csEntry->segmentNum << "\n";
//        iteratorCSEntry++;
//    }

    EV_INFO << simTime() << "=====\n";
}

void RFC8569Forwarder::dumpPIT()
{
    EV_INFO << simTime() << "==PIT===\n";
    list<PITEntry*>::iterator iteratorPITEntry = pit.begin();
    while (iteratorPITEntry != pit.end()) {
        PITEntry *pitEntry = *iteratorPITEntry;
        EV_INFO << simTime() << " "
                << pitEntry->prefixName << " "
                << pitEntry->dataName << " "
                << pitEntry->versionName << " "
                << pitEntry->segmentNum << "\n";
        for (int i = 0; i < pitEntry->arrivalInfoList.size(); i++) {
            EV_INFO << simTime() << "   arrival face: "
                    << pitEntry->arrivalInfoList[i]->receivedFace->faceID
                    << "\n";
        }
        iteratorPITEntry++;
    }
    EV_INFO << simTime() << "=====\n";
}

