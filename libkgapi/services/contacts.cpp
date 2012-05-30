/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "contacts.h"
#include "accessmanager.h"
#include "objects/contact.h"
#include "objects/contactsgroup.h"

#include <kurl.h>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace KGAPI
{

namespace Services
{

class ContactsPrivate
{

public:
    static KGAPI::Object* JSONToContact(const QVariantMap &data);
    static KGAPI::Object* JSONToGroup(const QVariantMap &data);

    static QByteArray contactToXML(const KGAPI::Object *object);
    static QByteArray groupToXML(const KGAPI::Object *object);

    static KGAPI::Object* XMLToContact(const QDomDocument &data);
    static KGAPI::Object* XMLToGroup(const QDomDocument &data);
};

}

}

using namespace KGAPI;
using namespace KGAPI::Services;

QUrl Contacts::ScopeUrl("https://www.google.com/m8/feeds/");

QByteArray Contacts::objectToJSON(KGAPI::Object* object)
{
    /* Google allows writing only XML data */
    return QByteArray();

    Q_UNUSED(object)
}

QByteArray Contacts::objectToXML(KGAPI::Object* object)
{
    if (!object)
        return QByteArray();

    if (dynamic_cast< KGAPI::Objects::Contact* >(object))
        return ContactsPrivate::contactToXML(object);
    else if (dynamic_cast< KGAPI::Objects::ContactsGroup* >(object))
        return ContactsPrivate::groupToXML(object);
    else
        return QByteArray();
}


QList< KGAPI::Object* > Contacts::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QList< KGAPI::Object* > output;
    QJson::Parser parser;
    QJson::Serializer serializer;

    QVariantMap head = parser.parse(jsonFeed).toMap();

    QVariantMap feed = head["feed"].toMap();

    Q_FOREACH(const QVariant &c, feed["category"].toList()) {
        QVariantMap category = c.toMap();
        bool groups = (category["term"].toString() == "http://schemas.google.com/contact/2008#group");

        QVariantList entries = feed["entry"].toList();

        Q_FOREACH(const QVariant &e, entries) {
            if (groups)
                output << ContactsPrivate::JSONToGroup(e.toMap());
            else
                output << ContactsPrivate::JSONToContact(e.toMap());
        }
    }

    QVariantList links = feed["link"].toList();
    Q_FOREACH(const QVariant &l, links) {
        QVariantMap link = l.toMap();
        if (link["rel"].toString() == "next") {
            feedData.nextPageUrl = KUrl(link["href"].toString());
            break;
        }
    }

    QVariantMap tmp = feed["openSearch$totalResults"].toMap();
    feedData.totalResults = tmp["$t"].toInt();

    tmp = feed["openSearch$startIndex"].toMap();
    feedData.startIndex = tmp["$t"].toInt();

    tmp = feed["openSearch$itemsPerPage"].toMap();
    feedData.itemsPerPage = tmp["$t"].toInt();

    return output;
}

Object *Contacts::JSONToObject(const QByteArray &jsonData)
{
    QJson::Parser parser;
    QVariantMap data, entry;

    data = parser.parse(jsonData).toMap();

    entry = data["entry"].toMap();

    Q_FOREACH(const QVariant &c, entry["category"].toList()) {
        QVariantMap category = c.toMap();

        if (category["term"].toString() == "http://schemas.google.com/contact/2008#group") {
            return ContactsPrivate::JSONToGroup(entry);
        }

        if (category["term"].toString() == "http://schemas.google.com/contact/2008#contact") {
            return ContactsPrivate::JSONToContact(entry);
        }
    }

    return 0;
}

KGAPI::Object* Contacts::XMLToObject(const QByteArray& xmlData)
{
    QByteArray xmlDoc;
    /* Document without <xml> header is not valid and Qt won't parse it */
    if (!xmlData.contains("<?xml"))
        xmlDoc.append("<?xml version='1.0' encoding='UTF-8'?>");
    xmlDoc.append(xmlData);

    QDomDocument doc;
    doc.setContent(xmlDoc);
    QDomNodeList entry = doc.elementsByTagName("entry");
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    bool isGroup = false;

    for (int i = 0; i < data.count(); i++) {
        QDomNode n = data.at(i);
        QDomElement e = n.toElement();

        if ((e.tagName() == "category") &&
                (e.attribute("term") == "http://schemas.google.com/contact/2008#group")) {
            isGroup = true;
            break;
        }
    }

    if (isGroup)
        return ContactsPrivate::XMLToGroup(doc);
    else
        return ContactsPrivate::XMLToContact(doc);

}

