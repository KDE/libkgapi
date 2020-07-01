/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "user.h"
#include "utils_p.h"

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

bool User::operator==(const User &other) const
{
    GAPI_COMPARE(displayName)
    GAPI_COMPARE(pictureUrl)
    GAPI_COMPARE(isAuthenticatedUser)
    GAPI_COMPARE(permissionId)
    return true;
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
    if (!map.contains(QLatin1String("kind")) ||
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
