/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "user.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN User::Private
{
  public:
    Private();
    Private(const Private &other);

    QString displayName;
    QUrl pictureUrl;
    bool isAuthenticatedUser;
    QString permissionId;
};

User::Private::Private():
    isAuthenticatedUser(false)
{
}

User::Private::Private(const Private &other):
    displayName(other.displayName),
    pictureUrl(other.pictureUrl),
    isAuthenticatedUser(other.isAuthenticatedUser),
    permissionId(other.permissionId)
{
}

User::User():
    d(new Private)
{
}

User::User(const User &other):
    d(new Private(*(other.d)))
{
}

User::~User()
{
    delete d;
}

QString User::displayName() const
{
    return d->displayName;
}

QUrl User::pictureUrl() const
{
    return d->pictureUrl;
}

bool User::isAuthenticatedUser() const
{
    return d->isAuthenticatedUser;
}

QString User::permissionId() const
{
    return d->permissionId;
}

UserPtr User::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QStringLiteral("kind")) ||
        map[QStringLiteral("kind")].toString() != QLatin1String("drive#user"))
    {
        return UserPtr();
    }

    UserPtr user(new User());
    user->d->displayName = map[QStringLiteral("displayName")].toString();
    const QVariantMap picture = map[QStringLiteral("picture")].toMap();
    user->d->pictureUrl = picture[QStringLiteral("url")].toUrl();
    user->d->isAuthenticatedUser = map[QStringLiteral("isAuthenticatedUser")].toBool();
    user->d->permissionId = map[QStringLiteral("permissionId")].toString();

    return user;
}