QList< KGAPI::Object* > Contacts::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object* >();
}

const QUrl& Contacts::scopeUrl() const
{
    return Contacts::ScopeUrl;
}

QUrl Contacts::fetchAllContactsUrl(const QString& user, const bool &showDeleted)
{
    QUrl url = "https://www.google.com/m8/feeds/contacts/" + user + "/full?alt=json";
    if (showDeleted)
        url.addQueryItem("showdeleted", "true");

    return url;
}

QUrl Contacts::fetchContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains("/"))
        id = contactID.mid(contactID.lastIndexOf("/") + 1);
    else
        id = contactID;

    return "https://www.google.com/m8/feeds/contacts/" + user + "/full/" + id + "?alt=json";
}

QUrl Contacts::createContactUrl(const QString& user)
{
    return "https://www.google.com/m8/feeds/contacts/" + user + "/full";
}

QUrl Contacts::updateContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains("/"))
        id = contactID.mid(contactID.lastIndexOf("/") + 1);
    else
        id = contactID;

    return "https://www.google.com/m8/feeds/contacts/" + user + "/full/" + id;
}

QUrl Contacts::removeContactUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains("/"))
        id = contactID.mid(contactID.lastIndexOf("/") + 1);
    else
        id = contactID;

    return "https://www.google.com/m8/feeds/contacts/" + user + "/full/" + id;
}

QUrl Contacts::fetchAllGroupsUrl(const QString &user)
{
    return "https://www.google.com/m8/feeds/groups/" + user + "/full?alt=json";
}

QUrl Contacts::fetchGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains("/"))
        id = groupId.mid(groupId.lastIndexOf("/") + 1);
    else
        id = groupId;

    return "https://www.google.com/m8/feeds/groups/" + user + "/base/" + id + "?alt=json";
}

QUrl Contacts::createGroupUrl(const QString &user)
{
    return "https://www.google.com/m8/feeds/groups/" + user + "/full";
}

QUrl Contacts::updateGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains("/"))
        id = groupId.mid(groupId.lastIndexOf("/") + 1);
    else
        id = groupId;

    return "https://www.google.com/m8/feeds/groups/" + user + "/full/" + id;
}

QUrl Contacts::removeGroupUrl(const QString &user, const QString &groupId)
{
    QString id;
    if (groupId.contains("/"))
        id = groupId.mid(groupId.lastIndexOf("/") + 1);
    else
        id = groupId;

    return "https://www.google.com/m8/feeds/groups/" + user + "/full/" + id;
}

QUrl Contacts::photoUrl(const QString& user, const QString& contactID)
{
    QString id;
    if (contactID.contains("/"))
        id = contactID.mid(contactID.lastIndexOf("/") + 1);
    else
        id = contactID;

    return "https://www.google.com/m8/feeds/photos/media/" + user + "/" + id;
}


QString Contacts::protocolVersion() const
{
    return "3.0";
}

bool Contacts::supportsJSONRead(QString* urlParam)
{
    if (urlParam)
        *urlParam = "json";

    return true;
}

bool Contacts::supportsJSONWrite(QString* urlParam)
{
    Q_UNUSED(urlParam)

    return false;
}



/*********************************** PRIVATE *************************************/

KGAPI::Object* ContactsPrivate::JSONToGroup(const QVariantMap &data)
{
    Objects::ContactsGroup *object = new Objects::ContactsGroup;

    object->setId(data["id"].toMap()["$t"].toString());
    object->setEtag(data["gd$etag"].toString());

    object->setTitle(data["title"].toMap()["$t"].toString());
    object->setContent(data["content"].toMap()["$t"].toString());

    object->setUpdated(AccessManager::RFC3339StringToDate(data["updated"].toMap()["$t"].toString()).dateTime());

    if (data.contains("gContact$systemGroup"))
        object->setIsSystemGroup(true);
    else
        object->setIsSystemGroup(false);

    return object;
}


