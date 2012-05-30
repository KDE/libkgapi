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

#include "services/accountinfo.h"
#include "objects/accountinfo.h"
#include "common.h"

#include <QtCore/QVariantMap>

#include <qjson/parser.h>

using namespace KGAPI;
using namespace Services;

QUrl AccountInfo::ScopeUrl("https://www.googleapis.com/auth/userinfo.profile");
QUrl AccountInfo::EmailScopeUrl("https://www.googleapis.com/auth/userinfo.email");

QString AccountInfo::protocolVersion() const
{
    return "1";
}

const QUrl& AccountInfo::scopeUrl() const
{
    return AccountInfo::ScopeUrl;
}

QUrl AccountInfo::fetchUrl()
{
    return QUrl("https://www.googleapis.com/oauth2/v1/userinfo");
}

KGAPI::Object* AccountInfo::JSONToObject(const QByteArray &jsonData)
{
    QJson::Parser parser;
    QVariantMap data;
    bool ok;

    data = parser.parse(jsonData, &ok).toMap();
    if (!ok) {
        throw Exception::InvalidResponse();
        return 0;
    }

    Objects::AccountInfo *object = new Objects::AccountInfo();
    object->setId(data["id"].toString());
    object->setEmail(data["email"].toString());
    object->setName(data["name"].toString());
    object->setGivenName(data["giveName"].toString());
    object->setFamilyName(data["familyName"].toString());
    object->setBirthday(data["birthday"].toString());
    object->setGender(data["gender"].toString());
    object->setLink(data["link"].toString());
    object->setLocale(data["locale"].toString());
    object->setTimezone(data["timezone"].toString());
    object->setPhotoUrl(data["picture"].toString());
    object->setVerifiedEmail(data["verified_email"].toBool());

    return dynamic_cast< KGAPI::Object* >(object);
}

QByteArray AccountInfo::objectToJSON(Object* object)
{
    Q_UNUSED(object);

    return QByteArray();
}

QList< Object* > AccountInfo::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    Q_UNUSED(jsonFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object *>();
}

QList< Object* > AccountInfo::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object * >();
}

QByteArray AccountInfo::objectToXML(Object* object)
{
    Q_UNUSED(object);

    return QByteArray();
};

Object* AccountInfo::XMLToObject(const QByteArray& xmlData)
{
    Q_UNUSED(xmlData)

    return 0;
}

