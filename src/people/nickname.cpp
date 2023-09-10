/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "nickname.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Nickname::Private : public QSharedData
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
        return value == other.value && type == other.type && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    Nickname::Type type{};
    FieldMetadata metadata{};
};

Nickname::Nickname()
    : d(new Private)
{
}

Nickname::Nickname(const Nickname &) = default;
Nickname::Nickname(Nickname &&) noexcept = default;
Nickname &Nickname::operator=(const Nickname &) = default;
Nickname &Nickname::operator=(Nickname &&) noexcept = default;
Nickname::~Nickname() = default;

bool Nickname::operator==(const Nickname &other) const
{
    return *d == *other.d;
}

bool Nickname::operator!=(const Nickname &other) const
{
    return !(*this == other);
}

QString Nickname::value() const
{
    return d->value;
}

void Nickname::setValue(const QString &value)
{
    d->value = value;
}
Nickname::Nickname::Type Nickname::type() const
{
    return d->type;
}

void Nickname::setType(Nickname::Type value)
{
    d->type = value;
}
FieldMetadata Nickname::metadata() const
{
    return d->metadata;
}

void Nickname::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Nickname Nickname::fromJSON(const QJsonObject &obj)
{
    Nickname nickname;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        nickname.setMetadata(FieldMetadata::fromJSON(metadata));
        nickname.setValue(obj.value(QStringLiteral("value")).toString());

        const auto type = obj.value(QStringLiteral("type")).toString();
        if(type == QLatin1String("MAIDEN_NAME")) {
            nickname.setType(Nickname::Type::MAIDEN_NAME);
        } else if (type == QLatin1String("INITIALS")) {
            nickname.setType(Nickname::Type::INITIALS);
        } else if (type == QLatin1String("GPLUS")) {
            nickname.setType(Nickname::Type::GPLUS);
        } else if (type == QLatin1String("OTHER_NAME")) {
            nickname.setType(Nickname::Type::OTHER_NAME);
        } else if (type == QLatin1String("ALTERNATE_NAME")) {
            nickname.setType(Nickname::Type::ALTERNATE_NAME);
        } else if (type == QLatin1String("SHORT_NAME")) {
            nickname.setType(Nickname::Type::SHORT_NAME);
        } else {
            nickname.setType(Nickname::Type::DEFAULT);
        }
    }

    return nickname;
}

QList<Nickname> Nickname::fromJSONArray(const QJsonArray &data)
{
    QList<Nickname> nicknames;

    for(const auto &nickname : data) {
        if(nickname.isObject()) {
            const auto objectifiedNickname = nickname.toObject();
            nicknames.append(fromJSON(objectifiedNickname));
        }
    }

    return nicknames;
}

QJsonValue Nickname::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    switch (d->type) {
    case Type::DEFAULT:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("DEFAULT"));
        break;
    case Type::MAIDEN_NAME:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("MAIDEN_NAME"));
        break;
    case Type::INITIALS:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("INITIALS"));
        break;
    case Type::GPLUS:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("GPLUS"));
        break;
    case Type::OTHER_NAME:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("OTHER_NAME"));
        break;
    case Type::ALTERNATE_NAME:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("ALTERNATE_NAME"));
        break;
    case Type::SHORT_NAME:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "type", QStringLiteral("SHORT_NAME"));
        break;
    }
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