KGAPI::Object* ContactsPrivate::JSONToContact(const QVariantMap &data)
{
    Objects::Contact *object = new Objects::Contact();

    /* Google contact ID */
    object->setUid(data["id"].toMap()["$t"].toString());

    /* Google ETAG. This can be used to identify if the item was changed remotly */
    object->setEtag(data["gd$etag"].toString());

    /* Date and time when contact was updated on the remote server */
    object->setUpdated(AccessManager::RFC3339StringToDate(data["updated"].toMap()["$t"].toString()));

    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    object->setDeleted(data["gd$deleted"].isValid());

    /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
     * itself. */
    QVariantList links = data["link"].toList();
    Q_FOREACH(const QVariant & link, links) {
        if (link.toMap()["rel"] == "http://schemas.google.com/contacts/2008/rel#photo") {
            object->setPhotoUrl(link.toMap()["href"].toString());
        }
    }

    /* Name */
    if (data.contains("title"))
        object->setName(data["title"].toMap()["$t"].toString());

    /* Formatted name */
    if (data.contains("gd$name")) {
        QVariantMap name = data["gd$name"].toMap();

        if (name.contains("gd$fullName"))
            object->setFormattedName(name["gd$fullName"].toMap()["$t"].toString());
        if (name.contains("gd$givenName"))
            object->setGivenName(name["gd$givenName"].toMap()["$t"].toString());
        if (name.contains("gd$familyName"))
            object->setFamilyName(name["gd$familyName"].toMap()["$t"].toString());
        if (name.contains("gd$additionalName"))
            object->setAdditionalName(name["gd$additionalName"].toMap()["$t"].toString());;
        if (name.contains("gd$namePrefix"))
            object->setPrefix(name["gd$namePrefix"].toMap()["$t"].toString());
        if (name.contains("gd$nameSuffix"))
            object->setSuffix(name["gd$nameSuffix"].toMap()["$t"].toString());
    }

    /* Note */
    if (data.contains("content"))
        object->setNote(data["content"].toMap()["$t"].toString());

    /* Organization (work) - KABC supports only one organization */
    if (data.contains("gd$organization")) {
        QVariantMap organization = data["gd$organization"].toList().first().toMap();

        if (organization.contains("gd$orgName"))
            object->setOrganization(organization["gd$orgName"].toMap()["$t"].toString());

        if (organization.contains("gd$orgDepartment"))
            object->setDepartment(organization["gd$orgDepartment"].toMap()["$t"].toString());

        if (organization.contains("gd$orgTitle"))
            object->setTitle(organization["gd$orgTitle"].toMap()["$t"].toString());

        if (organization.contains("gd$where"))
            object->setOffice(organization["gd$where"].toMap()["$t"].toString());
    }

    /* Nickname */
    if (data.contains("gContact$nickname")) {
        object->setNickName(data["gContact$nickname"].toMap()["$t"].toString());
    }

    /* Occupation (= organization/title) */
    if (data.contains("gContact$occupation")) {
        object->setProfession(data["gContact$occupation"].toMap()["$t"].toString());
    }

    /* Relationships */
    if (data.contains("gContact$relation")) {
        Q_FOREACH(const QVariant &r, data["gContact$relation"].toList()) {
            QVariantMap relation = r.toMap();
            if (relation["rel"].toString() == "spouse") {
                object->setSpousesName(relation["$t"].toString());
                continue;
            }

            if (relation["rel"].toString() == "manager") {
                object->setManagersName(relation["$t"].toString());
                continue;
            }

            if (relation["rel"].toString() == "assistant") {
                object->setAssistantsName(relation["$t"].toString());
                continue;
            }
        }
    }

    /* Anniversary */
    if (data.contains("gContact$event")) {
        Q_FOREACH(const QVariant &e, data["gContact$event"].toList()) {
            QVariantMap event = e.toMap();

            if (event["rel"].toString() == "anniversary") {
                QVariantMap when = event["gd$when"].toMap();
                object->setAnniversary(when["startTime"].toString());
            }
        }
    }

    /* Websites */
    if (data.contains("gContact$website")) {
        Q_FOREACH(const QVariant &w, data["gContact$website"].toList()) {
            QVariantMap web = w.toMap();

            if (web["rel"].toString() == "home-page") {
                object->setUrl(KUrl(web["href"].toString()));
                continue;
            }

            if (web["rel"].toString() == "blog") {
                object->setBlogFeed(web["href"].toString());
                continue;
            }
        }
    }

    /* Emails */
    QVariantList emails = data["gd$email"].toList();
    Q_FOREACH(const QVariant & em, emails) {
        QVariantMap email = em.toMap();
        object->insertEmail(email["address"].toString(), email["primary"].toBool());
    }

    /* IMs */
    QVariantList ims = data["gd$im"].toList();
    Q_FOREACH(const QVariant & i, ims) {
        QVariantMap im = i.toMap();
        object->insertCustom("messaging/" + Objects::Contact::IMSchemeToProtocolName(im["protocol"].toString()), "All", im["address"].toString());
    }

    /* Phone numbers */
    QVariantList phones = data["gd$phoneNumber"].toList();
    Q_FOREACH(const QVariant & p, phones) {
        QVariantMap phone = p.toMap();
        object->insertPhoneNumber(KABC::PhoneNumber(phone["$t"].toString(),
                                  Objects::Contact::phoneSchemeToType(phone["rel"].toString())));
    }

    /* Addresses */
    QVariantList addresses = data["gd$structuredPostalAddress"].toList();
    Q_FOREACH(const QVariant & a, addresses) {
        QVariantMap address = a.toMap();
        KABC::Address addr;
        if (!address.contains("gd$city") &&
                !address.contains("gd$country") &&
                !address.contains("gd$postcode") &&
                !address.contains("gd$region") &&
                !address.contains("gd$pobox")) {
            addr.setExtended(address["gd$street"].toMap()["$t"].toString());
        } else {
            if (address.contains("gd$street"))
                addr.setStreet(address["gd$street"].toMap()["$t"].toString());
            if (address.contains("gd$country"))
                addr.setCountry(address["gd$country"].toMap()["$t"].toString());
            if (address.contains("gd$city"))
                addr.setLocality(address["gd$city"].toMap()["$t"].toString());
            if (address.contains("gd$postcode"))
                addr.setPostalCode(address["gd$postcode"].toMap()["$t"].toString());
            if (address.contains("gdregion"))
                addr.setRegion(address["gd$region"].toMap()["$t"].toString());
            if (address.contains("gd$pobox"))
                addr.setPostOfficeBox(address["gd$pobox"].toMap()["$t"].toString());
        }
        addr.setType(Objects::Contact::addressSchemeToType(address["rel"].toString()));

        object->insertAddress(addr);
    }

    /* Birthday */
    QVariantMap bDay = data["gContact$birthday"].toMap();
    if (!bDay.isEmpty())
        object->setBirthday(QDateTime::fromString(bDay["when"].toString(), "yyyy-MM-dd"));

    /* User-defined fields */
    QVariantList userDefined = data["gContact$userDefinedField"].toList();
    Q_FOREACH(const QVariant & u, userDefined) {
        QVariantMap field = u.toMap();
        object->insertCustom("GCALENDAR", field["key"].toString(), field["value"].toString());
    }

    /* Groups */
    QVariantList groups = data["gContact$groupMembershipInfo"].toList();
    QStringList groupsList;
    Q_FOREACH(const QVariant & g, groups) {
        QVariantMap group = g.toMap();
        if (group["deleted"].toBool() == false)
            groupsList.append(group["href"].toString());
    }
    object->insertCustom("GCALENDAR", "groupMembershipInfo", groupsList.join(","));

    return dynamic_cast< KGAPI::Object* >(object);
}


