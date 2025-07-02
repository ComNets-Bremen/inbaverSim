//
// Generated file, do not edit! Created by opp_msgtool 6.1 from RFC8609Messages.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "RFC8609Messages_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(InterestMsg)

InterestMsg::InterestMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

InterestMsg::InterestMsg(const InterestMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

InterestMsg::~InterestMsg()
{
}

InterestMsg& InterestMsg::operator=(const InterestMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void InterestMsg::copy(const InterestMsg& other)
{
    this->hopLimit = other.hopLimit;
    this->lifetime = other.lifetime;
    this->prefixName = other.prefixName;
    this->dataName = other.dataName;
    this->versionName = other.versionName;
    this->segmentNum = other.segmentNum;
    this->headerSize = other.headerSize;
    this->payloadSize = other.payloadSize;
    this->hopsTravelled = other.hopsTravelled;
    this->arrivalFaceID = other.arrivalFaceID;
    this->arrivalFaceType = other.arrivalFaceType;
}

void InterestMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->hopLimit);
    doParsimPacking(b,this->lifetime);
    doParsimPacking(b,this->prefixName);
    doParsimPacking(b,this->dataName);
    doParsimPacking(b,this->versionName);
    doParsimPacking(b,this->segmentNum);
    doParsimPacking(b,this->headerSize);
    doParsimPacking(b,this->payloadSize);
    doParsimPacking(b,this->hopsTravelled);
    doParsimPacking(b,this->arrivalFaceID);
    doParsimPacking(b,this->arrivalFaceType);
}

void InterestMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->hopLimit);
    doParsimUnpacking(b,this->lifetime);
    doParsimUnpacking(b,this->prefixName);
    doParsimUnpacking(b,this->dataName);
    doParsimUnpacking(b,this->versionName);
    doParsimUnpacking(b,this->segmentNum);
    doParsimUnpacking(b,this->headerSize);
    doParsimUnpacking(b,this->payloadSize);
    doParsimUnpacking(b,this->hopsTravelled);
    doParsimUnpacking(b,this->arrivalFaceID);
    doParsimUnpacking(b,this->arrivalFaceType);
}

int InterestMsg::getHopLimit() const
{
    return this->hopLimit;
}

void InterestMsg::setHopLimit(int hopLimit)
{
    this->hopLimit = hopLimit;
}

omnetpp::simtime_t InterestMsg::getLifetime() const
{
    return this->lifetime;
}

void InterestMsg::setLifetime(omnetpp::simtime_t lifetime)
{
    this->lifetime = lifetime;
}

const char * InterestMsg::getPrefixName() const
{
    return this->prefixName.c_str();
}

void InterestMsg::setPrefixName(const char * prefixName)
{
    this->prefixName = prefixName;
}

const char * InterestMsg::getDataName() const
{
    return this->dataName.c_str();
}

void InterestMsg::setDataName(const char * dataName)
{
    this->dataName = dataName;
}

const char * InterestMsg::getVersionName() const
{
    return this->versionName.c_str();
}

void InterestMsg::setVersionName(const char * versionName)
{
    this->versionName = versionName;
}

int InterestMsg::getSegmentNum() const
{
    return this->segmentNum;
}

void InterestMsg::setSegmentNum(int segmentNum)
{
    this->segmentNum = segmentNum;
}

int InterestMsg::getHeaderSize() const
{
    return this->headerSize;
}

void InterestMsg::setHeaderSize(int headerSize)
{
    this->headerSize = headerSize;
}

int InterestMsg::getPayloadSize() const
{
    return this->payloadSize;
}

void InterestMsg::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

int InterestMsg::getHopsTravelled() const
{
    return this->hopsTravelled;
}

void InterestMsg::setHopsTravelled(int hopsTravelled)
{
    this->hopsTravelled = hopsTravelled;
}

long InterestMsg::getArrivalFaceID() const
{
    return this->arrivalFaceID;
}

void InterestMsg::setArrivalFaceID(long arrivalFaceID)
{
    this->arrivalFaceID = arrivalFaceID;
}

int InterestMsg::getArrivalFaceType() const
{
    return this->arrivalFaceType;
}

