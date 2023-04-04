/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include "kgapipeople_export.h"

#include <QList>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
/**
 * The metadata about a profile.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#profilemetadata
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT ProfileMetadata
{
public:
    enum class UserTypes {
        USER_TYPE_UNKNOWN, ///< The user type is not known.
        GOOGLE_USER, ///< The user is a Google user.
        GPLUS_USER, ///< The user is a Currents user.
        GOOGLE_APPS_USER, ///< The user is a Google Workspace user.
    };
    enum class ObjectType {
        OBJECT_TYPE_UNSPECIFIED, ///< Unspecified.
        PERSON, ///< Person.
        PAGE, ///< [Currents Page.](https://workspace.google.com/products/currents/)
    };

    /** Constructs a new ProfileMetadata **/
    explicit ProfileMetadata();
    ProfileMetadata(const ProfileMetadata &);
    ProfileMetadata(ProfileMetadata &&) noexcept;
    ProfileMetadata &operator=(const ProfileMetadata &);
    ProfileMetadata &operator=(ProfileMetadata &&) noexcept;
    /** Destructor. **/
    ~ProfileMetadata();

    bool operator==(const ProfileMetadata &) const;
    bool operator!=(const ProfileMetadata &) const;

    Q_REQUIRED_RESULT static ProfileMetadata fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** Output only. The user types. **/
    Q_REQUIRED_RESULT QList<ProfileMetadata::UserTypes> userTypes() const;

    /** Output only. The profile object type. **/
    Q_REQUIRED_RESULT ProfileMetadata::ObjectType objectType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ProfileMetadata

} // namespace KGAPI2::People