QByteArray ContactsPrivate::contactToXML(const KGAPI::Object* object)
{
    const Objects::Contact *contact = static_cast< const Objects::Contact* >(object);

    QByteArray output;

    QStringList parsedCustoms;

    /* Name */
    output.append("<gd:name>");
    if (!contact->givenName().isEmpty())
        output.append("<gd:givenName>").append(contact->givenName().toUtf8()).append("</gd:givenName>");
    if (!contact->familyName().isEmpty())
        output.append("<gd:familyName>").append(contact->familyName().toUtf8()).append("</gd:familyName>");
    if (!contact->assembledName().isEmpty())
        output.append("<gd:fullName>").append(contact->assembledName().toUtf8()).append("</gd:fullName>");
    if (!contact->additionalName().isEmpty())
        output.append("<gd:additionalName>").append(contact->additionalName().toUtf8()).append("</gd:additionalName>");
    if (!contact->prefix().isEmpty())
        output.append("<gd:namePrefix>").append(contact->prefix().toUtf8()).append("</gd:namePrefix>");
    if (!contact->suffix().isEmpty())
        output.append("<gd:nameSuffix>").append(contact->suffix().toUtf8()).append("</gd:nameSuffix>");
    output.append("</gd:name>");

    /* Notes */
    if (!contact->note().isEmpty())
        output.append("<atom:content type='text'>").append(contact->note().toUtf8()).append("</atom:content>");

    /* Organization (work) */
    QByteArray org;
    QString office = contact->office();
    if (!contact->organization().isEmpty())
        org.append("<gd:orgName>").append(contact->organization().toUtf8()).append("</gd:orgName>");
    if (!contact->department().isEmpty())
        org.append("<gd:orgDepartment>").append(contact->department().toUtf8()).append("</gd:orgDepartment>");
    if (!contact->title().isEmpty())
        org.append("<gd:orgTitle>").append(contact->title().toUtf8()).append("</gd:orgTitle>");
    if (!office.isEmpty()) {
        org.append("<gd:where>").append(office.toUtf8()).append("</gd:where>");
        parsedCustoms << "KADDRESSBOOK-X-Office";
    }
    if (!org.isEmpty())
        output.append("<gd:organization rel=\"http://schemas.google.com/g/2005#work\">").append(org).append("</gd:organization>");

    /* Nickname */
    if (!contact->nickName().isEmpty())
        output.append("<gContact:nickname>").append(contact->nickName().toUtf8()).append("</gContact:nickname>");

    /* Occupation */
    if (!contact->profession().isEmpty()) {
        output.append("<gContact:occupation>").append(contact->profession().toUtf8()).append("</gContact:occupation>");
        parsedCustoms << "KADDRESSBOOK-X-Profession";
    }

    /* Spouse */
    QString spouse = contact->spousesName();
    if (!spouse.isEmpty()) {
        output.append("<gContact:relation rel=\"spouse\">").append(spouse.toUtf8()).append("</gContact:relation>");
        parsedCustoms << "KADDRESSBOOK-X-SpousesName";
    }

    /* Manager */
    QString manager = contact->managersName();
    if (!manager.isEmpty()) {
        output.append("<gContact:relation rel=\"manager\">").append(manager.toUtf8()).append("</gContact:relation>");
        parsedCustoms << "KADDRESSBOOK-X-ManagersName";
    }

    /* Assistant */
    QString assistant = contact->assistantsName();
    if (!assistant.isEmpty()) {
        output.append("<gContact:relation rel=\"assistant\">").append(assistant.toUtf8()).append("</gContact:relation>");
        parsedCustoms << "KADDRESSBOOK-X-AssistantsName";
    }

    /* Anniversary */
    QString anniversary = contact->anniversary();
    if (!anniversary.isEmpty()) {
        output.append("<gContact:event rel=\"anniversary\"><gd:when startTime=\"").append(anniversary.toUtf8()).append("\" /></gContact:event>");
        parsedCustoms << "KADDRESSBOOK-X-Anniversary";
    }

    /* Homepage */
    if (!contact->url().isEmpty())
        output.append("<gContact:website rel=\"home-page\" href=\"").append(contact->url().prettyUrl().toUtf8()).append("\" />");

    /* Blog */
    QString blog = contact->blogFeed();
    if (!blog.isEmpty()) {
        output.append("<gContact:website rel=\"blog\" href=\"").append(blog.toUtf8()).append("\" />");
        parsedCustoms << "KADDRESSBOOK-BlogFeed";
    }

    /* Emails */
    Q_FOREACH(const QString &email, contact->emails()) {
        output.append("<gd:email rel='http://schemas.google.com/g/2005#home' address='").append(email.toUtf8()).append("' />");
    }

    /* IMs */
    QString im_str("<gd:im address=\"%1\" protocol=\"%2\" rel=\"http://schemas.google.com/g/2005#other\" primary=\"%3\"/>");
    Q_FOREACH(const QString &im, contact->customs()) {
        if (im.startsWith(QLatin1String("messaging/"))) {
            QString key = im.left(im.indexOf(':'));
            QString value = im.mid(im.indexOf(':') + 1);
            QString proto = key.mid(10);
            proto.chop(4);
            bool primary = (contact->custom("KADDRESSBOOK", "X-IMAddress") == value);
            output.append(im_str.arg(value, Objects::Contact::IMProtocolNameToScheme(proto), (primary ? "true" : "false")).toUtf8());
            parsedCustoms << key;
        }
    }
    parsedCustoms << "KADDRESSBOOK-X-IMAddress";

    /* Phone numbers */
    QString phone_str("<gd:phoneNumber rel=\"%1\">%2</gd:phoneNumber>");
    Q_FOREACH(const KABC::PhoneNumber &number, contact->phoneNumbers()) {
        output.append(phone_str.arg(Objects::Contact::phoneTypeToScheme(number.type()), number.number()).toUtf8());
    }

    /* Address */
    Q_FOREACH(const KABC::Address &address, contact->addresses()) {
        output.append("<gd:structuredPostalAddress rel='")
        .append(Objects::Contact::addressTypeToScheme(address.type()).toUtf8())
        .append("'>");

        if (!address.locality().isEmpty())
            output.append("<gd:city>").append(address.locality().toUtf8()).append("</gd:city>");
        if (!address.street().isEmpty())
            output.append("<gd:street>").append(address.street().toUtf8()).append("</gd:street>");
        if (!address.region().isEmpty())
            output.append("<gd:region>").append(address.region().toUtf8()).append("</gd:region>");
        if (!address.postalCode().isEmpty())
            output.append("<gd:postcode>").append(address.postalCode().toUtf8()).append("</gd:postcode>");
        if (!address.country().isEmpty())
            output.append("<gd:country>").append(address.country().toUtf8()).append("</gd:country>");
        if (!address.formattedAddress().isEmpty())
            output.append("<gd:formattedAddress>").append(address.formattedAddress().toUtf8()).append("</gd:formattedAddress>");
        output.append("</gd:structuredPostalAddress>");
    }

    /* Birthday */
    if (!contact->birthday().isNull() && contact->birthday().isValid()) {
        QString birthday = contact->birthday().toString("yyyy-MM-dd");
        output.append("<gContact:birthday when='").append(birthday.toUtf8()).append("'/>");
    }

    QStringList groups = contact->custom("GCALENDAR", "groupMembershipInfo").split(',');
    if ((groups.length() > 0) && !groups.at(0).isEmpty()) {
        Q_FOREACH(const QString & group, groups) {
            bool removed = contact->groupIsDeleted(group);
            if (!removed)
                output.append(QString("<gContact:groupMembershipInfo deleted=\"false\" href=\"%2\" />").arg(group).toUtf8());
        }
    }
    parsedCustoms << "GCALENDAR-groupMembershipInfo";

    /* User-defined fields */
    QStringList customs = contact->customs();
    QString defined_str("<gContact:userDefinedField key=\"%1\" value=\"%2\" />");
    Q_FOREACH(const QString &customStr, customs) {
        QString key = customStr.left(customStr.indexOf(':'));
        if (!parsedCustoms.contains(key)) {
            QString value = customStr.mid(customStr.indexOf(':') + 1);
            output.append(defined_str.arg(key, value).toUtf8());
        }
    }

    return output;
}

