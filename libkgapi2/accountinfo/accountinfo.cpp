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

#include <qjson/parser.h>
#include <QtCore/QVariantMap>

using namespace KGAPI2;


class AccountInfo::Private
{
public:
    Private();
    Private(const Private &other);
    ~Private();

    QString id;
    QString email;
    QString name;
    QString givenName;
    QString familyName;

    QString birthday;
    QString gender;

    QString link;
    QString locale;
    QString timezone;

    bool verifiedEmail;

    QString pictureUrl;
};

AccountInfo::Private::Private():
    verifiedEmail(false)
{
}

AccountInfo::Private::Private(const Private &other):
    id(other.id),
    email(other.email),
    name(other.name),
    givenName(other.givenName),
    familyName(other.familyName),
    birthday(other.birthday),
    gender(other.gender),
    link(other.link),
    locale(other.locale),
    timezone(other.timezone),
    verifiedEmail(other.verifiedEmail),
    pictureUrl(other.pictureUrl)
{
}

AccountInfo::Private::~Private()
{
}

AccountInfo::AccountInfo():
    Object(),
    d(new Private)
{
}

AccountInfo::AccountInfo(const AccountInfo &other):
    Object(other),
    d(new Private(*(other.d)))
{
}

AccountInfo::~AccountInfo()
{
    delete d;
}

void AccountInfo::setId(const QString &id)
{
    d->id = id;
}

QString AccountInfo::id() const
{
    return d->id;
}

void AccountInfo::setEmail(const QString &email)
{
    d->email = email;
}

QString AccountInfo::email() const
{
    return d->email;
}

void AccountInfo::setName(const QString &name)
{
    d->name = name;
}

QString AccountInfo::name() const
{
    return d->name;
}

void AccountInfo::setGivenName(const QString &givenName)
{
    d->givenName = givenName;
}

QString AccountInfo::givenName() const
{
    return d->givenName;
}

void AccountInfo::setFamilyName(const QString &familyName)
{
    d->familyName = familyName;
}

QString AccountInfo::familyName() const
{
    return d->familyName;
}

void AccountInfo::setBirthday(const QString &birthday)
{
    d->birthday = birthday;
}

QString AccountInfo::birthday() const
{
    return d->birthday;
}

void AccountInfo::setGender(const QString &gender)
{
    d->gender = gender;
}

QString AccountInfo::gender() const
{
    return d->gender;
}

void AccountInfo::setLink(const QString &link)
{
    d->link = link;
}

QString AccountInfo::link() const
{
    return d->link;
}

void AccountInfo::setLocale(const QString &locale)
{
    d->locale = locale;
}

QString AccountInfo::locale() const
{
    return d->locale;
}

void AccountInfo::setTimezone(const QString &timezone)
{
    d->timezone = timezone;
}

QString AccountInfo::timezone() const
{
    return d->timezone;
}

void AccountInfo::setVerifiedEmail(const bool verifiedEmail)
{
    d->verifiedEmail = verifiedEmail;
}

bool AccountInfo::verifiedEmail() const
{
    return d->verifiedEmail;
}

void AccountInfo::setPhotoUrl(const QString &url)
{
    d->pictureUrl = url;
}

QString AccountInfo::photoUrl() const
{
    return d->pictureUrl;
}

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
