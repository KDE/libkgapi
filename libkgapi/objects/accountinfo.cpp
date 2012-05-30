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

#include "accountinfo.h"
#include "accountinfo_p.h"

using namespace KGAPI::Objects;

AccountInfoData::AccountInfoData(const AccountInfoData &other):
    QSharedData(other),
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
{ }

AccountInfo::AccountInfo():
    KGAPI::Object(),
    d(new AccountInfoData)
{

}

AccountInfo::AccountInfo(const AccountInfo &other):
    KGAPI::Object(other),
    d(other.d)
{ }

AccountInfo::~AccountInfo()
{

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

const QString &AccountInfo::photoUrl() const
{
    return d->pictureUrl;
}
