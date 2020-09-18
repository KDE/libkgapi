/*
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "contactsservice.h"
#include "contact.h"
#include "contactsgroup.h"
#include "utils.h"
#include "common_p.h"
#include "../debug.h"

#include <QDomDocument>
#include <QDomElement>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

/* Qt::escape() */
#include <QTextDocument>


namespace KGAPI2
{

namespace ContactsService
{

namespace {

static const QString fieldFeed = QStringLiteral("feed");
static const QString fieldCategory = QStringLiteral("category");
static const QString fieldTerm = QStringLiteral("term");
static const QString fieldEntry = QStringLiteral("entry");
static const QString fieldLink = QStringLiteral("link");
static const QString fieldHref = QStringLiteral("href");
static const QString fieldRel = QStringLiteral("rel");
static const QString fieldNext = QStringLiteral("next");
static const QString fieldDollarT = QStringLiteral("$t");
static const QString fieldOpenSearchTotalResults = QStringLiteral("openSearch$totalResults");
static const QString fieldOpenSearchStartIndex = QStringLiteral("openSearch$startIndex");
static const QString fieldOpenSearchItemsPerPage = QStringLiteral("openSearch$itemsPerPage");

static constexpr QStringView schemaUrlContact = QStringView(u"http://schemas.google.com/contact/2008#contact");
static constexpr QStringView schemaUrlContactGroup = QStringView(u"http://schemas.google.com/contact/2008#group");
static constexpr QStringView schemaUrlGroup = QStringView(u"http://schemas.google.com/g/2008#group");

static const QString queryItemAlt = QStringLiteral("alt");
static const QString queryItemShowdeleted = QStringLiteral("showdeleted");
static const QString queryValueJson = QStringLiteral("json");
static const QString queryValueTrue = QStringLiteral("true");

static const QString propertyKADDRESSBOOK = QStringLiteral("KADDRESSBOOK");
}

namespace Private
{
    QString stringFromXMLMap(const QJsonObject &map, const QString &key) {
        return map[key][fieldDollarT].toString();
    }

    ObjectPtr JSONToContactsGroup(const QJsonObject &map);
    ObjectPtr JSONToContact(const QJsonObject & map);

    static const QUrl GoogleApisUrl(QStringLiteral("https://www.google.com"));
    static const QString ContactsBasePath(QStringLiteral("/m8/feeds/contacts"));
    static const QString ContactsGroupBasePath(QStringLiteral("/m8/feeds/groups"));
    static const QString PhotoBasePath(QStringLiteral("/m8/feeds/photos/media"));

    QByteArray addRelSchema(const QByteArray &rel)
    {
        if (!rel.startsWith("http://schemas.google.com/g/2005#")) {
            return "http://schemas.google.com/g/2005#" + rel;
        } else {
            return rel;
        }
    }
}

ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    ObjectsList output;
    const auto document = QJsonDocument::fromJson(jsonFeed);
    const auto feed = document[fieldFeed].toObject();
    const auto categories = feed.value(fieldCategory).toArray();
    for (const auto &c : categories) {
        const auto category = c.toObject();
        bool groups = false;

        if (category.value(fieldTerm).toString() == schemaUrlContactGroup) {
            groups = true;
        }

        const auto entries = feed.value(fieldEntry).toArray();
        for (const auto &e : entries) {
            if (groups) {
                output << Private::JSONToContactsGroup(e.toObject());
            } else {
                output << Private::JSONToContact(e.toObject());
            }
        }
    }

    const auto links = feed[fieldLink].toArray();
    for (const auto &link : links) {
        if (link[fieldRel].toString() == QLatin1String("next")) {
            feedData.nextPageUrl = QUrl(link[fieldHref].toString());
            break;
        }
    }

    feedData.totalResults = Private::stringFromXMLMap(feed, fieldOpenSearchTotalResults).toInt();
    feedData.startIndex = Private::stringFromXMLMap(feed, fieldOpenSearchStartIndex).toInt();
    feedData.itemsPerPage = Private::stringFromXMLMap(feed, fieldOpenSearchItemsPerPage).toInt();

    return output;
}

QUrl fetchAllContactsUrl(const QString& user, bool showDeleted)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsBasePath % QLatin1Char('/') % user % QLatin1String("/full"));
    QUrlQuery query(url);
    query.addQueryItem(queryItemAlt, queryValueJson);
    if (showDeleted) {
        query.addQueryItem(queryItemShowdeleted, queryValueTrue);
    }
    url.setQuery(query);

    return url;
}

QUrl fetchContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsBasePath % QLatin1Char('/') % user % QLatin1String("/full/") % id);
    QUrlQuery query(url);
    query.addQueryItem(queryItemAlt, queryValueJson);
    url.setQuery(query);
    return url;
}

QUrl createContactUrl(const QString& user)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsBasePath % QLatin1Char('/') % user % QLatin1String("/full"));
    return url;
}

QUrl updateContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsBasePath % QLatin1Char('/') % user % QLatin1String("/full/") % id);
    return url;
}

QUrl removeContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsBasePath % QLatin1Char('/') % user % QLatin1String("/full/") % id);
    return url;
}

QUrl fetchAllGroupsUrl(const QString &user)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsGroupBasePath % QLatin1Char('/') % user % QLatin1String("/full"));
    QUrlQuery query(url);
    query.addQueryItem(queryItemAlt, queryValueJson);
    url.setQuery(query);
    return url;
}

