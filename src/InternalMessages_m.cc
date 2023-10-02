//
// Generated file, do not edit! Created by opp_msgtool 6.0 from InternalMessages.msg.
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
#include "InternalMessages_m.h"

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

Register_Class(AppRegistrationMsg)

AppRegistrationMsg::AppRegistrationMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

AppRegistrationMsg::AppRegistrationMsg(const AppRegistrationMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

AppRegistrationMsg::~AppRegistrationMsg()
{
    delete [] this->hostedPrefixNames;
}

AppRegistrationMsg& AppRegistrationMsg::operator=(const AppRegistrationMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void AppRegistrationMsg::copy(const AppRegistrationMsg& other)
{
    this->appID = other.appID;
    this->contentServerApp = other.contentServerApp;
    delete [] this->hostedPrefixNames;
    this->hostedPrefixNames = (other.hostedPrefixNames_arraysize==0) ? nullptr : new omnetpp::opp_string[other.hostedPrefixNames_arraysize];
    hostedPrefixNames_arraysize = other.hostedPrefixNames_arraysize;
    for (size_t i = 0; i < hostedPrefixNames_arraysize; i++) {
        this->hostedPrefixNames[i] = other.hostedPrefixNames[i];
    }
    this->appDescription = other.appDescription;
}

void AppRegistrationMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->appID);
    doParsimPacking(b,this->contentServerApp);
    b->pack(hostedPrefixNames_arraysize);
    doParsimArrayPacking(b,this->hostedPrefixNames,hostedPrefixNames_arraysize);
    doParsimPacking(b,this->appDescription);
}

void AppRegistrationMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->appID);
    doParsimUnpacking(b,this->contentServerApp);
    delete [] this->hostedPrefixNames;
    b->unpack(hostedPrefixNames_arraysize);
    if (hostedPrefixNames_arraysize == 0) {
        this->hostedPrefixNames = nullptr;
    } else {
        this->hostedPrefixNames = new omnetpp::opp_string[hostedPrefixNames_arraysize];
        doParsimArrayUnpacking(b,this->hostedPrefixNames,hostedPrefixNames_arraysize);
    }
    doParsimUnpacking(b,this->appDescription);
}

long AppRegistrationMsg::getAppID() const
{
    return this->appID;
}

void AppRegistrationMsg::setAppID(long appID)
{
    this->appID = appID;
}

bool AppRegistrationMsg::getContentServerApp() const
{
    return this->contentServerApp;
}

void AppRegistrationMsg::setContentServerApp(bool contentServerApp)
{
    this->contentServerApp = contentServerApp;
}

size_t AppRegistrationMsg::getHostedPrefixNamesArraySize() const
{
    return hostedPrefixNames_arraysize;
}

const char * AppRegistrationMsg::getHostedPrefixNames(size_t k) const
{
    if (k >= hostedPrefixNames_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)hostedPrefixNames_arraysize, (unsigned long)k);
    return this->hostedPrefixNames[k].c_str();
}

void AppRegistrationMsg::setHostedPrefixNamesArraySize(size_t newSize)
{
    omnetpp::opp_string *hostedPrefixNames2 = (newSize==0) ? nullptr : new omnetpp::opp_string[newSize];
    size_t minSize = hostedPrefixNames_arraysize < newSize ? hostedPrefixNames_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        hostedPrefixNames2[i] = this->hostedPrefixNames[i];
    delete [] this->hostedPrefixNames;
    this->hostedPrefixNames = hostedPrefixNames2;
    hostedPrefixNames_arraysize = newSize;
}

void AppRegistrationMsg::setHostedPrefixNames(size_t k, const char * hostedPrefixNames)
{
    if (k >= hostedPrefixNames_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)hostedPrefixNames_arraysize, (unsigned long)k);
    this->hostedPrefixNames[k] = hostedPrefixNames;
}

