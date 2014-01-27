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

#include <libkgapi2/contacts/contactsservice.h>
#include <libkgapi2/contacts/contact.h>
#include <libkgapi2/contacts/contactsgroup.h>
#include <libkgapi2/account.h>

using namespace KGAPI;
using namespace KGAPI::Services;

QUrl Contacts::ScopeUrl(KGAPI2::Account::contactsScopeUrl());

static const QString serviceNameStr = QLatin1String("KGAPI::Services::Contacts");


QString Contacts::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Contacts >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}

QByteArray Contacts::objectToJSON(KGAPI::Object* object)
{
    /* Google allows writing only XML data */
    return QByteArray();

    Q_UNUSED(object)
}

QByteArray Contacts::objectToXML(KGAPI::Object* object)
{
    if (!object) {
        return QByteArray();
    }

    if (dynamic_cast< KGAPI::Objects::Contact* >(object)) {
        KGAPI2::ContactPtr contact(new KGAPI2::Contact(*reinterpret_cast<KGAPI2::Contact*>(object)));
        return KGAPI2::ContactsService::contactToXML(contact);
    } else if (dynamic_cast< KGAPI::Objects::ContactsGroup* >(object)) {
        KGAPI2::ContactsGroupPtr group(new KGAPI2::ContactsGroup(*reinterpret_cast<KGAPI2::ContactsGroup*>(object)));
        return KGAPI2::ContactsService::contactsGroupToXML(group);
    }

    return QByteArray();
}


QList< KGAPI::Object* > Contacts::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QList< KGAPI::Object* > list;

    KGAPI2::ObjectsList objects = KGAPI2::ContactsService::parseJSONFeed(jsonFeed, feedData);
    if (objects.isEmpty()) {
        return list;
    }

    bool isGroup = (objects.first().dynamicCast<KGAPI2::ContactsGroup>() != 0);
    Q_FOREACH(const KGAPI2::ObjectPtr &object, objects) {
        if (isGroup) {
            KGAPI2::ContactsGroupPtr group = object.dynamicCast<KGAPI2::ContactsGroup>();
            list << new Objects::ContactsGroup(*reinterpret_cast<Objects::ContactsGroup*>(group.data()));
        } else {
            KGAPI2::ContactPtr contact = object.dynamicCast<KGAPI2::Contact>();
            list << new Objects::Contact(*reinterpret_cast<Objects::Contact*>(contact.data()));
        }
    }

    return list;
}

Object *Contacts::JSONToObject(const QByteArray &jsonData)
{
    QJson::Parser parser;

    const QVariantMap data = parser.parse(jsonData).toMap();
    const QVariantMap entry = data.value(QLatin1String("entry")).toMap();
    const QVariantList categories = entry.value(QLatin1String("category")).toList();

    Q_FOREACH(const QVariant &c, categories) {
        const QVariantMap category = c.toMap();

        if (category.value(QLatin1String("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#group")) {
            KGAPI2::ContactsGroupPtr group = KGAPI2::ContactsService::JSONToContactsGroup(jsonData);
            return new Objects::ContactsGroup(*reinterpret_cast<Objects::ContactsGroup*>(group.data()));
        } else if (category.value(QLatin1String("term")).toString() == QLatin1String("http://schemas.google.com/contact/2008#contact")) {
            KGAPI2::ContactPtr contact = KGAPI2::ContactsService::JSONToContact(jsonData);
            return new Objects::Contact(*reinterpret_cast<Objects::Contact*>(contact.data()));
        }
    }

    return 0;
}

KGAPI::Object* Contacts::XMLToObject(const QByteArray& xmlData)
{
    QByteArray xmlDoc;
    /* Document without <xml> header is not valid and Qt won't parse it */
    if (!xmlData.contains("<?xml")) {
        xmlDoc.append("<?xml version='1.0' encoding='UTF-8'?>");
    }
    xmlDoc.append(xmlData);

    QDomDocument doc;
    doc.setContent(xmlDoc);
    const QDomNodeList entry = doc.elementsByTagName(QLatin1String("entry"));
    QDomNodeList data;
    if (entry.length() > 0) {
        data = entry.at(0).childNodes();
    } else {
        return 0;
    }

    bool isGroup = false;

    for (int i = 0; i < data.count(); i++) {
        const QDomNode n = data.at(i);
        const QDomElement e = n.toElement();

        if (((e.tagName() == QLatin1String("category")) &&
	     (e.attribute(QLatin1String("term")) == QLatin1String("http://schemas.google.com/contact/2008#group"))) ||
            ((e.tagName() == QLatin1String("atom:category")) &&
	     (e.attribute(QLatin1String("term")) == QLatin1String("http://schemas.google.com/g/2005#group"))))
	{
            isGroup = true;
            break;
        }
    }

    if (isGroup) {
        KGAPI2::ContactsGroupPtr group = KGAPI2::ContactsService::XMLToContactsGroup(xmlData);
        return new Objects::ContactsGroup(*reinterpret_cast<Objects::ContactsGroup*>(group.data()));
    } else {
        KGAPI2::ContactPtr contact = KGAPI2::ContactsService::XMLToContact(xmlData);
        return new Objects::Contact(*reinterpret_cast<Objects::Contact*>(contact.data()));
    }
}

QList< KGAPI::Object* > Contacts::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object* >();
}

QUrl Contacts::scopeUrl() const
{
    return Contacts::ScopeUrl;
}

QUrl Contacts::fetchAllContactsUrl(const QString& user, const bool &showDeleted)
{
    return KGAPI2::ContactsService::fetchAllContactsUrl(user, showDeleted);
}

QUrl Contacts::fetchContactUrl(const QString& user, const QString& contactID)
{
    return KGAPI2::ContactsService::fetchContactUrl(user, contactID);
}

QUrl Contacts::createContactUrl(const QString& user)
{
    return KGAPI2::ContactsService::createContactUrl(user);
}

QUrl Contacts::updateContactUrl(const QString& user, const QString& contactID)
{
    return KGAPI2::ContactsService::updateContactUrl(user, contactID);
}

QUrl Contacts::removeContactUrl(const QString& user, const QString& contactID)
{
    return KGAPI2::ContactsService::removeContactUrl(user, contactID);
}

QUrl Contacts::fetchAllGroupsUrl(const QString &user)
{
    return KGAPI2::ContactsService::fetchAllGroupsUrl(user);
}

QUrl Contacts::fetchGroupUrl(const QString &user, const QString &groupId)
{
    return KGAPI2::ContactsService::fetchGroupUrl(user, groupId);
}

QUrl Contacts::createGroupUrl(const QString &user)
{
    return KGAPI2::ContactsService::createGroupUrl(user);
}

QUrl Contacts::updateGroupUrl(const QString &user, const QString &groupId)
{
    return KGAPI2::ContactsService::updateGroupUrl(user, groupId);
}

QUrl Contacts::removeGroupUrl(const QString &user, const QString &groupId)
{
    return KGAPI2::ContactsService::removeGroupUrl(user, groupId);
}

QUrl Contacts::photoUrl(const QString& user, const QString& contactID)
{
    return KGAPI2::ContactsService::photoUrl(user, contactID);
}

QString Contacts::protocolVersion() const
{
    return KGAPI2::ContactsService::APIVersion();
}

bool Contacts::supportsJSONRead(QString* urlParam)
{
    if (urlParam)
        *urlParam = QLatin1String("json");

    return true;
}

bool Contacts::supportsJSONWrite(QString* urlParam)
{
    Q_UNUSED(urlParam)

    return false;
}