QByteArray ContactsPrivate::groupToXML(const KGAPI::Object *object)
{
    const KGAPI::Objects::ContactsGroup *group = static_cast< const KGAPI::Objects::ContactsGroup* >(object);

    QByteArray output;

    output.append("<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
                  "term=\"http://schemas.google.com/g/2005#group\"/>");
    output.append("<atom:title type=\"text\">").append(group->title().toUtf8()).append("</atom:title>");
    output.append("<atom:content type=\"text\">").append(group->content().toUtf8()).append("</atom:content>");

    return output;
}

KGAPI::Object* ContactsPrivate::XMLToContact(const QDomDocument &doc)
{
    Objects::Contact *contact;
    QStringList groups;

    QDomNodeList entry = doc.elementsByTagName("entry");
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    contact = new Objects::Contact();

    for (int i = 0; i < data.count(); i++) {
        QDomNode n = data.at(i);
        QDomElement e = n.toElement();

        if (e.tagName() == "id") {
            contact->setUid(e.text());
            continue;
        }

        /* ETag */
        if (e.tagName() == "etag") {
            contact->setEtag(e.text());
            continue;
        }

        if (e.tagName() == "gd:name") {
            QDomNodeList l = e.childNodes();
            for (uint i = 0; i < l.length(); i++) {
                QDomElement el = l.at(i).toElement();

                if (el.tagName() == "gd:fullname") {
                    contact->setFormattedName(el.text());
                    continue;
                }

                if (el.tagName() == "gd:givenName") {
                    contact->setGivenName(el.text());
                    continue;
                }

                if (el.tagName() == "gd:familyName") {
                    contact->setFamilyName(el.text());
                    continue;
                }

                if (el.tagName() == "gd:additionalName") {
                    contact->setAdditionalName(el.text());
                    continue;
                }

                if (el.tagName() == "gd:namePrefix") {
                    contact->setPrefix(el.text());
                    continue;
                }

                if (el.tagName() == "gd:nameSuffix") {
                    contact->setSuffix(el.text());
                    continue;
                }

            }
            continue;
        }

        /* If the contact was deleted, we don't need more info about it.
        * Just store our own flag, which will be then parsed by the resource
        * itself. */
        contact->setDeleted(e.tagName() == "gd:deleted");

        /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
        * itself. */
        if (e.tagName() == "link" && e.attribute("rel") == "http://schemas.google.com/contacts/2008/rel#photo") {
            contact->setPhotoUrl(e.attribute("href"));    /* URL */
            continue;
        }

        /* Name */
        if (e.tagName() == "title") {
            contact->setName(e.text());
            continue;
        }

        /* Note */
        if (e.tagName() == "content") {
            contact->setNote(e.text());
            continue;
        }

        /* Organization (work) - KABC supports only organization */
        if (e.tagName() == "gd:organization") {
            QDomNodeList l = e.childNodes();
            for (uint i = 0; i < l.length(); i++) {
                QDomElement el = l.at(i).toElement();

                if (el.tagName() == "gd:orgName") {
                    contact->setOrganization(el.text());
                    continue;
                }

                if (el.tagName() == "gd:orgDepartment") {
                    contact->setDepartment(el.text());
                    continue;
                }

                if (el.tagName() == "gd:orgTitle") {
                    contact->setTitle(el.text());
                    continue;
                }

                if (el.tagName() == "gd:where") {
                    contact->setOffice(el.text());
                    continue;
                }
            }
            continue;
        }

        /* Nickname */
        if (e.tagName() == "gContact:nickname") {
            contact->setNickName(e.text());
            continue;
        }

        /* Occupation (= organization/title) */
        if (e.tagName() == "gContact:occupation") {
            contact->setProfession(e.text());
            continue;
        }

        /* Relationships */
        if (e.tagName() == "gContact:relation") {
            if (e.attribute("rel", "") == "spouse") {
                contact->setSpousesName(e.text());
                continue;
            }

            if (e.attribute("rel", "") == "manager") {
                contact->setManagersName(e.text());
                continue;
            }

            if (e.attribute("rel", "") == "assistant") {
                contact->setAssistantsName(e.text());
                continue;
            }

            continue;
        }

        /* Anniversary */
        if (e.tagName() == "gContact:event") {
            if (e.attribute("rel", "") == "anniversary") {
                QDomElement w = e.firstChildElement("gd:when");
                contact->setAnniversary(w.attribute("startTime", ""));
            }

            continue;
        }

        /* Websites */
        if (e.tagName() == "gContact:website") {
            if (e.attribute("rel", "") == "home-page") {
                contact->setUrl(KUrl(e.attribute("href", "")));
                continue;
            }

            if (e.attribute("rel", "") == "blog") {
                contact->setBlogFeed(e.attribute("href", ""));
                continue;
            }

            continue;
        }

        /* Emails */
        if (e.tagName() == "gd:email") {
            contact->insertEmail(e.attribute("address"), (e.attribute("primary").toLower() == "true"));
            continue;
        }

        /* IMs */
        if (e.tagName() == "gd:im") {
            contact->insertCustom("messaging/" + Objects::Contact::IMSchemeToProtocolName(e.attribute("protocol")), "All", e.attribute("address"));
            continue;
        }

        /* Phone numbers */
        if (e.tagName() == "gd:phoneNumber") {
            contact->insertPhoneNumber(KABC::PhoneNumber(e.text(), Objects::Contact::phoneSchemeToType(e.attribute("rel"))));
            continue;
        }

        /* Addresses */
        /* FIXME */
        if (e.tagName() == "gd:structuredPostalAddress") {
            KABC::Address address;
            QDomNodeList l = e.childNodes();
            for (uint i = 0; i < l.length(); i++) {
                QDomElement el = l.at(i).toElement();

                if (el.tagName() == "gd:street") {
                    address.setStreet(el.text());
                    continue;
                }

                if (el.tagName() == "gd:country") {
                    address.setCountry(el.text());
                    continue;
                }

                if (el.tagName() == "gd:city") {
                    address.setLocality(el.text());
                    continue;
                }

                if (el.tagName() == "gd:postcode") {
                    address.setPostalCode(el.text());
                    continue;
                }

                if (el.tagName() == "gd:region") {
                    address.setRegion(el.text());
                    continue;
                }

                if (el.tagName() == "gd:pobox") {
                    address.setPostOfficeBox(el.text());
                    continue;
                }
            }

            address.setType(Objects::Contact::addressSchemeToType(e.attribute("rel"), (e.attribute("primary") == "true")));
            contact->insertAddress(address);
            continue;
        }

        /* Birthday */
        if (e.tagName() == "gContact:birthday") {
            contact->setBirthday(QDateTime::fromString(e.attribute("when"), "yyyy-MM-dd"));
            continue;
        }

        /* User-defined tags */
        if (e.tagName() == "gContact:userDefinedField") {
            contact->insertCustom("GCALENDAR", e.attribute("key", ""), e.attribute("value", ""));
            continue;
        }

        if (e.tagName() == "gContact:groupMembershipInfo") {
            if (e.hasAttribute("deleted") || e.attribute("deleted").toInt() == false) {
                groups.append(e.attribute("href"));
            }
        }
    }

    contact->insertCustom("GCALENDAR", "groupMembershipInfo", groups.join(","));

    return dynamic_cast< KGAPI::Object* >(contact);
}

Object* ContactsPrivate::XMLToGroup(const QDomDocument &doc)
{
    Objects::ContactsGroup *group;
    QStringList groups;

    QDomNodeList entry = doc.elementsByTagName("entry");
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    group = new Objects::ContactsGroup();

    for (int i = 0; i < data.count(); i++) {
        QDomNode n = data.at(i);
        QDomElement e = n.toElement();

        if (e.tagName() == "id") {
            group->setId(e.text());
            continue;
        }

        if (e.tagName() == "updated") {
            group->setUpdated(KGAPI::AccessManager::RFC3339StringToDate(e.text()).dateTime());
            continue;
        }

        if (e.tagName() == "title") {
            group->setTitle(e.text());
            continue;
        }

        if (e.tagName() == "content") {
            group->setContent(e.text());
            continue;
        }

        if (e.tagName() == "gContact:systemGroup") {
            group->setIsSystemGroup(true);
            continue;
        }
    }

    return dynamic_cast< KGAPI::Object* >(group);
}
