/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "interest.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Interest::Private : public QSharedData
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

Interest::Interest()
    : d(new Private)
{
}

Interest::Interest(const Interest &) = default;
Interest::Interest(Interest &&) noexcept = default;
Interest &Interest::operator=(const Interest &) = default;
Interest &Interest::operator=(Interest &&) noexcept = default;
Interest::~Interest() = default;

bool Interest::operator==(const Interest &other) const
{
    return *d == *other.d;
}

bool Interest::operator!=(const Interest &other) const
{
    return !(*this == other);
}

QString Interest::value() const
{
    return d->value;
}

void Interest::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata Interest::metadata() const
{
    return d->metadata;
}

void Interest::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Interest Interest::fromJSON(const QJsonObject &obj)
{
    Interest interest;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        interest.setMetadata(FieldMetadata::fromJSON(metadata));
        interest.setValue(obj.value(QStringLiteral("value")).toString());
    }

    return interest;
}

QList<Interest> Interest::fromJSONArray(const QJsonArray &data)
{
    QList<Interest> interests;

    for(const auto &interest : data) {
        if(interest.isObject()) {
            const auto objectifiedInterest = interest.toObject();
            interests.append(fromJSON(objectifiedInterest));
        }
    }

    return interests;
}

QJsonValue Interest::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
