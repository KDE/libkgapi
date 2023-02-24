/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "profilemetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class ProfileMetadata::Private : public QSharedData
{
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    bool operator==(const Private &other) const
    {
        return userTypes == other.userTypes && objectType == other.objectType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QVector<ProfileMetadata::UserTypes> userTypes{};
    ProfileMetadata::ObjectType objectType{};
};

ProfileMetadata::ProfileMetadata()
    : d(new Private)
{
}

ProfileMetadata::ProfileMetadata(const ProfileMetadata &) = default;
ProfileMetadata::ProfileMetadata(ProfileMetadata &&) noexcept = default;
ProfileMetadata &ProfileMetadata::operator=(const ProfileMetadata &) = default;
ProfileMetadata &ProfileMetadata::operator=(ProfileMetadata &&) noexcept = default;
ProfileMetadata::~ProfileMetadata() = default;

bool ProfileMetadata::operator==(const ProfileMetadata &other) const
{
    return *d == *other.d;
}

bool ProfileMetadata::operator!=(const ProfileMetadata &other) const
{
    return !(*this == other);
}

QVector<ProfileMetadata::UserTypes> ProfileMetadata::userTypes() const
{
    return d->userTypes;
}
ProfileMetadata::ProfileMetadata::ObjectType ProfileMetadata::objectType() const
{
    return d->objectType;
}

ProfileMetadata ProfileMetadata::fromJSON(const QJsonObject &obj)
{
    ProfileMetadata profileMetadata;

    if(!obj.isEmpty()) {
        const auto objectTypeEnumString = obj.value(QStringLiteral("objectType"));
        if(objectTypeEnumString == QStringLiteral("PERSON")) {
            profileMetadata.d->objectType = ObjectType::PERSON;
        } else if (objectTypeEnumString == QStringLiteral("PAGE")) {
            profileMetadata.d->objectType = ObjectType::PAGE;
        } else {
            profileMetadata.d->objectType = ObjectType::OBJECT_TYPE_UNSPECIFIED;
        }

        if(obj.value(QStringLiteral("userTypes")).isArray()) {
            const auto userTypesJsonArray = obj.value(QStringLiteral("userTypes")).toArray();

            for(const auto userType : userTypesJsonArray) {
                if(userType == obj.value(QStringLiteral("GOOGLE_USER"))) {
                    profileMetadata.d->userTypes.append(UserTypes::GOOGLE_USER);
                } else if(userType == obj.value(QStringLiteral("GPLUS_USER"))) {
                    profileMetadata.d->userTypes.append(UserTypes::GPLUS_USER);
                } else if(userType == obj.value(QStringLiteral("GOOGLE_APPS_USER"))) {
                    profileMetadata.d->userTypes.append(UserTypes::GOOGLE_APPS_USER);
                } else {
                    profileMetadata.d->userTypes.append(UserTypes::USER_TYPE_UNKNOWN);
                }
            }
        } else {
            profileMetadata.d->userTypes.append(UserTypes::USER_TYPE_UNKNOWN);
        }
    }

    return profileMetadata;
}

QJsonValue ProfileMetadata::toJSON() const
{
    QJsonObject obj;

    {
        QJsonArray arr;
        std::transform(d->userTypes.cbegin(), d->userTypes.cend(), std::back_inserter(arr), [](const auto &val) {
            switch (val) {
            default:
            case UserTypes::USER_TYPE_UNKNOWN:
                return QStringLiteral("USER_TYPE_UNKNOWN");
            case UserTypes::GOOGLE_USER:
                return QStringLiteral("GOOGLE_USER");
            case UserTypes::GPLUS_USER:
                return QStringLiteral("GPLUS_USER");
            case UserTypes::GOOGLE_APPS_USER:
                return QStringLiteral("GOOGLE_APPS_USER");
            }
        });
        if (!arr.isEmpty()) {
            obj.insert(QStringView{u"userTypes"}, std::move(arr));
        }
    }
    switch (d->objectType) {
    case ObjectType::OBJECT_TYPE_UNSPECIFIED:
        obj.insert(QStringView{u"objectType"}, QStringLiteral(u"OBJECT_TYPE_UNSPECIFIED"));
        break;
    case ObjectType::PERSON:
        obj.insert(QStringView{u"objectType"}, QStringLiteral(u"PERSON"));
        break;
    case ObjectType::PAGE:
        obj.insert(QStringView{u"objectType"}, QStringLiteral(u"PAGE"));
        break;
    }
    return obj;
}

} // namespace KGAPI2::People