void InterestMsg::setArrivalFaceType(int arrivalFaceType)
{
    this->arrivalFaceType = arrivalFaceType;
}

class InterestMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_hopLimit,
        FIELD_lifetime,
        FIELD_prefixName,
        FIELD_dataName,
        FIELD_versionName,
        FIELD_segmentNum,
        FIELD_headerSize,
        FIELD_payloadSize,
        FIELD_hopsTravelled,
        FIELD_arrivalFaceID,
        FIELD_arrivalFaceType,
    };
  public:
    InterestMsgDescriptor();
    virtual ~InterestMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(InterestMsgDescriptor)

InterestMsgDescriptor::InterestMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(InterestMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

InterestMsgDescriptor::~InterestMsgDescriptor()
{
    delete[] propertyNames;
}

bool InterestMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<InterestMsg *>(obj)!=nullptr;
}

const char **InterestMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *InterestMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int InterestMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 11+base->getFieldCount() : 11;
}

unsigned int InterestMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_hopLimit
        FD_ISEDITABLE,    // FIELD_lifetime
        FD_ISEDITABLE,    // FIELD_prefixName
        FD_ISEDITABLE,    // FIELD_dataName
        FD_ISEDITABLE,    // FIELD_versionName
        FD_ISEDITABLE,    // FIELD_segmentNum
        FD_ISEDITABLE,    // FIELD_headerSize
        FD_ISEDITABLE,    // FIELD_payloadSize
        FD_ISEDITABLE,    // FIELD_hopsTravelled
        FD_ISEDITABLE,    // FIELD_arrivalFaceID
        FD_ISEDITABLE,    // FIELD_arrivalFaceType
    };
    return (field >= 0 && field < 11) ? fieldTypeFlags[field] : 0;
}

const char *InterestMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "hopLimit",
        "lifetime",
        "prefixName",
        "dataName",
        "versionName",
        "segmentNum",
        "headerSize",
        "payloadSize",
        "hopsTravelled",
        "arrivalFaceID",
        "arrivalFaceType",
    };
    return (field >= 0 && field < 11) ? fieldNames[field] : nullptr;
}

int InterestMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "hopLimit") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "lifetime") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "prefixName") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "dataName") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "versionName") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "segmentNum") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "headerSize") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "payloadSize") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "hopsTravelled") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "arrivalFaceID") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "arrivalFaceType") == 0) return baseIndex + 10;
    return base ? base->findField(fieldName) : -1;
}

const char *InterestMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_hopLimit
        "omnetpp::simtime_t",    // FIELD_lifetime
        "string",    // FIELD_prefixName
        "string",    // FIELD_dataName
        "string",    // FIELD_versionName
        "int",    // FIELD_segmentNum
        "int",    // FIELD_headerSize
        "int",    // FIELD_payloadSize
        "int",    // FIELD_hopsTravelled
        "long",    // FIELD_arrivalFaceID
        "int",    // FIELD_arrivalFaceType
    };
    return (field >= 0 && field < 11) ? fieldTypeStrings[field] : nullptr;
}

const char **InterestMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *InterestMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int InterestMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void InterestMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'InterestMsg'", field);
    }
}

const char *InterestMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string InterestMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hopLimit: return long2string(pp->getHopLimit());
        case FIELD_lifetime: return simtime2string(pp->getLifetime());
        case FIELD_prefixName: return oppstring2string(pp->getPrefixName());
        case FIELD_dataName: return oppstring2string(pp->getDataName());
        case FIELD_versionName: return oppstring2string(pp->getVersionName());
        case FIELD_segmentNum: return long2string(pp->getSegmentNum());
        case FIELD_headerSize: return long2string(pp->getHeaderSize());
        case FIELD_payloadSize: return long2string(pp->getPayloadSize());
        case FIELD_hopsTravelled: return long2string(pp->getHopsTravelled());
        case FIELD_arrivalFaceID: return long2string(pp->getArrivalFaceID());
        case FIELD_arrivalFaceType: return long2string(pp->getArrivalFaceType());
        default: return "";
    }
}

void InterestMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hopLimit: pp->setHopLimit(string2long(value)); break;
        case FIELD_lifetime: pp->setLifetime(string2simtime(value)); break;
        case FIELD_prefixName: pp->setPrefixName((value)); break;
        case FIELD_dataName: pp->setDataName((value)); break;
        case FIELD_versionName: pp->setVersionName((value)); break;
        case FIELD_segmentNum: pp->setSegmentNum(string2long(value)); break;
        case FIELD_headerSize: pp->setHeaderSize(string2long(value)); break;
        case FIELD_payloadSize: pp->setPayloadSize(string2long(value)); break;
        case FIELD_hopsTravelled: pp->setHopsTravelled(string2long(value)); break;
        case FIELD_arrivalFaceID: pp->setArrivalFaceID(string2long(value)); break;
        case FIELD_arrivalFaceType: pp->setArrivalFaceType(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterestMsg'", field);
    }
}

omnetpp::cValue InterestMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hopLimit: return pp->getHopLimit();
        case FIELD_lifetime: return pp->getLifetime().dbl();
        case FIELD_prefixName: return pp->getPrefixName();
        case FIELD_dataName: return pp->getDataName();
        case FIELD_versionName: return pp->getVersionName();
        case FIELD_segmentNum: return pp->getSegmentNum();
        case FIELD_headerSize: return pp->getHeaderSize();
        case FIELD_payloadSize: return pp->getPayloadSize();
        case FIELD_hopsTravelled: return pp->getHopsTravelled();
        case FIELD_arrivalFaceID: return (omnetpp::intval_t)(pp->getArrivalFaceID());
        case FIELD_arrivalFaceType: return pp->getArrivalFaceType();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'InterestMsg' as cValue -- field index out of range?", field);
    }
}

void InterestMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hopLimit: pp->setHopLimit(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_lifetime: pp->setLifetime(value.doubleValue()); break;
        case FIELD_prefixName: pp->setPrefixName(value.stringValue()); break;
        case FIELD_dataName: pp->setDataName(value.stringValue()); break;
        case FIELD_versionName: pp->setVersionName(value.stringValue()); break;
        case FIELD_segmentNum: pp->setSegmentNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_headerSize: pp->setHeaderSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_payloadSize: pp->setPayloadSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopsTravelled: pp->setHopsTravelled(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_arrivalFaceID: pp->setArrivalFaceID(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_arrivalFaceType: pp->setArrivalFaceType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterestMsg'", field);
    }
}

const char *InterestMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr InterestMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void InterestMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestMsg *pp = omnetpp::fromAnyPtr<InterestMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterestMsg'", field);
    }
}

Register_Class(ContentObjMsg)

ContentObjMsg::ContentObjMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

ContentObjMsg::ContentObjMsg(const ContentObjMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

ContentObjMsg::~ContentObjMsg()
{
}

ContentObjMsg& ContentObjMsg::operator=(const ContentObjMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ContentObjMsg::copy(const ContentObjMsg& other)
{
    this->cachetime = other.cachetime;
    this->prefixName = other.prefixName;
    this->dataName = other.dataName;
    this->versionName = other.versionName;
    this->segmentNum = other.segmentNum;
    this->expirytime = other.expirytime;
    this->headerSize = other.headerSize;
    this->payloadSize = other.payloadSize;
    this->totalNumSegments = other.totalNumSegments;
    this->payloadAsString = other.payloadAsString;
}

void ContentObjMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->cachetime);
    doParsimPacking(b,this->prefixName);
    doParsimPacking(b,this->dataName);
    doParsimPacking(b,this->versionName);
    doParsimPacking(b,this->segmentNum);
    doParsimPacking(b,this->expirytime);
    doParsimPacking(b,this->headerSize);
    doParsimPacking(b,this->payloadSize);
    doParsimPacking(b,this->totalNumSegments);
    doParsimPacking(b,this->payloadAsString);
}

void ContentObjMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->cachetime);
    doParsimUnpacking(b,this->prefixName);
    doParsimUnpacking(b,this->dataName);
    doParsimUnpacking(b,this->versionName);
    doParsimUnpacking(b,this->segmentNum);
    doParsimUnpacking(b,this->expirytime);
    doParsimUnpacking(b,this->headerSize);
    doParsimUnpacking(b,this->payloadSize);
    doParsimUnpacking(b,this->totalNumSegments);
    doParsimUnpacking(b,this->payloadAsString);
}

omnetpp::simtime_t ContentObjMsg::getCachetime() const
{
    return this->cachetime;
}

void ContentObjMsg::setCachetime(omnetpp::simtime_t cachetime)
{
    this->cachetime = cachetime;
}

const char * ContentObjMsg::getPrefixName() const
{
    return this->prefixName.c_str();
}

void ContentObjMsg::setPrefixName(const char * prefixName)
{
    this->prefixName = prefixName;
}

const char * ContentObjMsg::getDataName() const
{
    return this->dataName.c_str();
}

void ContentObjMsg::setDataName(const char * dataName)
{
    this->dataName = dataName;
}

const char * ContentObjMsg::getVersionName() const
{
    return this->versionName.c_str();
}

void ContentObjMsg::setVersionName(const char * versionName)
{
    this->versionName = versionName;
}

int ContentObjMsg::getSegmentNum() const
{
    return this->segmentNum;
}

void ContentObjMsg::setSegmentNum(int segmentNum)
{
    this->segmentNum = segmentNum;
}

omnetpp::simtime_t ContentObjMsg::getExpirytime() const
{
    return this->expirytime;
}

void ContentObjMsg::setExpirytime(omnetpp::simtime_t expirytime)
{
    this->expirytime = expirytime;
}

int ContentObjMsg::getHeaderSize() const
{
    return this->headerSize;
}

void ContentObjMsg::setHeaderSize(int headerSize)
{
    this->headerSize = headerSize;
}

int ContentObjMsg::getPayloadSize() const
{
    return this->payloadSize;
}

void ContentObjMsg::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

int ContentObjMsg::getTotalNumSegments() const
{
    return this->totalNumSegments;
}

void ContentObjMsg::setTotalNumSegments(int totalNumSegments)
{
    this->totalNumSegments = totalNumSegments;
}

const char * ContentObjMsg::getPayloadAsString() const
{
    return this->payloadAsString.c_str();
}

void ContentObjMsg::setPayloadAsString(const char * payloadAsString)
{
    this->payloadAsString = payloadAsString;
}

class ContentObjMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_cachetime,
        FIELD_prefixName,
        FIELD_dataName,
        FIELD_versionName,
        FIELD_segmentNum,
        FIELD_expirytime,
        FIELD_headerSize,
        FIELD_payloadSize,
        FIELD_totalNumSegments,
        FIELD_payloadAsString,
    };
  public:
    ContentObjMsgDescriptor();
    virtual ~ContentObjMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ContentObjMsgDescriptor)

ContentObjMsgDescriptor::ContentObjMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ContentObjMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

ContentObjMsgDescriptor::~ContentObjMsgDescriptor()
{
    delete[] propertyNames;
}

bool ContentObjMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ContentObjMsg *>(obj)!=nullptr;
}

const char **ContentObjMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ContentObjMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ContentObjMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int ContentObjMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_cachetime
        FD_ISEDITABLE,    // FIELD_prefixName
        FD_ISEDITABLE,    // FIELD_dataName
        FD_ISEDITABLE,    // FIELD_versionName
        FD_ISEDITABLE,    // FIELD_segmentNum
        FD_ISEDITABLE,    // FIELD_expirytime
        FD_ISEDITABLE,    // FIELD_headerSize
        FD_ISEDITABLE,    // FIELD_payloadSize
        FD_ISEDITABLE,    // FIELD_totalNumSegments
        FD_ISEDITABLE,    // FIELD_payloadAsString
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *ContentObjMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "cachetime",
        "prefixName",
        "dataName",
        "versionName",
        "segmentNum",
        "expirytime",
        "headerSize",
        "payloadSize",
        "totalNumSegments",
        "payloadAsString",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int ContentObjMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "cachetime") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "prefixName") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "dataName") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "versionName") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "segmentNum") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "expirytime") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "headerSize") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "payloadSize") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "totalNumSegments") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "payloadAsString") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *ContentObjMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "omnetpp::simtime_t",    // FIELD_cachetime
        "string",    // FIELD_prefixName
        "string",    // FIELD_dataName
        "string",    // FIELD_versionName
        "int",    // FIELD_segmentNum
        "omnetpp::simtime_t",    // FIELD_expirytime
        "int",    // FIELD_headerSize
        "int",    // FIELD_payloadSize
        "int",    // FIELD_totalNumSegments
        "string",    // FIELD_payloadAsString
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **ContentObjMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ContentObjMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ContentObjMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ContentObjMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ContentObjMsg'", field);
    }
}

