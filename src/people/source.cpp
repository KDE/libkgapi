/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "source.h"

#include "peopleservice.h"
#include "profilemetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Source::Private : public QSharedData
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
        return etag == other.etag && profileMetadata == other.profileMetadata && id == other.id && updateTime == other.updateTime && type == other.type;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString etag{};
    ProfileMetadata profileMetadata{};
    QString id{};
    QString updateTime{};
    Source::Type type{};
};

Source::Source()
    : d(new Private)
{
}

Source::Source(const Source &) = default;
Source::Source(Source &&) noexcept = default;
Source &Source::operator=(const Source &) = default;
Source &Source::operator=(Source &&) noexcept = default;
Source::~Source() = default;

bool Source::operator==(const Source &other) const
{
    return *d == *other.d;
}

bool Source::operator!=(const Source &other) const
{
    return !(*this == other);
}

QString Source::etag() const
{
    return d->etag;
}

void Source::setEtag(const QString &value)
{
    d->etag = value;
}
ProfileMetadata Source::profileMetadata() const
{
    return d->profileMetadata;
}
QString Source::id() const
{
    return d->id;
}

void Source::setId(const QString &value)
{
    d->id = value;
}
QString Source::updateTime() const
{
    return d->updateTime;
}
Source::Source::Type Source::type() const
{
    return d->type;
}

void Source::setType(Source::Type value)
{
    d->type = value;
}

Source Source::fromJSON(const QJsonObject &obj)
{
    Source source;

    if(!obj.isEmpty()) {
        const auto typeEnumString = obj.value(QStringLiteral("type"));

        if(typeEnumString == QStringLiteral("ACCOUNT")) {
            source.d->type = Type::ACCOUNT;
        } else if(typeEnumString == QStringLiteral("PROFILE")) {
            source.d->type = Type::PROFILE;
        } else if(typeEnumString == QStringLiteral("DOMAIN_PROFILE")) {
            source.d->type = Type::DOMAIN_PROFILE;
        } else if(typeEnumString == QStringLiteral("CONTACT")) {
            source.d->type = Type::CONTACT;
        } else if(typeEnumString == QStringLiteral("OTHER_CONTACT")) {
            source.d->type = Type::OTHER_CONTACT;
        } else if(typeEnumString == QStringLiteral("DOMAIN_CONTACT")) {
            source.d->type = Type::DOMAIN_CONTACT;
        } else {
            source.d->type = Type::SOURCE_TYPE_UNSPECIFIED;
        }

        source.d->id = obj.value(QStringLiteral("id")).toString();
        source.d->etag = obj.value(QStringLiteral("etag")).toString();
        source.d->updateTime = obj.value(QStringLiteral("id")).toString();
        source.d->profileMetadata = ProfileMetadata::fromJSON(obj.value(QStringLiteral("profileMetadata")).toObject());
    }

    return source;
}

QJsonValue Source::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "etag", d->etag);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "profileMetadata", d->profileMetadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "id", d->id);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "updateTime", d->updateTime);
    switch (d->type) {
    case Type::SOURCE_TYPE_UNSPECIFIED:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("SOURCE_TYPE_UNSPECIFIED"));
        break;
    case Type::ACCOUNT:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("ACCOUNT"));
        break;
    case Type::PROFILE:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("PROFILE"));
        break;
    case Type::DOMAIN_PROFILE:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("DOMAIN_PROFILE"));
        break;
    case Type::CONTACT:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("CONTACT"));
        break;
    case Type::OTHER_CONTACT:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OTHER_CONTACT"));
        break;
    case Type::DOMAIN_CONTACT:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("DOMAIN_CONTACT"));
        break;
    }
    return obj;
}

} // namespace KGAPI2::People