QUrl fetchGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains(QLatin1Char('/'))) {
        id = groupId.mid(groupId.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = groupId;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsGroupBasePath % QLatin1Char('/') % user % QLatin1String("/base/") % id);
    QUrlQuery query(url);
    query.addQueryItem(queryItemAlt, queryValueJson);
    url.setQuery(query);
    return url;
}

QUrl createGroupUrl(const QString &user)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsGroupBasePath % QLatin1Char('/') % user % QLatin1String("/full"));
    return url;
}

QUrl updateGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains(QLatin1Char('/'))) {
        id = groupId.mid(groupId.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = groupId;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsGroupBasePath % QLatin1Char('/') % user % QLatin1String("/full/") % id);
    return url;
}

QUrl removeGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains(QLatin1Char('/'))) {
        id = groupId.mid(groupId.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = groupId;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsGroupBasePath % QLatin1Char('/') % user % QLatin1String("/full/") % id);
    return url;
}

QUrl photoUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains(QLatin1Char('/'))) {
        id = contactID.mid(contactID.lastIndexOf(QLatin1Char('/')) + 1);
    } else {
        id = contactID;
    }

    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::PhotoBasePath % QLatin1Char('/') % user % QLatin1Char('/') % id);
    return url;
}


QString APIVersion()
{
    return QStringLiteral("3.0");
}


/*********************************** PRIVATE *************************************/

namespace {
static const QString fieldId = QStringLiteral("id");
static const QString fieldGdEtag = QStringLiteral("gd$etag");
static const QString fieldTitle = QStringLiteral("title");
static const QString fieldContent = QStringLiteral("content");
static const QString fieldUpdated = QStringLiteral("updated");
static const QString fieldGContactSystemGroup = QStringLiteral("gContact$systemGroup");
}

ObjectPtr Private::JSONToContactsGroup(const QJsonObject &data)
{
    ContactsGroupPtr group(new ContactsGroup);

    group->setId(Private::stringFromXMLMap(data, fieldId));
    group->setEtag(data[fieldGdEtag].toString());

    group->setTitle(Private::stringFromXMLMap(data, fieldTitle));
    group->setContent(Private::stringFromXMLMap(data, fieldContent));

    group->setUpdated(QDateTime::fromString(Private::stringFromXMLMap(data, fieldUpdated), Qt::ISODate));

    group->setIsSystemGroup(data.contains(fieldGContactSystemGroup));

    return group;
}

ContactsGroupPtr JSONToContactsGroup(const QByteArray& jsonData)
{
    const auto document = QJsonDocument::fromJson(jsonData);
    const auto categories = document[fieldEntry][fieldCategory].toArray();

    const bool isGroup = std::any_of(categories.cbegin(), categories.cend(), [](const auto &category) {
        return category[fieldTerm].toString() == schemaUrlContactGroup;
    });

    if (!isGroup) {
        return ContactsGroupPtr();
    }

    return Private::JSONToContactsGroup(document[fieldEntry].toObject()).staticCast<ContactsGroup>();
}

namespace {
static const QString fieldGdDeleted = QStringLiteral("gd$deleted");
static const QString fieldGdName = QStringLiteral("gd$name");
static const QString fieldGdFullName = QStringLiteral("gd$fullName");
static const QString fieldGdGivenName = QStringLiteral("gd$givenName");
static const QString fieldGdFamilyName = QStringLiteral("gd$familyName");
static const QString fieldGdAdditionalName = QStringLiteral("gd$additionalName");
static const QString fieldGdNamePrefix = QStringLiteral("gd$namePrefix");
static const QString fieldGdNameSuffix = QStringLiteral("gd$nameSuffix");
static const QString fieldGdOrganization = QStringLiteral("gd$organization");
static const QString fieldGdOrgName = QStringLiteral("gd$orgName");
static const QString fieldGdOrgDepartment = QStringLiteral("gd$orgDepartment");
static const QString fieldGdOrgTitle = QStringLiteral("gd$orgTitle");
static const QString fieldGdWhere = QStringLiteral("gd$where");
static const QString fieldGContactNickname = QStringLiteral("gContact$nickname");
static const QString fieldGContactOccupation = QStringLiteral("gContact$occupation");
static const QString fieldGContactRelation = QStringLiteral("gContact$relation");
static const QString fieldGContactEvent = QStringLiteral("gContact$event");
static const QString fieldGdWhen = QStringLiteral("gd$when");
static const QString fieldStartTime = QStringLiteral("startTime");
static const QString fieldGContactWebsite = QStringLiteral("gContact$website");
static const QString fieldGdEmail = QStringLiteral("gd$email");
static const QString fieldGdIm = QStringLiteral("gd$im");
static const QString fieldAddress = QStringLiteral("address");
static const QString fieldPrimary = QStringLiteral("primary");
static const QString fieldProtocol = QStringLiteral("protocol");
static const QString fieldGdPhoneNumber = QStringLiteral("gd$phoneNumber");
static const QString fieldGdStructuredPostalAddress = QStringLiteral("gd$structuredPostalAddress");
static const QString fieldGdCity = QStringLiteral("gd$city");
static const QString fieldGdCountry = QStringLiteral("gd$country");
static const QString fieldGdPostcode = QStringLiteral("gd$postcode");
static const QString fieldGdRegion = QStringLiteral("gd$region");
static const QString fieldGdPOBox = QStringLiteral("gd$pobox");
static const QString fieldGdStreet = QStringLiteral("gd$street");
static const QString fieldGContactBirthday = QStringLiteral("gContact$birthday");
static const QString fieldWhen = QStringLiteral("when");
static const QString fieldGContactUserDefinedField = QStringLiteral("gContact$userDefinedField");
static const QString fieldKey = QStringLiteral("key");
static const QString fieldValue = QStringLiteral("value");
static const QString fieldGContactGroupMembershipInfo = QStringLiteral("gContact$groupMembershipInfo");
static const QString fieldDeleted = QStringLiteral("deleted");

static constexpr QStringView relationSpouse = {u"spouse"};
static constexpr QStringView relationManager = {u"manager"};
static constexpr QStringView relationAssistant = {u"assistant"};
static constexpr QStringView eventAnniversary = {u"anniversary"};
static constexpr QStringView websiteHomePage = {u"home-page"};
static constexpr QStringView websiteWork = {u"work"};
static constexpr QStringView websiteProfile = {u"profile"};
static constexpr QStringView websiteBlog = {u"blog"};

static const QString paramType = QStringLiteral("TYPE");
static const QString locatorHome = QStringLiteral("HOME");
static const QString locatorWork = QStringLiteral("WORK");
static const QString locatorProfile = QStringLiteral("PROFILE");

static constexpr QStringView schemaUrlPhoto = {u"http://schemas.google.com/contacts/2008/rel#photo"};
}