const char *ContentObjMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ContentObjMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        case FIELD_cachetime: return simtime2string(pp->getCachetime());
        case FIELD_prefixName: return oppstring2string(pp->getPrefixName());
        case FIELD_dataName: return oppstring2string(pp->getDataName());
        case FIELD_versionName: return oppstring2string(pp->getVersionName());
        case FIELD_segmentNum: return long2string(pp->getSegmentNum());
        case FIELD_expirytime: return simtime2string(pp->getExpirytime());
        case FIELD_headerSize: return long2string(pp->getHeaderSize());
        case FIELD_payloadSize: return long2string(pp->getPayloadSize());
        case FIELD_totalNumSegments: return long2string(pp->getTotalNumSegments());
        case FIELD_payloadAsString: return oppstring2string(pp->getPayloadAsString());
        default: return "";
    }
}

void ContentObjMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        case FIELD_cachetime: pp->setCachetime(string2simtime(value)); break;
        case FIELD_prefixName: pp->setPrefixName((value)); break;
        case FIELD_dataName: pp->setDataName((value)); break;
        case FIELD_versionName: pp->setVersionName((value)); break;
        case FIELD_segmentNum: pp->setSegmentNum(string2long(value)); break;
        case FIELD_expirytime: pp->setExpirytime(string2simtime(value)); break;
        case FIELD_headerSize: pp->setHeaderSize(string2long(value)); break;
        case FIELD_payloadSize: pp->setPayloadSize(string2long(value)); break;
        case FIELD_totalNumSegments: pp->setTotalNumSegments(string2long(value)); break;
        case FIELD_payloadAsString: pp->setPayloadAsString((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ContentObjMsg'", field);
    }
}

omnetpp::cValue ContentObjMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        case FIELD_cachetime: return pp->getCachetime().dbl();
        case FIELD_prefixName: return pp->getPrefixName();
        case FIELD_dataName: return pp->getDataName();
        case FIELD_versionName: return pp->getVersionName();
        case FIELD_segmentNum: return pp->getSegmentNum();
        case FIELD_expirytime: return pp->getExpirytime().dbl();
        case FIELD_headerSize: return pp->getHeaderSize();
        case FIELD_payloadSize: return pp->getPayloadSize();
        case FIELD_totalNumSegments: return pp->getTotalNumSegments();
        case FIELD_payloadAsString: return pp->getPayloadAsString();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ContentObjMsg' as cValue -- field index out of range?", field);
    }
}

void ContentObjMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        case FIELD_cachetime: pp->setCachetime(value.doubleValue()); break;
        case FIELD_prefixName: pp->setPrefixName(value.stringValue()); break;
        case FIELD_dataName: pp->setDataName(value.stringValue()); break;
        case FIELD_versionName: pp->setVersionName(value.stringValue()); break;
        case FIELD_segmentNum: pp->setSegmentNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_expirytime: pp->setExpirytime(value.doubleValue()); break;
        case FIELD_headerSize: pp->setHeaderSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_payloadSize: pp->setPayloadSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_totalNumSegments: pp->setTotalNumSegments(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_payloadAsString: pp->setPayloadAsString(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ContentObjMsg'", field);
    }
}

const char *ContentObjMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ContentObjMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ContentObjMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ContentObjMsg *pp = omnetpp::fromAnyPtr<ContentObjMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ContentObjMsg'", field);
    }
}