void AppRegistrationMsg::insertHostedPrefixNames(size_t k, const char * hostedPrefixNames)
{
    if (k > hostedPrefixNames_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)hostedPrefixNames_arraysize, (unsigned long)k);
    size_t newSize = hostedPrefixNames_arraysize + 1;
    omnetpp::opp_string *hostedPrefixNames2 = new omnetpp::opp_string[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        hostedPrefixNames2[i] = this->hostedPrefixNames[i];
    hostedPrefixNames2[k] = hostedPrefixNames;
    for (i = k + 1; i < newSize; i++)
        hostedPrefixNames2[i] = this->hostedPrefixNames[i-1];
    delete [] this->hostedPrefixNames;
    this->hostedPrefixNames = hostedPrefixNames2;
    hostedPrefixNames_arraysize = newSize;
}

void AppRegistrationMsg::appendHostedPrefixNames(const char * hostedPrefixNames)
{
    insertHostedPrefixNames(hostedPrefixNames_arraysize, hostedPrefixNames);
}

void AppRegistrationMsg::eraseHostedPrefixNames(size_t k)
{
    if (k >= hostedPrefixNames_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)hostedPrefixNames_arraysize, (unsigned long)k);
    size_t newSize = hostedPrefixNames_arraysize - 1;
    omnetpp::opp_string *hostedPrefixNames2 = (newSize == 0) ? nullptr : new omnetpp::opp_string[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        hostedPrefixNames2[i] = this->hostedPrefixNames[i];
    for (i = k; i < newSize; i++)
        hostedPrefixNames2[i] = this->hostedPrefixNames[i+1];
    delete [] this->hostedPrefixNames;
    this->hostedPrefixNames = hostedPrefixNames2;
    hostedPrefixNames_arraysize = newSize;
}

const char * AppRegistrationMsg::getAppDescription() const
{
    return this->appDescription.c_str();
}

void AppRegistrationMsg::setAppDescription(const char * appDescription)
{
    this->appDescription = appDescription;
}

class AppRegistrationMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_appID,
        FIELD_contentServerApp,
        FIELD_hostedPrefixNames,
        FIELD_appDescription,
    };
  public:
    AppRegistrationMsgDescriptor();
    virtual ~AppRegistrationMsgDescriptor();

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

Register_ClassDescriptor(AppRegistrationMsgDescriptor)

AppRegistrationMsgDescriptor::AppRegistrationMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(AppRegistrationMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

AppRegistrationMsgDescriptor::~AppRegistrationMsgDescriptor()
{
    delete[] propertyNames;
}

bool AppRegistrationMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AppRegistrationMsg *>(obj)!=nullptr;
}

const char **AppRegistrationMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *AppRegistrationMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int AppRegistrationMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int AppRegistrationMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_appID
        FD_ISEDITABLE,    // FIELD_contentServerApp
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_hostedPrefixNames
        FD_ISEDITABLE,    // FIELD_appDescription
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *AppRegistrationMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "appID",
        "contentServerApp",
        "hostedPrefixNames",
        "appDescription",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int AppRegistrationMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "appID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "contentServerApp") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "hostedPrefixNames") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "appDescription") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *AppRegistrationMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "long",    // FIELD_appID
        "bool",    // FIELD_contentServerApp
        "string",    // FIELD_hostedPrefixNames
        "string",    // FIELD_appDescription
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **AppRegistrationMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *AppRegistrationMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int AppRegistrationMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hostedPrefixNames: return pp->getHostedPrefixNamesArraySize();
        default: return 0;
    }
}

void AppRegistrationMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hostedPrefixNames: pp->setHostedPrefixNamesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'AppRegistrationMsg'", field);
    }
}

const char *AppRegistrationMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string AppRegistrationMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_appID: return long2string(pp->getAppID());
        case FIELD_contentServerApp: return bool2string(pp->getContentServerApp());
        case FIELD_hostedPrefixNames: return oppstring2string(pp->getHostedPrefixNames(i));
        case FIELD_appDescription: return oppstring2string(pp->getAppDescription());
        default: return "";
    }
}

void AppRegistrationMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_appID: pp->setAppID(string2long(value)); break;
        case FIELD_contentServerApp: pp->setContentServerApp(string2bool(value)); break;
        case FIELD_hostedPrefixNames: pp->setHostedPrefixNames(i,(value)); break;
        case FIELD_appDescription: pp->setAppDescription((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AppRegistrationMsg'", field);
    }
}

