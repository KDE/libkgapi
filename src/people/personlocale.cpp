/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "personlocale.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class PersonLocale::Private : public QSharedData
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
        return metadata == other.metadata && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QString value{};
};

PersonLocale::PersonLocale()
    : d(new Private)
{
}

PersonLocale::PersonLocale(const PersonLocale &) = default;
PersonLocale::PersonLocale(PersonLocale &&) noexcept = default;
PersonLocale &PersonLocale::operator=(const PersonLocale &) = default;
PersonLocale &PersonLocale::operator=(PersonLocale &&) noexcept = default;
PersonLocale::~PersonLocale() = default;

bool PersonLocale::operator==(const PersonLocale &other) const
{
    return *d == *other.d;
}

bool PersonLocale::operator!=(const PersonLocale &other) const
{
    return !(*this == other);
}

FieldMetadata PersonLocale::metadata() const
{
    return d->metadata;
}

void PersonLocale::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString PersonLocale::value() const
{
    return d->value;
}

void PersonLocale::setValue(const QString &value)
{
    d->value = value;
}

PersonLocale PersonLocale::fromJSON(const QJsonObject &obj)
{
    PersonLocale locale;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        locale.setMetadata(FieldMetadata::fromJSON(metadata));
        locale.setValue(obj.value(QStringLiteral("value")).toString());
    }

    return locale;
}

QVector<PersonLocale> PersonLocale::fromJSONArray(const QJsonArray& data)
{
    QVector<PersonLocale> locales;

    for (const auto &locale : data) {
        if(locale.isObject()) {
            const auto objectifiedLocale = locale.toObject();
            locales.append(fromJSON(objectifiedLocale));
        }
    }

    return locales;
}

QJsonValue PersonLocale::toJSON() const
{
    QJsonObject obj;

    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    return obj;
}

} // namespace KGAPI2::People