Register_Enum(ReturnCodeTypes, (ReturnCodeTypes::ReturnCodeTypeNoRoute, ReturnCodeTypes::ReturnCodeTypeHopLimitExceeded, ReturnCodeTypes::ReturnCodeTypeMTUTooLarge, ReturnCodeTypes::ReturnCodeTypeNoResources, ReturnCodeTypes::ReturnCodeTypePathError, ReturnCodeTypes::ReturnCodeTypeProhibited, ReturnCodeTypes::ReturnCodeTypeCongestion, ReturnCodeTypes::ReturnCodeTypeReturnCodeTypeUnsupportedHash, ReturnCodeTypes::ReturnCodeTypeMalformedInterest));

Register_Class(InterestRtnMsg)

InterestRtnMsg::InterestRtnMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

InterestRtnMsg::InterestRtnMsg(const InterestRtnMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

InterestRtnMsg::~InterestRtnMsg()
{
}

InterestRtnMsg& InterestRtnMsg::operator=(const InterestRtnMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void InterestRtnMsg::copy(const InterestRtnMsg& other)
{
    this->returnCode = other.returnCode;
    this->prefixName = other.prefixName;
    this->dataName = other.dataName;
    this->versionName = other.versionName;
    this->segmentNum = other.segmentNum;
    this->headerSize = other.headerSize;
    this->payloadSize = other.payloadSize;
}

void InterestRtnMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->returnCode);
    doParsimPacking(b,this->prefixName);
    doParsimPacking(b,this->dataName);
    doParsimPacking(b,this->versionName);
    doParsimPacking(b,this->segmentNum);
    doParsimPacking(b,this->headerSize);
    doParsimPacking(b,this->payloadSize);
}

void InterestRtnMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->returnCode);
    doParsimUnpacking(b,this->prefixName);
    doParsimUnpacking(b,this->dataName);
    doParsimUnpacking(b,this->versionName);
    doParsimUnpacking(b,this->segmentNum);
    doParsimUnpacking(b,this->headerSize);
    doParsimUnpacking(b,this->payloadSize);
}

int InterestRtnMsg::getReturnCode() const
{
    return this->returnCode;
}

void InterestRtnMsg::setReturnCode(int returnCode)
{
    this->returnCode = returnCode;
}

const char * InterestRtnMsg::getPrefixName() const
{
    return this->prefixName.c_str();
}

void InterestRtnMsg::setPrefixName(const char * prefixName)
{
    this->prefixName = prefixName;
}

const char * InterestRtnMsg::getDataName() const
{
    return this->dataName.c_str();
}

void InterestRtnMsg::setDataName(const char * dataName)
{
    this->dataName = dataName;
}

const char * InterestRtnMsg::getVersionName() const
{
    return this->versionName.c_str();
}

void InterestRtnMsg::setVersionName(const char * versionName)
{
    this->versionName = versionName;
}

int InterestRtnMsg::getSegmentNum() const
{
    return this->segmentNum;
}

void InterestRtnMsg::setSegmentNum(int segmentNum)
{
    this->segmentNum = segmentNum;
}

int InterestRtnMsg::getHeaderSize() const
{
    return this->headerSize;
}

void InterestRtnMsg::setHeaderSize(int headerSize)
{
    this->headerSize = headerSize;
}

int InterestRtnMsg::getPayloadSize() const
{
    return this->payloadSize;
}

void InterestRtnMsg::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

class InterestRtnMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_returnCode,
        FIELD_prefixName,
        FIELD_dataName,
        FIELD_versionName,
        FIELD_segmentNum,
        FIELD_headerSize,
        FIELD_payloadSize,
    };
  public:
    InterestRtnMsgDescriptor();
    virtual ~InterestRtnMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(InterestRtnMsgDescriptor)

InterestRtnMsgDescriptor::InterestRtnMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(InterestRtnMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

InterestRtnMsgDescriptor::~InterestRtnMsgDescriptor()
{
    delete[] propertyNames;
}

bool InterestRtnMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<InterestRtnMsg *>(obj)!=nullptr;
}

const char **InterestRtnMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *InterestRtnMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int InterestRtnMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 7+base->getFieldCount() : 7;
}

