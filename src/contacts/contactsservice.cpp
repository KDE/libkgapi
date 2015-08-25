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

#include "contactsservice.h"
#include "contact.h"
#include "contactsgroup.h"
#include "../debug.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QJsonDocument>

/* Qt::escape() */
#include <QTextDocument>


namespace KGAPI2
{

namespace ContactsService
{

namespace Private
{
    QString stringFromXMLMap(const QVariantMap &map, const QString &key) {
        const QVariantMap t = map.value(key).toMap();
        return t.value(QStringLiteral("$t")).toString();
    }

    ObjectPtr JSONToContactsGroup(const QVariantMap &map);
    ObjectPtr JSONToContact(const QVariantMap& map);

    static const QUrl GoogleApisUrl(QStringLiteral("https://www.google.com"));
    static const QString ContactsBasePath(QStringLiteral("/m8/feeds/contacts"));
    static const QString ContactsGroupBasePath(QStringLiteral("/m8/feeds/groups"));
    static const QString PhotoBasePath(QStringLiteral("/m8/feeds/photos/media"));
}

ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    ObjectsList output;
    QJsonDocument document = QJsonDocument::fromJson(jsonFeed);
    const QVariantMap head = document.toVariant().toMap();
    const QVariantMap feed = head.value(QStringLiteral("feed")).toMap();
    const QVariantList categories = feed.value(QStringLiteral("category")).toList();
    Q_FOREACH(const QVariant &c, categories) {
        const QVariantMap category = c.toMap();
        bool groups = false;

        if (category.value(QStringLiteral("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#group")) {
            groups = true;
        }

        const QVariantList entries = feed.value(QStringLiteral("entry")).toList();
        Q_FOREACH(const QVariant &e, entries) {
            if (groups) {
                output << Private::JSONToContactsGroup(e.toMap());
            } else {
                output << Private::JSONToContact(e.toMap());
            }
        }
    }

    const QVariantList links = feed.value(QStringLiteral("link")).toList();
    Q_FOREACH(const QVariant &l, links) {
        const QVariantMap link = l.toMap();
        if (link.value(QStringLiteral("rel")).toString() == QLatin1String("next")) {
            feedData.nextPageUrl = QUrl(link.value(QStringLiteral("href")).toString());
            break;
        }
    }

    feedData.totalResults = Private::stringFromXMLMap(feed, QStringLiteral("openSearch$totalResults")).toInt();
    feedData.startIndex = Private::stringFromXMLMap(feed, QStringLiteral("openSearch$startIndex")).toInt();
    feedData.itemsPerPage = Private::stringFromXMLMap(feed, QStringLiteral("openSearch$itemsPerPage")).toInt();

    return output;
}

QUrl fetchAllContactsUrl(const QString& user, const bool &showDeleted)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ContactsBasePath % QLatin1Char('/') % user % QLatin1String("/full"));
    url.addQueryItem(QStringLiteral("alt"), QStringLiteral("json"));
    if (showDeleted) {
        url.addQueryItem(QStringLiteral("showdeleted"), QStringLiteral("true"));
    }

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
    url.addQueryItem(QStringLiteral("alt"), QStringLiteral("json"));
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
    url.addQueryItem(QStringLiteral("alt"), QStringLiteral("json"));
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
    url.addQueryItem(QStringLiteral("alt"), QStringLiteral("json"));

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

ObjectPtr Private::JSONToContactsGroup(const QVariantMap& data)
{
    ContactsGroupPtr group(new ContactsGroup);

    group->setId(Private::stringFromXMLMap(data, QStringLiteral("id")));
    group->setEtag(data.value(QStringLiteral("gd$etag")).toString());

    group->setTitle(Private::stringFromXMLMap(data, QStringLiteral("title")));
    group->setContent(Private::stringFromXMLMap(data, QStringLiteral("content")));

    group->setUpdated(QDateTime::fromString(Private::stringFromXMLMap(data, QStringLiteral("updated")), Qt::ISODate));

    if (data.contains(QStringLiteral("gContact$systemGroup"))) {
        group->setIsSystemGroup(true);
    } else {
        group->setIsSystemGroup(false);
    }

    return group;
}

ContactsGroupPtr JSONToContactsGroup(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    const QVariantMap data = document.toVariant().toMap();
    const QVariantMap entry = data.value(QStringLiteral("entry")).toMap();
    const QVariantList categories = entry.value(QStringLiteral("category")).toList();

    bool isGroup = false;
    Q_FOREACH(const QVariant &c, categories) {
        const QVariantMap category = c.toMap();

        if (category.value(QStringLiteral("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#group")) {
            isGroup = true;
            break;
        }
    }
    if (!isGroup) {
        return ContactsGroupPtr();
    }

    return Private::JSONToContactsGroup(entry).staticCast<ContactsGroup>();
}

ObjectPtr Private::JSONToContact(const QVariantMap& data)
{
    ContactPtr contact(new Contact);

    /* Google contact ID */
    contact->setUid(Private::stringFromXMLMap(data, QStringLiteral("id")));

    /* Google ETAG. This can be used to identify if the item was changed remotly */
    contact->setEtag(data.value(QStringLiteral("gd$etag")).toString());

    /* Date and time when contact was updated on the remote server */
    contact->setUpdated(QDateTime::fromString(Private::stringFromXMLMap(data, QStringLiteral("updated")), Qt::ISODate));

    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    contact->setDeleted(data.value(QStringLiteral("gd$deleted")).isValid());

    /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
     * itself. */
    const QVariantList links = data.value(QStringLiteral("link")).toList();
    Q_FOREACH(const QVariant &link, links) {
        const QVariantMap linkMap = link.toMap();
        if (linkMap.value(QStringLiteral("rel")).toString() == QLatin1String("http://schemas.google.com/contacts/2008/rel#photo")) {
            contact->setPhotoUrl(linkMap.value(QStringLiteral("href")).toString());
        }
    }

    /* Name */
    if (data.contains(QStringLiteral("title"))) {
        contact->setName(Private::stringFromXMLMap(data, QStringLiteral("title")));
    }

    /* Formatted name */
    if (data.contains(QStringLiteral("gd$name"))) {
        const QVariantMap name = data.value(QStringLiteral("gd$name")).toMap();

        if (name.contains(QStringLiteral("gd$fullName"))) {
            contact->setFormattedName(Private::stringFromXMLMap(name, QStringLiteral("gd$fullName")));
        }
        if (name.contains(QStringLiteral("gd$givenName"))) {
            contact->setGivenName(Private::stringFromXMLMap(name, QStringLiteral("gd$givenName")));
        }
        if (name.contains(QStringLiteral("gd$familyName"))) {
            contact->setFamilyName(Private::stringFromXMLMap(name, QStringLiteral("gd$familyName")));
        }
        if (name.contains(QStringLiteral("gd$additionalName"))) {
            contact->setAdditionalName(Private::stringFromXMLMap(name, QStringLiteral("gd$additionalName")));
        }
        if (name.contains(QStringLiteral("gd$namePrefix"))) {
            contact->setPrefix(Private::stringFromXMLMap(name, QStringLiteral("gd$namePrefix")));
        }
        if (name.contains(QStringLiteral("gd$nameSuffix"))) {
            contact->setSuffix(Private::stringFromXMLMap(name, QStringLiteral("gd$nameSuffix")));
        }
    }

    /* Note */
    if (data.contains(QStringLiteral("content"))) {
        contact->setNote(Private::stringFromXMLMap(data, QStringLiteral("content")));
    }

    /* Organization (work) - KABC supports only one organization */
    if (data.contains(QStringLiteral("gd$organization"))) {
        const QVariantList organizations = data.value(QStringLiteral("gd$organization")).toList();
        const QVariantMap organization = organizations.first().toMap();

        if (organization.contains(QStringLiteral("gd$orgName"))) {
            contact->setOrganization(Private::stringFromXMLMap(organization, QStringLiteral("gd$orgName")));
        }
        if (organization.contains(QStringLiteral("gd$orgDepartment"))) {
            contact->setDepartment(Private::stringFromXMLMap(organization, QStringLiteral("gd$orgDepartment")));
        }
        if (organization.contains(QStringLiteral("gd$orgTitle"))) {
            contact->setTitle(Private::stringFromXMLMap(organization, QStringLiteral("gd$orgTitle")));
        }
        if (organization.contains(QStringLiteral("gd$where"))) {
            contact->setOffice(Private::stringFromXMLMap(organization, QStringLiteral("gd$where")));
        }
    }

    /* Nickname */
    if (data.contains(QStringLiteral("gContact$nickname"))) {
        contact->setNickName(Private::stringFromXMLMap(data, QStringLiteral("gContact$nickname")));
    }

    /* Occupation (= organization/title) */
    if (data.contains(QStringLiteral("gContact$occupation"))) {
        contact->setProfession(Private::stringFromXMLMap(data, QStringLiteral("gContact$occupation")));
    }

    /* Relationships */
    if (data.contains(QStringLiteral("gContact$relation"))) {
        const QVariantList relations = data.value(QStringLiteral("gContact$relation")).toList();
        Q_FOREACH(const QVariant &r, relations) {
            const QVariantMap relation = r.toMap();
            if (relation.value(QStringLiteral("rel")).toString() == QLatin1String("spouse")) {
                contact->setSpousesName(relation.value(QStringLiteral("$t")).toString());
                continue;
            }

            if (relation.value(QStringLiteral("rel")).toString() == QLatin1String("manager")) {
                contact->setManagersName(relation.value(QStringLiteral("$t")).toString());
                continue;
            }

            if (relation.value(QStringLiteral("rel")).toString() == QLatin1String("assistant")) {
                contact->setAssistantsName(relation.value(QStringLiteral("$t")).toString());
                continue;
            }
        }
    }

    /* Anniversary */
    if (data.contains(QStringLiteral("gContact$event"))) {
        const QVariantList events = data.value(QStringLiteral("gContact$event")).toList();
        Q_FOREACH(const QVariant &e, events) {
            const QVariantMap event = e.toMap();

            if (event.value(QStringLiteral("rel")).toString() == QLatin1String("anniversary")) {
                QVariantMap when = event.value(QStringLiteral("gd$when")).toMap();
                contact->setAnniversary(when.value(QStringLiteral("startTime")).toString());
            }
        }
    }

    /* Websites */
    if (data.contains(QStringLiteral("gContact$website"))) {
        const QVariantList websites = data.value(QStringLiteral("gContact$website")).toList();
        Q_FOREACH(const QVariant &w, websites) {
            const QVariantMap web = w.toMap();

            if (web.value(QStringLiteral("rel")).toString() == QLatin1String("home-page")) {
                KContacts::ResourceLocatorUrl url;
                url.setUrl(QUrl(web.value(QStringLiteral("href")).toString()));
                contact->setUrl(url);
                continue;
            }

            if (web.value(QStringLiteral("rel")).toString() == QLatin1String("blog")) {
                contact->setBlogFeed(web.value(QStringLiteral("href")).toString());
                continue;
            }
        }
    }

    /* Emails */
    const QVariantList emails = data.value(QStringLiteral("gd$email")).toList();
    Q_FOREACH(const QVariant & em, emails) {
        const QVariantMap email = em.toMap();
        contact->insertEmail(email.value(QStringLiteral("address")).toString(),
                            email.value(QStringLiteral("primary")).toBool());
    }

    /* IMs */
    const QVariantList ims = data.value(QStringLiteral("gd$im")).toList();
    Q_FOREACH(const QVariant & i, ims) {
        const QVariantMap im = i.toMap();
        const QString protocol = Contact::IMSchemeToProtocolName(im.value(QStringLiteral("protocol")).toString());
        contact->insertCustom(QLatin1String("messaging/") + protocol,
                             QStringLiteral("All"),
                             im.value(QStringLiteral("address")).toString());
    }

    /* Phone numbers */
    const QVariantList phones = data.value(QStringLiteral("gd$phoneNumber")).toList();
    Q_FOREACH(const QVariant & p, phones) {
        const QVariantMap phone = p.toMap();
        contact->insertPhoneNumber(KContacts::PhoneNumber(phone.value(QStringLiteral("$t")).toString(),
                                  Contact::phoneSchemeToType(phone.value(QStringLiteral("rel")).toString())));
    }

    /* Addresses */
    const QVariantList addresses = data.value(QStringLiteral("gd$structuredPostalAddress")).toList();
    Q_FOREACH(const QVariant & a, addresses) {
        const QVariantMap address = a.toMap();
        KContacts::Address addr;
        if (!address.contains(QStringLiteral("gd$city")) &&
                !address.contains(QStringLiteral("gd$country")) &&
                !address.contains(QStringLiteral("gd$postcode")) &&
                !address.contains(QStringLiteral("gd$region")) &&
                !address.contains(QStringLiteral("gd$pobox")))
        {
            addr.setExtended(Private::stringFromXMLMap(address, QStringLiteral("gd$street")));
        } else {
            if (address.contains(QStringLiteral("gd$street"))) {
                addr.setStreet(Private::stringFromXMLMap(address, QStringLiteral("gd$street")));
            }
            if (address.contains(QStringLiteral("gd$country"))) {
                addr.setCountry(Private::stringFromXMLMap(address, QStringLiteral("gd$country")));
            }
            if (address.contains(QStringLiteral("gd$city"))) {
                addr.setLocality(Private::stringFromXMLMap(address, QStringLiteral("gd$city")));
            }
            if (address.contains(QStringLiteral("gd$postcode"))) {
                addr.setPostalCode(Private::stringFromXMLMap(address, QStringLiteral("gd$postcode")));
            }
            if (address.contains(QStringLiteral("gdregion"))) {
                addr.setRegion(Private::stringFromXMLMap(address, QStringLiteral("gd$region")));
            }
            if (address.contains(QStringLiteral("gd$pobox"))) {
                addr.setPostOfficeBox(Private::stringFromXMLMap(address, QStringLiteral("gd$pobox")));
            }
        }
        addr.setType(Contact::addressSchemeToType(address.value(QStringLiteral("rel")).toString()));

        contact->insertAddress(addr);
    }

    /* Birthday */
    const QVariantMap bDay = data.value(QStringLiteral("gContact$birthday")).toMap();
    if (!bDay.isEmpty()) {
        QString birthday = bDay.value(QStringLiteral("when")).toString();
        /* Birthdays in format "--MM-DD" are valid and mean that no year has
         * been specified. Since KABC does not support birthdays without year,
         * we simulate that by specifying a fake year - 1900 */
        if (birthday.startsWith(QLatin1String("--"))) {
            birthday = QLatin1String("1900") + birthday.mid(1);
        }
        contact->setBirthday(QDateTime::fromString(birthday, QStringLiteral("yyyy-MM-dd")));
    }

    /* User-defined fields */
    const QVariantList userDefined = data.value(QStringLiteral("gContact$userDefinedField")).toList();
    Q_FOREACH(const QVariant & u, userDefined) {
        const QVariantMap field = u.toMap();
        contact->insertCustom(QStringLiteral("KADDRESSBOOK"),
                             field.value(QStringLiteral("key")).toString(),
                             field.value(QStringLiteral("value")).toString());
    }

    /* Groups */
    const QVariantList groups = data.value(QStringLiteral("gContact$groupMembershipInfo")).toList();
    QStringList groupsList;
    Q_FOREACH(const QVariant & g, groups) {
        const QVariantMap group = g.toMap();
        if (group.value(QStringLiteral("deleted")).toBool() == false) {
            groupsList.append(group.value(QStringLiteral("href")).toString());
        }
    }
    contact->insertCustom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo"),
                         groupsList.join(QStringLiteral(",")));

    return contact;
}

ContactPtr JSONToContact(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    const QVariantMap data = document.toVariant().toMap();
    const QVariantMap entry = data.value(QStringLiteral("entry")).toMap();
    const QVariantList categories = entry.value(QStringLiteral("category")).toList();

    bool isContact = false;
    Q_FOREACH(const QVariant &c, categories) {
        const QVariantMap category = c.toMap();

        if (category.value(QStringLiteral("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#contact")) {
            isContact = true;
            break;
        }
    }
    if (!isContact) {
        return ContactPtr();
    }

    return Private::JSONToContact(entry).staticCast<Contact>();
}


QByteArray contactToXML(const ContactPtr& contact)
{
    QByteArray output;

    QStringList parsedCustoms;

    /* Name */
    output.append("<gd:name>");
    if (!contact->givenName().isEmpty()) {
        output.append("<gd:givenName>").append(Qt::escape(contact->givenName()).toUtf8()).append("</gd:givenName>");
    }
    if (!contact->familyName().isEmpty()) {
        output.append("<gd:familyName>").append(Qt::escape(contact->familyName()).toUtf8()).append("</gd:familyName>");
    }
    if (!contact->assembledName().isEmpty()) {
        output.append("<gd:fullName>").append(Qt::escape(contact->assembledName()).toUtf8()).append("</gd:fullName>");
    }
    if (!contact->additionalName().isEmpty()) {
        output.append("<gd:additionalName>").append(Qt::escape(contact->additionalName()).toUtf8()).append("</gd:additionalName>");
    }
    if (!contact->prefix().isEmpty()) {
        output.append("<gd:namePrefix>").append(Qt::escape(contact->prefix()).toUtf8()).append("</gd:namePrefix>");
    }
    if (!contact->suffix().isEmpty()) {
        output.append("<gd:nameSuffix>").append(Qt::escape(contact->suffix()).toUtf8()).append("</gd:nameSuffix>");
    }
    output.append("</gd:name>");

    /* Notes */
    if (!contact->note().isEmpty()) {
        output.append("<atom:content type='text'>").append(Qt::escape(contact->note()).toUtf8()).append("</atom:content>");
    }

    /* Organization (work) */
    QByteArray org;
    const QString office = contact->office();
    if (!contact->organization().isEmpty()) {
        org.append("<gd:orgName>").append(Qt::escape(contact->organization()).toUtf8()).append("</gd:orgName>");
    }
    if (!contact->department().isEmpty()) {
        org.append("<gd:orgDepartment>").append(Qt::escape(contact->department()).toUtf8()).append("</gd:orgDepartment>");
    }
    if (!contact->title().isEmpty()) {
        org.append("<gd:orgTitle>").append(Qt::escape(contact->title()).toUtf8()).append("</gd:orgTitle>");
    }
    if (!office.isEmpty()) {
        org.append("<gd:where>").append(Qt::escape(office).toUtf8()).append("</gd:where>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-Office");
    }
    if (!org.isEmpty()) {
        output.append("<gd:organization rel=\"http://schemas.google.com/g/2005#work\">").append(org).append("</gd:organization>");
    }

    /* Nickname */
    if (!contact->nickName().isEmpty()) {
        output.append("<gContact:nickname>").append(Qt::escape(contact->nickName()).toUtf8()).append("</gContact:nickname>");
    }

    /* Occupation */
    if (!contact->profession().isEmpty()) {
        output.append("<gContact:occupation>").append(Qt::escape(contact->profession()).toUtf8()).append("</gContact:occupation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-Profession");
    }

    /* Spouse */
    const QString spouse = contact->spousesName();
    if (!spouse.isEmpty()) {
        output.append("<gContact:relation rel=\"spouse\">").append(Qt::escape(spouse).toUtf8()).append("</gContact:relation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-SpousesName");
    }

    /* Manager */
    const QString manager = contact->managersName();
    if (!manager.isEmpty()) {
        output.append("<gContact:relation rel=\"manager\">").append(Qt::escape(manager).toUtf8()).append("</gContact:relation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-ManagersName");
    }

    /* Assistant */
    const QString assistant = contact->assistantsName();
    if (!assistant.isEmpty()) {
        output.append("<gContact:relation rel=\"assistant\">").append(Qt::escape(assistant).toUtf8()).append("</gContact:relation>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-AssistantsName");
    }

    /* Anniversary */
    const QString anniversary = contact->anniversary();
    if (!anniversary.isEmpty()) {
        output.append("<gContact:event rel=\"anniversary\"><gd:when startTime=\"").append(Qt::escape(anniversary).toUtf8()).append("\" /></gContact:event>");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-X-Anniversary");
    }

    /* Homepage */
    if (!contact->url().url().isEmpty()) {
        output.append("<gContact:website rel=\"home-page\" href=\"").append(Qt::escape(contact->url().toString()).toUtf8()).append("\" />");
    }

    /* Blog */
    const QString blog = contact->blogFeed();
    if (!blog.isEmpty()) {
        output.append("<gContact:website rel=\"blog\" href=\"").append(Qt::escape(blog).toUtf8()).append("\" />");
        parsedCustoms << QStringLiteral("KADDRESSBOOK-BlogFeed");
    }

    /* Emails */
    Q_FOREACH(const QString &email, contact->emails()) {
        output.append("<gd:email rel='http://schemas.google.com/g/2005#home' address='").append(Qt::escape(email).toUtf8()).append("' />");
    }

    /* IMs */
    const QString im_str = QStringLiteral("<gd:im address=\"%1\" protocol=\"%2\" rel=\"http://schemas.google.com/g/2005#other\" primary=\"%3\"/>");
    Q_FOREACH(const QString &im, contact->customs()) {
        if (im.startsWith(QLatin1String("messaging/"))) {
            QString key = im.left(im.indexOf(QLatin1Char(':')));
            QString value = im.mid(im.indexOf(QLatin1Char(':')) + 1);
            QString proto = key.mid(10);
            proto.chop(4);
            bool primary = (contact->custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-IMAddress")) == value);
            output.append(im_str.arg(value, Contact::IMProtocolNameToScheme(proto),
                                     (primary ? QStringLiteral("true") : QStringLiteral("false"))).toUtf8());
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
            output.append("<gd:city>").append(Qt::escape(address.locality()).toUtf8()).append("</gd:city>");
        if (!address.street().isEmpty())
            output.append("<gd:street>").append(Qt::escape(address.street()).toUtf8()).append("</gd:street>");
        if (!address.region().isEmpty())
            output.append("<gd:region>").append(Qt::escape(address.region()).toUtf8()).append("</gd:region>");
        if (!address.postalCode().isEmpty())
            output.append("<gd:postcode>").append(Qt::escape(address.postalCode()).toUtf8()).append("</gd:postcode>");
        if (!address.country().isEmpty())
            output.append("<gd:country>").append(Qt::escape(address.country()).toUtf8()).append("</gd:country>");
        if (!address.formattedAddress().isEmpty())
            output.append("<gd:formattedAddress>").append(Qt::escape(address.formattedAddress()).toUtf8()).append("</gd:formattedAddress>");
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
    if ((groups.length() > 0) && !groups.at(0).isEmpty()) {
        Q_FOREACH(const QString & group, groups) {
            bool removed = contact->groupIsDeleted(group);
            if (!removed)
                output.append(QStringLiteral("<gContact:groupMembershipInfo deleted=\"false\" href=\"%2\" />").arg(group).toUtf8());
        }
    }
    parsedCustoms << QStringLiteral("GCALENDAR-groupMembershipInfo");

    /* User-defined fields */
    const QStringList customs = contact->customs();
    const QString defined_str = QStringLiteral("<gContact:userDefinedField key=\"%1\" value=\"%2\" />");
    Q_FOREACH(const QString &customStr, customs) {
        QString key = customStr.left(customStr.indexOf(QLatin1Char(':')));
        if (!parsedCustoms.contains(key)) {
            if (key.startsWith(QLatin1String("KADDRESSBOOK-"))) {
                key = key.remove(QStringLiteral("KADDRESSBOOK-"));
            }
            const QString value = customStr.mid(customStr.indexOf(QLatin1Char(':')) + 1);
            output.append(defined_str.arg(Qt::escape(key), Qt::escape(value)).toUtf8());
        }
    }

    return output;
}

QByteArray contactsGroupToXML(const ContactsGroupPtr& group)
{
    QByteArray output;

    output.append("<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
                  "term=\"http://schemas.google.com/g/2005#group\"/>");
    output.append("<atom:title type=\"text\">").append(Qt::escape(group->title()).toUtf8()).append("</atom:title>");
    output.append("<atom:content type=\"text\">").append(Qt::escape(group->content()).toUtf8()).append("</atom:content>");

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
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return ContactPtr();
    }

    bool isGroup = false;
    for (int i = 0; i < data.count(); ++i) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (((e.tagName() == QLatin1String("category")) &&
             (e.attribute(QStringLiteral("term")) == QLatin1String("http://schemas.google.com/contact/2008#group"))) ||
            ((e.tagName() == QLatin1String("atom:category")) &&
             (e.attribute(QStringLiteral("term")) == QLatin1String("http://schemas.google.com/g/2005#group"))))
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

                if (el.tagName() == QLatin1String("gd:fullname")) {
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

        /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture
        * itself. */
        if ((e.tagName() == QLatin1String("link")) &&
            (e.attribute(QStringLiteral("rel")) == QLatin1String("http://schemas.google.com/contacts/2008/rel#photo"))) {
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
                contact->setAnniversary(w.attribute(QStringLiteral("startTime"), QString()));
            }

            continue;
        }

        /* Websites */
        if (e.tagName() == QLatin1String("gContact:website")) {
            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("home-page")) {
                KContacts::ResourceLocatorUrl url;
                url.setUrl(QUrl(e.attribute(QStringLiteral("href"), QString())));
                contact->setUrl(url);
                continue;
            }

            if (e.attribute(QStringLiteral("rel"), QString()) == QLatin1String("blog")) {
                contact->setBlogFeed(e.attribute(QStringLiteral("href"), QString()));
                continue;
            }

            continue;
        }

        /* Emails */
        if (e.tagName() == QLatin1String("gd:email")) {
            contact->insertEmail(e.attribute(QStringLiteral("address")),
                                 (e.attribute(QStringLiteral("primary")).toLower() == QLatin1String("true")));
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
            contact->insertPhoneNumber(KContacts::PhoneNumber(e.text(), Contact::phoneSchemeToType(e.attribute(QStringLiteral("rel")))));
            continue;
        }

        /* Addresses */
        if (e.tagName() == QLatin1String("gd:structuredPostalAddress")) {
            KContacts::Address address;
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

    contact->insertCustom(QStringLiteral("GCALENDAR"), QStringLiteral("groupMembershipInfo"), groups.join(QStringLiteral(",")));

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
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return ContactsGroupPtr();
    }

    bool isGroup = false;
    for (int i = 0; i < data.count(); ++i) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (((e.tagName() == QLatin1String("category")) &&
             (e.attribute(QStringLiteral("term")) == QLatin1String("http://schemas.google.com/contact/2008#group"))) ||
            ((e.tagName() == QLatin1String("atom:category")) &&
             (e.attribute(QStringLiteral("term")) == QLatin1String("http://schemas.google.com/g/2005#group"))))
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
