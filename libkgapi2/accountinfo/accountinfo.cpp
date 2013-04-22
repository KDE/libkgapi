/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "accountinfo.h"

using namespace KGAPI2;

#include "common/accountinfo.inc.cpp"

#include <qjson/parser.h>
#include <QtCore/QVariantMap>

AccountInfoPtr AccountInfo::fromJSON(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QVariantMap data;
    bool ok;

    data = parser.parse(jsonData, &ok).toMap();
    if (!ok) {
        return AccountInfoPtr();
    }

    AccountInfoPtr accountInfo(new AccountInfo);
    accountInfo->setId(data.value(QLatin1String("id")).toString());
    accountInfo->setEmail(data.value(QLatin1String("email")).toString());
    accountInfo->setName(data.value(QLatin1String("name")).toString());
    accountInfo->setGivenName(data.value(QLatin1String("given_name")).toString());
    accountInfo->setFamilyName(data.value(QLatin1String("family_name")).toString());
    accountInfo->setBirthday(data.value(QLatin1String("birthday")).toString());
    accountInfo->setGender(data.value(QLatin1String("gender")).toString());
    accountInfo->setLink(data.value(QLatin1String("link")).toString());
    accountInfo->setLocale(data.value(QLatin1String("locale")).toString());
    accountInfo->setTimezone(data.value(QLatin1String("timezone")).toString());
    accountInfo->setPhotoUrl(data.value(QLatin1String("picture")).toString());
    accountInfo->setVerifiedEmail(data.value(QLatin1String("verified_email")).toBool());

    return accountInfo;
}