unsigned int InterestRtnMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_returnCode
        FD_ISEDITABLE,    // FIELD_prefixName
        FD_ISEDITABLE,    // FIELD_dataName
        FD_ISEDITABLE,    // FIELD_versionName
        FD_ISEDITABLE,    // FIELD_segmentNum
        FD_ISEDITABLE,    // FIELD_headerSize
        FD_ISEDITABLE,    // FIELD_payloadSize
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *InterestRtnMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "returnCode",
        "prefixName",
        "dataName",
        "versionName",
        "segmentNum",
        "headerSize",
        "payloadSize",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int InterestRtnMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "returnCode") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "prefixName") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "dataName") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "versionName") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "segmentNum") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "headerSize") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "payloadSize") == 0) return baseIndex + 6;
    return base ? base->findField(fieldName) : -1;
}

const char *InterestRtnMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_returnCode
        "string",    // FIELD_prefixName
        "string",    // FIELD_dataName
        "string",    // FIELD_versionName
        "int",    // FIELD_segmentNum
        "int",    // FIELD_headerSize
        "int",    // FIELD_payloadSize
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **InterestRtnMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_returnCode: {
            static const char *names[] = { "enum", "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *InterestRtnMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_returnCode:
            if (!strcmp(propertyName, "enum")) return "ReturnCodeTypes";
            if (!strcmp(propertyName, "enum")) return "ReturnCodeTypes";
            return nullptr;
        default: return nullptr;
    }
}

int InterestRtnMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void InterestRtnMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'InterestRtnMsg'", field);
    }
}

const char *InterestRtnMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string InterestRtnMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        case FIELD_returnCode: return enum2string(pp->getReturnCode(), "ReturnCodeTypes");
        case FIELD_prefixName: return oppstring2string(pp->getPrefixName());
        case FIELD_dataName: return oppstring2string(pp->getDataName());
        case FIELD_versionName: return oppstring2string(pp->getVersionName());
        case FIELD_segmentNum: return long2string(pp->getSegmentNum());
        case FIELD_headerSize: return long2string(pp->getHeaderSize());
        case FIELD_payloadSize: return long2string(pp->getPayloadSize());
        default: return "";
    }
}

void InterestRtnMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        case FIELD_returnCode: pp->setReturnCode((ReturnCodeTypes)string2enum(value, "ReturnCodeTypes")); break;
        case FIELD_prefixName: pp->setPrefixName((value)); break;
        case FIELD_dataName: pp->setDataName((value)); break;
        case FIELD_versionName: pp->setVersionName((value)); break;
        case FIELD_segmentNum: pp->setSegmentNum(string2long(value)); break;
        case FIELD_headerSize: pp->setHeaderSize(string2long(value)); break;
        case FIELD_payloadSize: pp->setPayloadSize(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterestRtnMsg'", field);
    }
}

omnetpp::cValue InterestRtnMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        case FIELD_returnCode: return pp->getReturnCode();
        case FIELD_prefixName: return pp->getPrefixName();
        case FIELD_dataName: return pp->getDataName();
        case FIELD_versionName: return pp->getVersionName();
        case FIELD_segmentNum: return pp->getSegmentNum();
        case FIELD_headerSize: return pp->getHeaderSize();
        case FIELD_payloadSize: return pp->getPayloadSize();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'InterestRtnMsg' as cValue -- field index out of range?", field);
    }
}

void InterestRtnMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        case FIELD_returnCode: pp->setReturnCode((ReturnCodeTypes)value.intValue()); break;
        case FIELD_prefixName: pp->setPrefixName(value.stringValue()); break;
        case FIELD_dataName: pp->setDataName(value.stringValue()); break;
        case FIELD_versionName: pp->setVersionName(value.stringValue()); break;
        case FIELD_segmentNum: pp->setSegmentNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_headerSize: pp->setHeaderSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_payloadSize: pp->setPayloadSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterestRtnMsg'", field);
    }
}

const char *InterestRtnMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr InterestRtnMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void InterestRtnMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    InterestRtnMsg *pp = omnetpp::fromAnyPtr<InterestRtnMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterestRtnMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

