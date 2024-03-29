/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "gender.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Gender::Private : public QSharedData
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
        return metadata == other.metadata && formattedValue == other.formattedValue && value == other.value && addressMeAs == other.addressMeAs;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString formattedValue{};
    QString value{};
    QString addressMeAs{};
};

Gender::Gender()
    : d(new Private)
{
}

Gender::Gender(const Gender &) = default;
Gender::Gender(Gender &&) noexcept = default;
Gender &Gender::operator=(const Gender &) = default;
Gender &Gender::operator=(Gender &&) noexcept = default;
Gender::~Gender() = default;

bool Gender::operator==(const Gender &other) const
{
    return *d == *other.d;
}

bool Gender::operator!=(const Gender &other) const
{
    return !(*this == other);
}

FieldMetadata Gender::metadata() const
{
    return d->metadata;
}

void Gender::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Gender::formattedValue() const
{
    return d->formattedValue;
}
QString Gender::value() const
{
    return d->value;
}

void Gender::setValue(const QString &value)
{
    d->value = value;
}
QString Gender::addressMeAs() const
{
    return d->addressMeAs;
}

void Gender::setAddressMeAs(const QString &value)
{
    d->addressMeAs = value;
}

Gender Gender::fromJSON(const QJsonObject &obj)
{
    Gender gender;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        gender.d->metadata = FieldMetadata::fromJSON(metadata);
        gender.d->value = obj.value(QStringLiteral("value")).toString();
        gender.d->formattedValue = obj.value(QStringLiteral("formattedValue")).toString();
        gender.d->addressMeAs = obj.value(QStringLiteral("addressMeAs")).toString();
    }

    return gender;
}

QList<Gender> Gender::fromJSONArray(const QJsonArray &data)
{
    QList<Gender> genders;

    for(const auto &gender : data) {
        if(gender.isObject()) {
            const auto objectifiedGender = gender.toObject();
            genders.append(fromJSON(objectifiedGender));
        }
    }

    return genders;
}

QJsonValue Gender::toJSON() const
{
    QJsonObject obj;

    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedValue", d->formattedValue);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "addressMeAs", d->addressMeAs);
    return obj;
}

} // namespace KGAPI2::People
