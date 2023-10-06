/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#pragma once

#include "kgapidrive_export.h"
#include "types.h"

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
    bool operator==(const User &other) const;
    bool operator!=(const User &other) const
    {
        return !operator==(other);
    }

    /**
     * @brief Returns a plain text displayable name for this user.
     */
    [[nodiscard]] QString displayName() const;

    /**
     * @brief Returns a URL that points to a profile picture of this user.
     */
    [[nodiscard]] QUrl pictureUrl() const;

    /**
     * @brief Returns whether this user is the same as the authenticated
     *        user for whom the request was made.
     */
    [[nodiscard]] bool isAuthenticatedUser() const;

    /**
     * @brief Returns the user's ID as visible in the permissions collection.
     */
    [[nodiscard]] QString permissionId() const;

    static UserPtr fromJSON(const QVariantMap &jsonMap);

private:
    explicit User();

    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
