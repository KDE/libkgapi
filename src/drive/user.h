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
#ifndef KGAPI2_DRIVEUSER_H
#define KGAPI2_DRIVEUSER_H

#include "types.h"
#include "kgapidrive_export.h"

#include <QString>
#include <QUrl>
#include <QVariantMap>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief The authenticated user.
 */
class KGAPIDRIVE_EXPORT User
{
  public:
    explicit User(const User &other);
    virtual ~User();

    /**
     * @brief Returns a plain text displayable name for this user.
     */
    QString displayName() const;

    /**
     * @brief Returns a URL that points to a profile picture of this user.
     */
    QUrl pictureUrl() const;

    /**
     * @brief Returns whether this user is the same as the authenticated
     *        user for whom the request was made.
     */
    bool isAuthenticatedUser() const;

    /**
     * @brief Returns the user's ID as visible in the permissions collection.
     */
    QString permissionId() const;

    static UserPtr fromJSON(const QVariantMap &jsonMap);

  private:
    explicit User();

    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEUSER_H