omnetpp::cValue AppRegistrationMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_appID: return (omnetpp::intval_t)(pp->getAppID());
        case FIELD_contentServerApp: return pp->getContentServerApp();
        case FIELD_hostedPrefixNames: return pp->getHostedPrefixNames(i);
        case FIELD_appDescription: return pp->getAppDescription();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'AppRegistrationMsg' as cValue -- field index out of range?", field);
    }
}

void AppRegistrationMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_appID: pp->setAppID(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_contentServerApp: pp->setContentServerApp(value.boolValue()); break;
        case FIELD_hostedPrefixNames: pp->setHostedPrefixNames(i,value.stringValue()); break;
        case FIELD_appDescription: pp->setAppDescription(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AppRegistrationMsg'", field);
    }
}

const char *AppRegistrationMsgDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr AppRegistrationMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void AppRegistrationMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    AppRegistrationMsg *pp = omnetpp::fromAnyPtr<AppRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AppRegistrationMsg'", field);
    }
}

Register_Class(TransportRegistrationMsg)

TransportRegistrationMsg::TransportRegistrationMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

TransportRegistrationMsg::TransportRegistrationMsg(const TransportRegistrationMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

TransportRegistrationMsg::~TransportRegistrationMsg()
{
}

TransportRegistrationMsg& TransportRegistrationMsg::operator=(const TransportRegistrationMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void TransportRegistrationMsg::copy(const TransportRegistrationMsg& other)
{
    this->transportID = other.transportID;
    this->transportDescription = other.transportDescription;
    this->transportAddress = other.transportAddress;
}

void TransportRegistrationMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->transportID);
    doParsimPacking(b,this->transportDescription);
    doParsimPacking(b,this->transportAddress);
}

void TransportRegistrationMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->transportID);
    doParsimUnpacking(b,this->transportDescription);
    doParsimUnpacking(b,this->transportAddress);
}

long TransportRegistrationMsg::getTransportID() const
{
    return this->transportID;
}

void TransportRegistrationMsg::setTransportID(long transportID)
{
    this->transportID = transportID;
}

const char * TransportRegistrationMsg::getTransportDescription() const
{
    return this->transportDescription.c_str();
}

void TransportRegistrationMsg::setTransportDescription(const char * transportDescription)
{
    this->transportDescription = transportDescription;
}

const char * TransportRegistrationMsg::getTransportAddress() const
{
    return this->transportAddress.c_str();
}

void TransportRegistrationMsg::setTransportAddress(const char * transportAddress)
{
    this->transportAddress = transportAddress;
}

class TransportRegistrationMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_transportID,
        FIELD_transportDescription,
        FIELD_transportAddress,
    };
  public:
    TransportRegistrationMsgDescriptor();
    virtual ~TransportRegistrationMsgDescriptor();

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

Register_ClassDescriptor(TransportRegistrationMsgDescriptor)

TransportRegistrationMsgDescriptor::TransportRegistrationMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(TransportRegistrationMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

TransportRegistrationMsgDescriptor::~TransportRegistrationMsgDescriptor()
{
    delete[] propertyNames;
}

bool TransportRegistrationMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TransportRegistrationMsg *>(obj)!=nullptr;
}

const char **TransportRegistrationMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *TransportRegistrationMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int TransportRegistrationMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int TransportRegistrationMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_transportID
        FD_ISEDITABLE,    // FIELD_transportDescription
        FD_ISEDITABLE,    // FIELD_transportAddress
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *TransportRegistrationMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "transportID",
        "transportDescription",
        "transportAddress",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int TransportRegistrationMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "transportID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "transportDescription") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "transportAddress") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *TransportRegistrationMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "long",    // FIELD_transportID
        "string",    // FIELD_transportDescription
        "string",    // FIELD_transportAddress
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **TransportRegistrationMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *TransportRegistrationMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int TransportRegistrationMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void TransportRegistrationMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TransportRegistrationMsg'", field);
    }
}