namespace Private {

template<typename T, typename Func>
void fromXMLMap(T *obj, Func func, const QJsonObject &data, const QString &entry)
{
    if (data.contains(entry)) {
        (obj->*func)(Private::stringFromXMLMap(data, entry));
    }
}

template<typename Obj>
void fromXMLMapProperties(Obj *o, const QJsonObject &data,
                          std::initializer_list<std::pair<void(Obj::*)(const QString &), const QString &>> properties)
{
    for (const auto &entry : properties) {
        if (data.contains(entry.second)) {
            (o->*(entry.first))(Private::stringFromXMLMap(data, entry.second));
        }
    }
}

} // namespace

ObjectPtr Private::JSONToContact(const QJsonObject& data)
{
    ContactPtr contact(new Contact);

    /* Google contact ID */
    contact->setUid(Private::stringFromXMLMap(data, fieldId));

    /* Google ETAG. This can be used to identify if the item was changed remotely */
    contact->setEtag(data[fieldGdEtag].toString());

    /* Date and time when contact was updated on the remote server */
    contact->setUpdated(QDateTime::fromString(Private::stringFromXMLMap(data, fieldUpdated), Qt::ISODate));

    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    contact->setDeleted(data.contains(fieldGdDeleted));

    /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
     * itself. */
    const auto links = data[fieldLink].toArray();
    for (const auto &link : links) {
        if (link[fieldRel].toString() == schemaUrlPhoto) {
            contact->setPhotoUrl(link[fieldHref].toString());
        }
    }

    /* Name, note, nickname and occupation (=organization/title) */
    Private::fromXMLMapProperties(contact.get(), data, {
            {&Contact::setName, fieldTitle},
            {&Contact::setNote, fieldContent},
            {&Contact::setNickName, fieldGContactNickname},
            {&Contact::setProfession, fieldGContactOccupation}
    });

    /* Formatted name */
    if (data.contains(fieldGdName)) {
        Private::fromXMLMapProperties(contact.get(), data[fieldGdName].toObject(), {
                {&Contact::setFormattedName, fieldGdFullName},
                {&Contact::setGivenName, fieldGdGivenName},
                {&Contact::setFamilyName, fieldGdFamilyName},
                {&Contact::setAdditionalName, fieldGdAdditionalName},
                {&Contact::setPrefix, fieldGdNamePrefix},
                {&Contact::setSuffix, fieldGdNameSuffix}
        });
    }

    /* Organization (work) - KABC supports only one organization */
    if (data.contains(fieldGdOrganization)) {
        Private::fromXMLMapProperties(contact.get(), data[fieldGdOrganization].toArray()[0].toObject(), {
                {&Contact::setOrganization, fieldGdOrgName},
                {&Contact::setDepartment, fieldGdOrgDepartment},
                {&Contact::setTitle, fieldGdOrgTitle},
                {&Contact::setOffice, fieldGdWhere}
        });
    }

    /* Relationships */
    if (data.contains(fieldGContactRelation)) {
        const auto relations = data[fieldGContactRelation].toArray();
        for (const auto &relation : relations) {
            if (relation[fieldRel].toString() == relationSpouse) {
                contact->setSpousesName(relation[fieldDollarT].toString());
            } else if (relation[fieldRel].toString() == relationManager) {
                contact->setManagersName(relation[fieldDollarT].toString());
            } else if (relation[fieldRel].toString() == relationAssistant) {
                contact->setAssistantsName(relation[fieldDollarT].toString());
            }
        }
    }

    /* Anniversary */
    if (data.contains(fieldGContactEvent)) {
        const auto &events = data[fieldGContactEvent].toArray();
        for (const auto &event : events) {
            if (event[fieldRel].toString() == eventAnniversary) {
                contact->setAnniversary(QDate::fromString(event[fieldGdWhen][fieldStartTime].toString(), Qt::ISODate));
                break;
            }
        }
    }

    /* Websites */
    if (data.contains(fieldGContactWebsite)) {
        const auto websites = data[fieldGContactWebsite].toArray();
        for (const auto &website : websites) {
            const auto rel = website[fieldRel].toString();
            const QUrl url(website[fieldHref].toString());
            if (rel == websiteHomePage) {
                KContacts::ResourceLocatorUrl locator;
                locator.setUrl(url);
                locator.setParameters({{paramType, {locatorHome}}});
                contact->insertExtraUrl(locator);
            } else if (rel == websiteWork) {
                KContacts::ResourceLocatorUrl locator;
                locator.setUrl(url);
                locator.setParameters({{paramType, {locatorWork}}});
                contact->insertExtraUrl(locator);
            } else if (rel == websiteProfile) {
                KContacts::ResourceLocatorUrl locator;
                locator.setUrl(url);
                locator.setParameters({{paramType, {locatorProfile}}});
                contact->insertExtraUrl(locator);
            } else if (rel == websiteBlog) {
                contact->setBlogFeed(url);
            } else {
                KContacts::ResourceLocatorUrl locator;
                locator.setUrl(url);
                locator.setParameters({{paramType, {rel}}});
                contact->insertExtraUrl(locator);
            }
        }
    }

    /* Emails */
    const auto emails = data[fieldGdEmail].toArray();
    for (const auto &email : emails) {
        const auto emailType = Contact::emailSchemeToProtocolName(email[fieldRel].toString());
        const QMap<QString, QStringList> params({{paramType, {emailType}}});
        contact->insertEmail(email[fieldAddress].toString(), email[fieldPrimary].toBool(), params);
    }

    /* IMs */
    const auto ims = data[fieldGdIm].toArray();
    for (const auto &im : ims) {
        const QString protocol = Contact::IMSchemeToProtocolName(im[fieldProtocol].toString());
        contact->insertCustom(QLatin1String("messaging/") + protocol,
                             QStringLiteral("All"),
                             im[fieldAddress].toString());
    }

    /* Phone numbers */
    const auto phones = data[fieldGdPhoneNumber].toArray();
    for (const auto &phone : phones) {
        KContacts::PhoneNumber phoneNumber(
                phone[fieldDollarT].toString(),
                Contact::phoneSchemeToType(phone[fieldRel].toString()));
        phoneNumber.setId(phoneNumber.number());
        contact->insertPhoneNumber(phoneNumber);
    }

    /* Addresses */
    const auto addresses = data[fieldGdStructuredPostalAddress].toArray();
    for (const auto &address : addresses) {
        KContacts::Address addr;
        addr.setId(QString::number(contact->addresses().count()));
        const auto addrObj = address.toObject();
        if (!addrObj.contains(fieldGdCity) &&
                !addrObj.contains(fieldGdCountry) &&
                !addrObj.contains(fieldGdPostcode) &&
                !addrObj.contains(fieldGdRegion) &&
                !addrObj.contains(fieldGdPOBox)) {
            fromXMLMap(&addr, &KContacts::Address::setExtended, addrObj, fieldGdStreet);
        } else {
            if (addrObj.contains(fieldGdStreet)) {
                fromXMLMap(&addr, &KContacts::Address::setStreet, addrObj, fieldGdStreet);
            }
            if (addrObj.contains(fieldGdCountry)) {
                fromXMLMap(&addr, &KContacts::Address::setCountry, addrObj, fieldGdCountry);
            }
            if (addrObj.contains(fieldGdCity)) {
                fromXMLMap(&addr, &KContacts::Address::setLocality, addrObj, fieldGdCity);
            }
            if (addrObj.contains(fieldGdPostcode)) {
                fromXMLMap(&addr, &KContacts::Address::setPostalCode, addrObj, fieldGdPostcode);
            }
            if (addrObj.contains(fieldGdRegion)) {
                fromXMLMap(&addr, &KContacts::Address::setRegion, addrObj, fieldGdRegion);
            }
            if (addrObj.contains(fieldGdPOBox)) {
                fromXMLMap(&addr, &KContacts::Address::setPostOfficeBox, addrObj, fieldGdPOBox);
            }
        }
        addr.setType(Contact::addressSchemeToType(address[fieldRel].toString()));

        contact->insertAddress(addr);
    }

    /* Birthday */
    static const auto bDay = data[fieldGContactBirthday];
    if (bDay.isObject()) {
        QString birthday = bDay[fieldWhen].toString();
        /* Birthdays in format "--MM-DD" are valid and mean that no year has
         * been specified. Since KABC does not support birthdays without year,
         * we simulate that by specifying a fake year - 1900 */
        if (birthday.startsWith(QLatin1String("--"))) {
            birthday = QLatin1String("1900") + birthday.mid(1);
        }
        contact->setBirthday(QDateTime::fromString(birthday, QStringLiteral("yyyy-MM-dd")));
    }

    /* User-defined fields */
    const auto userDefined = data[fieldGContactUserDefinedField].toArray();
    for (const auto &ud : userDefined) {
        contact->insertCustom(propertyKADDRESSBOOK, ud[fieldKey].toString(), ud[fieldValue].toString());
    }

    /* Groups */
    const auto groups = data[fieldGContactGroupMembershipInfo].toArray();
    QStringList groupsList;
    for (const auto &group : groups) {
        if (group[fieldDeleted].toBool() == false) {
            groupsList.append(group[fieldHref].toString());
        }
    }
    contact->insertCustom(propertyGCalendar, propertyGroupMembershipInfo, groupsList.join(QLatin1Char(',')));

    return contact;
}

