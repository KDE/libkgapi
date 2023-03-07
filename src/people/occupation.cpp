/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "occupation.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Occupation::Private : public QSharedData
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
        return value == other.value && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    FieldMetadata metadata{};
};

Occupation::Occupation()
    : d(new Private)
{
}

Occupation::Occupation(const Occupation &) = default;
Occupation::Occupation(Occupation &&) noexcept = default;
Occupation &Occupation::operator=(const Occupation &) = default;
Occupation &Occupation::operator=(Occupation &&) noexcept = default;
Occupation::~Occupation() = default;

bool Occupation::operator==(const Occupation &other) const
{
    return *d == *other.d;
}

bool Occupation::operator!=(const Occupation &other) const
{
    return !(*this == other);
}

QString Occupation::value() const
{
    return d->value;
}

void Occupation::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata Occupation::metadata() const
{
    return d->metadata;
}

void Occupation::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Occupation Occupation::fromJSON(const QJsonObject &obj)
{
    Occupation occupation;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        occupation.setMetadata(FieldMetadata::fromJSON(metadata));
        occupation.setValue(obj.value(QStringLiteral("value")).toString());
    }

    return occupation;
}

QVector<Occupation> Occupation::fromJSONArray(const QJsonArray &data)
{
    QVector<Occupation> occupations;

    for(const auto &occupation : data) {
        if(occupation.isObject()) {
            const auto objectifiedOccupation = occupation.toObject();
            occupations.append(fromJSON(objectifiedOccupation));
        }
    }

    return occupations;
}

QJsonValue Occupation::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