const char *TransportRegistrationMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TransportRegistrationMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: return long2string(pp->getTransportID());
        case FIELD_transportDescription: return oppstring2string(pp->getTransportDescription());
        case FIELD_transportAddress: return oppstring2string(pp->getTransportAddress());
        default: return "";
    }
}

void TransportRegistrationMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: pp->setTransportID(string2long(value)); break;
        case FIELD_transportDescription: pp->setTransportDescription((value)); break;
        case FIELD_transportAddress: pp->setTransportAddress((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TransportRegistrationMsg'", field);
    }
}

omnetpp::cValue TransportRegistrationMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: return (omnetpp::intval_t)(pp->getTransportID());
        case FIELD_transportDescription: return pp->getTransportDescription();
        case FIELD_transportAddress: return pp->getTransportAddress();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'TransportRegistrationMsg' as cValue -- field index out of range?", field);
    }
}

void TransportRegistrationMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: pp->setTransportID(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_transportDescription: pp->setTransportDescription(value.stringValue()); break;
        case FIELD_transportAddress: pp->setTransportAddress(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TransportRegistrationMsg'", field);
    }
}

const char *TransportRegistrationMsgDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr TransportRegistrationMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void TransportRegistrationMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    TransportRegistrationMsg *pp = omnetpp::fromAnyPtr<TransportRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TransportRegistrationMsg'", field);
    }
}

Register_Class(PrefixRegistrationMsg)

PrefixRegistrationMsg::PrefixRegistrationMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

PrefixRegistrationMsg::PrefixRegistrationMsg(const PrefixRegistrationMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

PrefixRegistrationMsg::~PrefixRegistrationMsg()
{
}

PrefixRegistrationMsg& PrefixRegistrationMsg::operator=(const PrefixRegistrationMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void PrefixRegistrationMsg::copy(const PrefixRegistrationMsg& other)
{
    this->prefixName = other.prefixName;
    this->faceID = other.faceID;
}

void PrefixRegistrationMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->prefixName);
    doParsimPacking(b,this->faceID);
}

void PrefixRegistrationMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->prefixName);
    doParsimUnpacking(b,this->faceID);
}

const char * PrefixRegistrationMsg::getPrefixName() const
{
    return this->prefixName.c_str();
}

void PrefixRegistrationMsg::setPrefixName(const char * prefixName)
{
    this->prefixName = prefixName;
}

long PrefixRegistrationMsg::getFaceID() const
{
    return this->faceID;
}

void PrefixRegistrationMsg::setFaceID(long faceID)
{
    this->faceID = faceID;
}

class PrefixRegistrationMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_prefixName,
        FIELD_faceID,
    };
  public:
    PrefixRegistrationMsgDescriptor();
    virtual ~PrefixRegistrationMsgDescriptor();

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

Register_ClassDescriptor(PrefixRegistrationMsgDescriptor)

PrefixRegistrationMsgDescriptor::PrefixRegistrationMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(PrefixRegistrationMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

PrefixRegistrationMsgDescriptor::~PrefixRegistrationMsgDescriptor()
{
    delete[] propertyNames;
}

bool PrefixRegistrationMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrefixRegistrationMsg *>(obj)!=nullptr;
}

const char **PrefixRegistrationMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PrefixRegistrationMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PrefixRegistrationMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int PrefixRegistrationMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_prefixName
        FD_ISEDITABLE,    // FIELD_faceID
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *PrefixRegistrationMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "prefixName",
        "faceID",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int PrefixRegistrationMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "prefixName") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "faceID") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *PrefixRegistrationMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_prefixName
        "long",    // FIELD_faceID
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **PrefixRegistrationMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *PrefixRegistrationMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int PrefixRegistrationMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void PrefixRegistrationMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PrefixRegistrationMsg'", field);
    }
}

const char *PrefixRegistrationMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PrefixRegistrationMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_prefixName: return oppstring2string(pp->getPrefixName());
        case FIELD_faceID: return long2string(pp->getFaceID());
        default: return "";
    }
}

void PrefixRegistrationMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_prefixName: pp->setPrefixName((value)); break;
        case FIELD_faceID: pp->setFaceID(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrefixRegistrationMsg'", field);
    }
}

omnetpp::cValue PrefixRegistrationMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_prefixName: return pp->getPrefixName();
        case FIELD_faceID: return (omnetpp::intval_t)(pp->getFaceID());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PrefixRegistrationMsg' as cValue -- field index out of range?", field);
    }
}

void PrefixRegistrationMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        case FIELD_prefixName: pp->setPrefixName(value.stringValue()); break;
        case FIELD_faceID: pp->setFaceID(omnetpp::checked_int_cast<long>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrefixRegistrationMsg'", field);
    }
}

const char *PrefixRegistrationMsgDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr PrefixRegistrationMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PrefixRegistrationMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PrefixRegistrationMsg *pp = omnetpp::fromAnyPtr<PrefixRegistrationMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PrefixRegistrationMsg'", field);
    }
}

Register_Class(NeighbourListMsg)

NeighbourListMsg::NeighbourListMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

NeighbourListMsg::NeighbourListMsg(const NeighbourListMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

NeighbourListMsg::~NeighbourListMsg()
{
    delete [] this->neighbourAddressList;
}

NeighbourListMsg& NeighbourListMsg::operator=(const NeighbourListMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void NeighbourListMsg::copy(const NeighbourListMsg& other)
{
    this->transportID = other.transportID;
    delete [] this->neighbourAddressList;
    this->neighbourAddressList = (other.neighbourAddressList_arraysize==0) ? nullptr : new omnetpp::opp_string[other.neighbourAddressList_arraysize];
    neighbourAddressList_arraysize = other.neighbourAddressList_arraysize;
    for (size_t i = 0; i < neighbourAddressList_arraysize; i++) {
        this->neighbourAddressList[i] = other.neighbourAddressList[i];
    }
}

void NeighbourListMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->transportID);
    b->pack(neighbourAddressList_arraysize);
    doParsimArrayPacking(b,this->neighbourAddressList,neighbourAddressList_arraysize);
}

void NeighbourListMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->transportID);
    delete [] this->neighbourAddressList;
    b->unpack(neighbourAddressList_arraysize);
    if (neighbourAddressList_arraysize == 0) {
        this->neighbourAddressList = nullptr;
    } else {
        this->neighbourAddressList = new omnetpp::opp_string[neighbourAddressList_arraysize];
        doParsimArrayUnpacking(b,this->neighbourAddressList,neighbourAddressList_arraysize);
    }
}

long NeighbourListMsg::getTransportID() const
{
    return this->transportID;
}

void NeighbourListMsg::setTransportID(long transportID)
{
    this->transportID = transportID;
}

size_t NeighbourListMsg::getNeighbourAddressListArraySize() const
{
    return neighbourAddressList_arraysize;
}

const char * NeighbourListMsg::getNeighbourAddressList(size_t k) const
{
    if (k >= neighbourAddressList_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)neighbourAddressList_arraysize, (unsigned long)k);
    return this->neighbourAddressList[k].c_str();
}

void NeighbourListMsg::setNeighbourAddressListArraySize(size_t newSize)
{
    omnetpp::opp_string *neighbourAddressList2 = (newSize==0) ? nullptr : new omnetpp::opp_string[newSize];
    size_t minSize = neighbourAddressList_arraysize < newSize ? neighbourAddressList_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        neighbourAddressList2[i] = this->neighbourAddressList[i];
    delete [] this->neighbourAddressList;
    this->neighbourAddressList = neighbourAddressList2;
    neighbourAddressList_arraysize = newSize;
}

void NeighbourListMsg::setNeighbourAddressList(size_t k, const char * neighbourAddressList)
{
    if (k >= neighbourAddressList_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)neighbourAddressList_arraysize, (unsigned long)k);
    this->neighbourAddressList[k] = neighbourAddressList;
}