ContactPtr JSONToContact(const QByteArray& jsonData)
{
    const auto document = QJsonDocument::fromJson(jsonData);
    const QJsonValue entry = document.object()[fieldEntry];
    const auto categories = entry[fieldCategory].toArray();

    const bool isContact = std::any_of(categories.cbegin(), categories.cend(),
                                       [](const auto &category) {
                                           return category[fieldTerm].toString() == schemaUrlContact;
                                       });
    if (!isContact) {
        return ContactPtr();
    }

    return Private::JSONToContact(entry.toObject()).staticCast<Contact>();
}


QByteArray contactToXML(const ContactPtr& contact)
{
    QByteArray output;

    QStringList parsedCustoms;

    /* Name */
    output.append("<gd:name>");
    if (!contact->givenName().isEmpty()) {
        output.append("<gd:givenName>").append(contact->givenName().toHtmlEscaped().toUtf8()).append("</gd:givenName>");
    }
    if (!contact->familyName().isEmpty()) {
        output.append("<gd:familyName>").append(contact->familyName().toHtmlEscaped().toUtf8()).append("</gd:familyName>");
    }
    if (!contact->assembledName().isEmpty()) {
        output.append("<gd:fullName>").append(contact->assembledName().toHtmlEscaped().toUtf8()).append("</gd:fullName>");
    }
    if (!contact->additionalName().isEmpty()) {
        output.append("<gd:additionalName>").append(contact->additionalName().toHtmlEscaped().toUtf8()).append("</gd:additionalName>");
    }
    if (!contact->prefix().isEmpty()) {
        output.append("<gd:namePrefix>").append(contact->prefix().toHtmlEscaped().toUtf8()).append("</gd:namePrefix>");
    }
    if (!contact->suffix().isEmpty()) {
        output.append("<gd:nameSuffix>").append(contact->suffix().toHtmlEscaped().toUtf8()).append("</gd:nameSuffix>");
    }
    output.append("</gd:name>");

    /* Notes */
    if (!contact->note().isEmpty()) {
        output.append("<atom:content type='text'>").append(contact->note().toHtmlEscaped().toUtf8()).append("</atom:content>");
    }

    /* Organization (work) */
    QByteArray org;
    const QString office = contact->office();
    if (!contact->organization().isEmpty()) {
        org.append("<gd:orgName>").append(contact->organization().toHtmlEscaped().toUtf8()).append("</gd:orgName>");
    }
    if (!contact->department().isEmpty()) {
        org.append("<gd:orgDepartment>").append(contact->department().toHtmlEscaped().toUtf8()).append("</gd:orgDepartment>");
    }
    if (!contact->title().isEmpty()) {
        org.append("<gd:orgTitle>").append(contact->title().toHtmlEscaped().toUtf8()).append("</gd:orgTitle>");
    }
    if (!office.isEmpty()) {
        org.append("<gd:where>").append(office.toHtmlEscaped().toUtf8()).append("</gd:where>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-Office");
    }
    if (!org.isEmpty()) {
        output.append("<gd:organization rel=\"http://schemas.google.com/g/2005#work\">").append(org).append("</gd:organization>");
    }

    /* Nickname */
    if (!contact->nickName().isEmpty()) {
        output.append("<gContact:nickname>").append(contact->nickName().toHtmlEscaped().toUtf8()).append("</gContact:nickname>");
    }

    /* Occupation */
    if (!contact->profession().isEmpty()) {
        output.append("<gContact:occupation>").append(contact->profession().toHtmlEscaped().toUtf8()).append("</gContact:occupation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-Profession");
    }

    /* Spouse */
    const QString spouse = contact->spousesName();
    if (!spouse.isEmpty()) {
        output.append("<gContact:relation rel=\"spouse\">").append(spouse.toHtmlEscaped().toUtf8()).append("</gContact:relation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-SpousesName");
    }

    /* Manager */
    const QString manager = contact->managersName();
    if (!manager.isEmpty()) {
        output.append("<gContact:relation rel=\"manager\">").append(manager.toHtmlEscaped().toUtf8()).append("</gContact:relation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-ManagersName");
    }

    /* Assistant */
    const QString assistant = contact->assistantsName();
    if (!assistant.isEmpty()) {
        output.append("<gContact:relation rel=\"assistant\">").append(assistant.toHtmlEscaped().toUtf8()).append("</gContact:relation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-AssistantsName");
    }

    /* Anniversary */
    const QString anniversary = contact->anniversary().toString(Qt::ISODate);
    if (!anniversary.isEmpty()) {
        output.append("<gContact:event rel=\"anniversary\"><gd:when startTime=\"").append(anniversary.toHtmlEscaped().toUtf8()).append("\" /></gContact:event>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-Anniversary");
    }

    /* Blog */
    const QString blog = contact->blogFeed().url();
    if (!blog.isEmpty()) {
        output.append("<gContact:website rel=\"blog\" href=\"").append(blog.toHtmlEscaped().toUtf8()).append("\" />");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-BlogFeed");
    }

    /* URLs */
    const auto extraUrls = contact->extraUrlList();
    for (const auto &extraUrl : extraUrls) {
        const auto rels = extraUrl.parameters().value(QStringLiteral("TYPE"));
        auto rel = rels.isEmpty() ? "other" : rels.at(0).toLower().toUtf8();
        if (rel == "home") {
            rel = "home-page";
        }
        output.append("<gContact:website rel=\"" + rel + "\" href=\"" + extraUrl.url().toString().toHtmlEscaped().toUtf8() + "\" />");
    }

    /* Emails */
    const auto preferredEmail = contact->preferredEmail();
    Q_FOREACH(const auto &email, contact->emailList()) {
        const auto rels = email.parameters().value(QStringLiteral("TYPE"), { QStringLiteral("home") });
        const auto rel = Private::addRelSchema(rels.isEmpty() ? "home" : rels.at(0).toLower().toUtf8());
        output.append("<gd:email rel='" + rel + "' address='").append(email.mail().toHtmlEscaped().toUtf8()).append("'");
        if (email.mail() == preferredEmail) {
            output.append(" primary=\"true\"");
        }
        output.append("/>");
    }

    /* IMs */
    const QString im_str = QStringLiteral("<gd:im address=\"%1\" protocol=\"%2\" rel=\"http://schemas.google.com/g/2005#other\" primary=\"%3\"/>");
    Q_FOREACH(const auto &impp, contact->imppList()) {
        output.append(im_str.arg(impp.address().path(), Contact::IMProtocolNameToScheme(impp.serviceType()), Utils::bool2Str(impp.isPreferred())).toUtf8());
    }
    Q_FOREACH(const QString &im, contact->customs()) {
        if (im.startsWith(QLatin1String("messaging/"))) {
            QString key = im.left(im.indexOf(QLatin1Char(':')));
            QString value = im.mid(im.indexOf(QLatin1Char(':')) + 1);
            QString proto = key.mid(10);
            proto.chop(4);
            bool primary = (contact->custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-IMAddress")) == value);
            output.append(im_str.arg(value, Contact::IMProtocolNameToScheme(proto), Utils::bool2Str(primary)).toUtf8());
            parsedCustoms << key;
        /* X-messaging is probably a new key (?) used by KAddressbook when importing
         * contacts from vCard. */
        } else if (im.startsWith(QLatin1String("X-messaging"))) {
            const QString key = im.left(im.indexOf(QLatin1Char(':')));
            const QString value = im.mid(im.indexOf(QLatin1Char(':')) + 1);
            QString proto = key.mid(12); /* strlen("X-messaging/") */
            if (proto.endsWith(QLatin1String("-All"))) {
                proto.chop(4);
            }
            output.append(im_str.arg(value, proto, QStringLiteral("false")).toUtf8());
            parsedCustoms << key;
        }
    }
    parsedCustoms << QStringLiteral("KADDRESSBOOK-X-IMAddress");

    /* Phone numbers */
    const QString phone_str = QStringLiteral("<gd:phoneNumber rel=\"%1\">%2</gd:phoneNumber>");
    Q_FOREACH(const KContacts::PhoneNumber &number, contact->phoneNumbers()) {
        output.append(phone_str.arg(Contact::phoneTypeToScheme(number.type()), number.number()).toUtf8());
    }

    /* Address */
    Q_FOREACH(const KContacts::Address &address, contact->addresses()) {
        output.append("<gd:structuredPostalAddress rel='")
        .append(Contact::addressTypeToScheme(address.type()).toUtf8())
        .append("'>");

        if (!address.locality().isEmpty())
            output.append("<gd:city>").append(address.locality().toHtmlEscaped().toUtf8()).append("</gd:city>");
        if (!address.street().isEmpty())
            output.append("<gd:street>").append(address.street().toHtmlEscaped().toUtf8()).append("</gd:street>");
        if (!address.region().isEmpty())
            output.append("<gd:region>").append(address.region().toHtmlEscaped().toUtf8()).append("</gd:region>");
        if (!address.postalCode().isEmpty())
            output.append("<gd:postcode>").append(address.postalCode().toHtmlEscaped().toUtf8()).append("</gd:postcode>");
        if (!address.country().isEmpty())
            output.append("<gd:country>").append(address.country().toHtmlEscaped().toUtf8()).append("</gd:country>");
        if (!address.formattedAddress().isEmpty())
            output.append("<gd:formattedAddress>").append(address.formattedAddress().toHtmlEscaped().toUtf8()).append("</gd:formattedAddress>");
        output.append("</gd:structuredPostalAddress>");
    }

    /* Birthday */
    const QDate birthday = contact->birthday().date();
    if (birthday.isValid()) {
        QString birthdayStr;
        /* We use year 1900 as a fake year for birthdays without a year specified.
            * Here we assume that nobody actually has a contact born in 1900 and so
            * we replace 1900 by "-", so that we get "--MM-dd" date, which is a valid
            * birthday date according to RFC6350 */
        if (birthday.year() == 1900) {
            birthdayStr = birthday.toString(QStringLiteral("--MM-dd"));
        } else {
            birthdayStr = birthday.toString(QStringLiteral("yyyy-MM-dd"));
        }
        output.append("<gContact:birthday when='").append(birthdayStr.toUtf8()).append("'/>");
    }

    const QStringList groups = contact->custom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo")).split(QLatin1Char(','));
    qCDebug(KGAPIDebug) << groups;
    if ((!groups.isEmpty()) && !groups.at(0).isEmpty()) {
        for (const QString & group :groups) {
            bool removed = contact->groupIsDeleted(group);
            if (!removed)
                output.append(QStringLiteral("<gContact:groupMembershipInfo deleted=\"false\" href=\"%2\" />").arg(group).toUtf8());
        }
    }
    parsedCustoms << QStringLiteral("GCALENDAR-groupMembershipInfo");

    /* User-defined fields */
    const QStringList customs = contact->customs();
    const QString defined_str = QStringLiteral("<gContact:userDefinedField key=\"%1\" value=\"%2\" />");
    for (const QString &customStr : customs) {
        QString key = customStr.left(customStr.indexOf(QLatin1Char(':')));
        if (!parsedCustoms.contains(key)) {
            if (key.startsWith(QLatin1String("KADDRESSBOOK-"))) {
                key.remove(QStringLiteral("KADDRESSBOOK-"));
            }
            const QString value = customStr.mid(customStr.indexOf(QLatin1Char(':')) + 1);
            output.append(defined_str.arg(key.toHtmlEscaped(), value).toHtmlEscaped().toUtf8());
        }
    }

    return output;
}

QByteArray contactsGroupToXML(const ContactsGroupPtr& group)
{
    QByteArray output;

    output.append("<atom:title type=\"text\">").append(group->title().toHtmlEscaped().toUtf8()).append("</atom:title>");
    output.append("<atom:content type=\"text\">").append(group->content().toHtmlEscaped().toUtf8()).append("</atom:content>");

    return output;
}

ContactPtr XMLToContact(const QByteArray& xmlData)
{
    QByteArray xmlDoc;
    /* Document without <xml> header is not valid and Qt won't parse it */
    if (!xmlData.contains("<?xml")) {
        xmlDoc.append("<?xml version='1.0' encoding='UTF-8'?>");
    }
    xmlDoc.append(xmlData);

    QDomDocument doc;
    doc.setContent(xmlDoc);
    const QDomNodeList entry = doc.elementsByTagName(QStringLiteral("entry"));
    QDomNodeList data;
    if (!entry.isEmpty()) {
        data = entry.at(0).childNodes();
    } else {
        return ContactPtr();
    }

    bool isGroup = false;
    for (int i = 0; i < data.count(); ++i) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (((e.tagName() == QLatin1String("category")) &&
             (e.attribute(QStringLiteral("term")) == schemaUrlContactGroup)) ||
            ((e.tagName() == QLatin1String("atom:category")) &&
             (e.attribute(QStringLiteral("term")) == schemaUrlGroup)))
        {
            isGroup = true;
            break;
        }
    }

    if (isGroup) {
        return ContactPtr();
    }

    QStringList groups;
    ContactPtr contact(new Contact);

    contact->setEtag(entry.at(0).toElement().attribute(QStringLiteral("gd:etag")));

    for (int i = 0; i < data.count(); ++i) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (e.tagName() == QLatin1String("id")) {
            contact->setUid(e.text());
            continue;
        }

        /* ETag */
        if (e.tagName() == QLatin1String("etag")) {
            contact->setEtag(e.text());
            continue;
        }

        if (e.tagName() == QLatin1String("gd:name")) {
            QDomNodeList l = e.childNodes();
            for (int i = 0; i < l.length(); ++i) {
                const QDomElement el = l.at(i).toElement();

                if (el.tagName() == QLatin1String("gd:fullName")) {
                    contact->setFormattedName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:givenName")) {
                    contact->setGivenName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:familyName")) {
                    contact->setFamilyName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:additionalName")) {
                    contact->setAdditionalName(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:namePrefix")) {
                    contact->setPrefix(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:nameSuffix")) {
                    contact->setSuffix(el.text());
                    continue;
                }

            }
            continue;
        }

        /* If the contact was deleted, we don't need more info about it.
        * Just store our own flag, which will be then parsed by the resource
        * itself. */
        contact->setDeleted(e.tagName() == QLatin1String("gd:deleted"));

        if (e.tagName() == QLatin1String("updated")) {
            contact->setUpdated(QDateTime::fromString(e.text(), Qt::ISODate));
        }

        /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
        * itself. */
        if ((e.tagName() == QLatin1String("link")) &&
            (e.attribute(QStringLiteral("rel")) == schemaUrlPhoto)) {
            contact->setPhotoUrl(e.attribute(QStringLiteral("href")));    /* URL */
            continue;
        }

        /* Name */
        if (e.tagName() == QLatin1String("title")) {
            contact->setName(e.text());
            continue;
        }

        /* Note */
        if (e.tagName() == QLatin1String("content")) {
            contact->setNote(e.text());
            continue;
        }

        /* Organization (work) - KABC supports only organization */
        if (e.tagName() == QLatin1String("gd:organization")) {
            const QDomNodeList l = e.childNodes();
            for (int i = 0; i < l.length(); ++i) {
                const QDomElement el = l.at(i).toElement();

                if (el.tagName() == QLatin1String("gd:orgName")) {
                    contact->setOrganization(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:orgDepartment")) {
                    contact->setDepartment(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:orgTitle")) {
                    contact->setTitle(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:where")) {
                    contact->setOffice(el.text());
                    continue;
                }
            }
            continue;
        }

        /* Nickname */
        if (e.tagName() == QLatin1String("gContact:nickname")) {
            contact->setNickName(e.text());
            continue;
        }

        /* Occupation (= organization/title) */
        if (e.tagName() == QLatin1String("gContact:occupation")) {
            contact->setProfession(e.text());
            continue;
        }

        /* Relationships */
        if (e.tagName() == QLatin1String("gContact:relation")) {
            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("spouse")) {
                contact->setSpousesName(e.text());
                continue;
            }

            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("manager")) {
                contact->setManagersName(e.text());
                continue;
            }

            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("assistant")) {
                contact->setAssistantsName(e.text());
                continue;
            }

            continue;
        }

        /* Anniversary */
        if (e.tagName() == QLatin1String("gContact:event")) {
            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("anniversary")) {
                QDomElement w = e.firstChildElement(QStringLiteral("gd:when"));
                contact->setAnniversary(QDate::fromString(w.attribute(QStringLiteral("startTime"), QString()), Qt::ISODate));
            }

            continue;
        }

        /* Websites */
        if (e.tagName() == QLatin1String("gContact:website")) {
            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("blog")) {
                contact->setBlogFeed(QUrl(e.attribute(QStringLiteral("href"), QString())));
                continue;
            }

            KContacts::ResourceLocatorUrl url;
            QString rel = e.attribute(QStringLiteral("rel")).toUpper();
            if (rel == QLatin1String("home-page")) {
                rel = QStringLiteral("HOME");
            }
            url.setParameters({ { QStringLiteral("TYPE"), { rel } } });
            url.setUrl(QUrl(e.attribute(QStringLiteral("href"), {})));
            contact->insertExtraUrl(url);

            continue;
        }

        /* Emails */
        if (e.tagName() == QLatin1String("gd:email")) {
            const auto emailType = Contact::emailSchemeToProtocolName(e.attribute(QStringLiteral("rel"), {}));
            const QMap<QString, QStringList> params({ { QStringLiteral("TYPE"), { emailType } } });
            contact->insertEmail(e.attribute(QStringLiteral("address")),
                                 (e.attribute(QStringLiteral("primary")).toLower() == QLatin1String("true")), params);
            continue;
        }

        /* IMs */
        if (e.tagName() == QLatin1String("gd:im")) {
            contact->insertCustom(QLatin1String("messaging/") + Contact::IMSchemeToProtocolName(e.attribute(QStringLiteral("protocol"))),
                                  QStringLiteral("All"),
                                  e.attribute(QStringLiteral("address")));
            continue;
        }

        /* Phone numbers */
        if (e.tagName() == QLatin1String("gd:phoneNumber")) {
            KContacts::PhoneNumber number(e.text(), Contact::phoneSchemeToType(e.attribute(QStringLiteral("rel"))));
            number.setId(e.text());
            contact->insertPhoneNumber(number);
            continue;
        }

        /* Addresses */
        if (e.tagName() == QLatin1String("gd:structuredPostalAddress")) {
            KContacts::Address address;
            address.setId(QString::number(contact->addresses().count()));
            const QDomNodeList l = e.childNodes();
            for (int i = 0; i < l.length(); ++i) {
                const QDomElement el = l.at(i).toElement();

                if (el.tagName() == QLatin1String("gd:street")) {
                    address.setStreet(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:country")) {
                    address.setCountry(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:city")) {
                    address.setLocality(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:postcode")) {
                    address.setPostalCode(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:region")) {
                    address.setRegion(el.text());
                    continue;
                }

                if (el.tagName() == QLatin1String("gd:pobox")) {
                    address.setPostOfficeBox(el.text());
                    continue;
                }
            }

            address.setType(Contact::addressSchemeToType(e.attribute(QStringLiteral("rel")),
                                                         (e.attribute(QStringLiteral("primary")) == QLatin1String("true"))));
            contact->insertAddress(address);
            continue;
        }

        /* Birthday */
        if (e.tagName() == QLatin1String("gContact:birthday")) {
            QString birthday = e.attribute(QStringLiteral("when"));
            /* Birthdays in format "--MM-DD" are valid and mean that no year has
             * been specified. Since KABC does not support birthdays without year,
             * we simulate that by specifying a fake year - 1900 */
            if (birthday.startsWith(QLatin1String("--"))) {
                birthday = QLatin1String("1900") + birthday.mid(1);
            }
            contact->setBirthday(QDateTime::fromString(birthday, QStringLiteral("yyyy-MM-dd")));
            continue;
        }

        /* User-defined tags */
        if (e.tagName() == QLatin1String("gContact:userDefinedField")) {
            contact->insertCustom(QStringLiteral("KADDRESSBOOK"),
                                  e.attribute(QStringLiteral("key"), QString()),
                                  e.attribute(QStringLiteral("value"), QString()));
            continue;
        }

        if (e.tagName() == QLatin1String("gContact:groupMembershipInfo")) {
            if (e.hasAttribute(QStringLiteral("deleted")) || e.attribute(QStringLiteral("deleted")).toInt() == false) {
                groups.append(e.attribute(QStringLiteral("href")));
            }
        }
    }

    contact->insertCustom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo"), groups.join(QLatin1Char(',')));

    return contact;
}

ContactsGroupPtr XMLToContactsGroup(const QByteArray& xmlData)
{
    QByteArray xmlDoc;
    /* Document without <xml> header is not valid and Qt won't parse it */
    if (!xmlData.contains("<?xml")) {
        xmlDoc.append("<?xml version='1.0' encoding='UTF-8'?>");
    }
    xmlDoc.append(xmlData);

    QDomDocument doc;
    doc.setContent(xmlDoc);
    const QDomNodeList entry = doc.elementsByTagName(QStringLiteral("entry"));
    QDomNodeList data;
    if (!entry.isEmpty()) {
        data = entry.at(0).childNodes();
    } else {
        return ContactsGroupPtr();
    }

    bool isGroup = false;
    for (int i = 0; i < data.count(); ++i) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (((e.tagName() == QLatin1String("category")) &&
             (e.attribute(QStringLiteral("term")) == schemaUrlContactGroup)) ||
            ((e.tagName() == QLatin1String("atom:category")) &&
             (e.attribute(QStringLiteral("term")) == schemaUrlGroup)))
        {
            isGroup = true;
            break;
        }
    }

    if (!isGroup) {
        return ContactsGroupPtr();
    }

    ContactsGroupPtr group(new ContactsGroup);
    QStringList groups;

    for (int i = 0; i < data.count(); ++i) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (e.tagName() == QLatin1String("id")) {
            group->setId(e.text());
            continue;
        }

        if (e.tagName() == QLatin1String("updated")) {
            group->setUpdated(QDateTime::fromString(e.text(), Qt::ISODate));
            continue;
        }

        if ((e.tagName() == QLatin1String("title")) ||
            (e.tagName() == QLatin1String("atom:title"))) {
            group->setTitle(e.text());
            continue;
        }

        if ((e.tagName() == QLatin1String("content")) ||
            (e.tagName() == QLatin1String("atom:content"))) {
            group->setContent(e.text());
            continue;
        }

        if (e.tagName() == QLatin1String("gContact:systemGroup")) {
            group->setIsSystemGroup(true);
            continue;
        }
    }

    return group;
}

} // namespace ContactsService

} // namespace KGAPI2
