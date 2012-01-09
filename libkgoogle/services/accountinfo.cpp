/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

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

#include <qjson/parser.h>


KGoogle::Services::AccountInfo::AccountInfo()
{

}

const QString KGoogle::Services::AccountInfo::protocolVersion()
{
  return "1";
}

QString KGoogle::Services::AccountInfo::scopeUrl(const QString &propertyName)
{
  return "https://www.googleapis.com/auth/userinfo." + propertyName;
}

QString KGoogle::Services::AccountInfo::fetchUrl()
{
  return "https://www.googleapis.com/oauth2/v1/userinfo";
}

KGoogle::Object *KGoogle::Services::AccountInfo::JSONToObject (const QByteArray &jsonData)
{
  QJson::Parser parser;
  QVariantMap data;
  bool ok;

  data = parser.parse(jsonData, &ok).toMap();
  if (!ok) {
    throw KGoogle::Exception::InvalidResponse();
    return 0;
  }

  KGoogle::Objects::AccountInfo *object = new KGoogle::Objects::AccountInfo();
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

  return dynamic_cast< KGoogle::Object* >(object);
}