void NeighbourListMsg::insertNeighbourAddressList(size_t k, const char * neighbourAddressList)
{
    if (k > neighbourAddressList_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)neighbourAddressList_arraysize, (unsigned long)k);
    size_t newSize = neighbourAddressList_arraysize + 1;
    omnetpp::opp_string *neighbourAddressList2 = new omnetpp::opp_string[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        neighbourAddressList2[i] = this->neighbourAddressList[i];
    neighbourAddressList2[k] = neighbourAddressList;
    for (i = k + 1; i < newSize; i++)
        neighbourAddressList2[i] = this->neighbourAddressList[i-1];
    delete [] this->neighbourAddressList;
    this->neighbourAddressList = neighbourAddressList2;
    neighbourAddressList_arraysize = newSize;
}

void NeighbourListMsg::appendNeighbourAddressList(const char * neighbourAddressList)
{
    insertNeighbourAddressList(neighbourAddressList_arraysize, neighbourAddressList);
}

void NeighbourListMsg::eraseNeighbourAddressList(size_t k)
{
    if (k >= neighbourAddressList_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)neighbourAddressList_arraysize, (unsigned long)k);
    size_t newSize = neighbourAddressList_arraysize - 1;
    omnetpp::opp_string *neighbourAddressList2 = (newSize == 0) ? nullptr : new omnetpp::opp_string[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        neighbourAddressList2[i] = this->neighbourAddressList[i];
    for (i = k; i < newSize; i++)
        neighbourAddressList2[i] = this->neighbourAddressList[i+1];
    delete [] this->neighbourAddressList;
    this->neighbourAddressList = neighbourAddressList2;
    neighbourAddressList_arraysize = newSize;
}

class NeighbourListMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_transportID,
        FIELD_neighbourAddressList,
    };
  public:
    NeighbourListMsgDescriptor();
    virtual ~NeighbourListMsgDescriptor();

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

Register_ClassDescriptor(NeighbourListMsgDescriptor)

NeighbourListMsgDescriptor::NeighbourListMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(NeighbourListMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

NeighbourListMsgDescriptor::~NeighbourListMsgDescriptor()
{
    delete[] propertyNames;
}

bool NeighbourListMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NeighbourListMsg *>(obj)!=nullptr;
}

const char **NeighbourListMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *NeighbourListMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int NeighbourListMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int NeighbourListMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_transportID
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_neighbourAddressList
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *NeighbourListMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "transportID",
        "neighbourAddressList",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int NeighbourListMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "transportID") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "neighbourAddressList") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *NeighbourListMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "long",    // FIELD_transportID
        "string",    // FIELD_neighbourAddressList
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **NeighbourListMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *NeighbourListMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int NeighbourListMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        case FIELD_neighbourAddressList: return pp->getNeighbourAddressListArraySize();
        default: return 0;
    }
}

void NeighbourListMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        case FIELD_neighbourAddressList: pp->setNeighbourAddressListArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'NeighbourListMsg'", field);
    }
}

const char *NeighbourListMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NeighbourListMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: return long2string(pp->getTransportID());
        case FIELD_neighbourAddressList: return oppstring2string(pp->getNeighbourAddressList(i));
        default: return "";
    }
}

void NeighbourListMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: pp->setTransportID(string2long(value)); break;
        case FIELD_neighbourAddressList: pp->setNeighbourAddressList(i,(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'NeighbourListMsg'", field);
    }
}

omnetpp::cValue NeighbourListMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: return (omnetpp::intval_t)(pp->getTransportID());
        case FIELD_neighbourAddressList: return pp->getNeighbourAddressList(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'NeighbourListMsg' as cValue -- field index out of range?", field);
    }
}

void NeighbourListMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        case FIELD_transportID: pp->setTransportID(omnetpp::checked_int_cast<long>(value.intValue())); break;
        case FIELD_neighbourAddressList: pp->setNeighbourAddressList(i,value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'NeighbourListMsg'", field);
    }
}

const char *NeighbourListMsgDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr NeighbourListMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void NeighbourListMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    NeighbourListMsg *pp = omnetpp::fromAnyPtr<NeighbourListMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'NeighbourListMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

