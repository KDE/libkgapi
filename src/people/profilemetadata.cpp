/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
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
    Q_UNUSED(obj);
    return ProfileMetadata();
}

QJsonValue ProfileMetadata::toJSON() const
{
    QJsonObject obj;

    {
        QJsonArray arr;
        std::transform(d->userTypes.cbegin(), d->userTypes.cend(), std::back_inserter(arr), [](const auto &val) {
            switch (val) {
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
        obj.insert(QStringView{u"userTypes"}, std::move(arr));
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
