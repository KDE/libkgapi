/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "personlocale.h"

#include "fieldmetadata.h"

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
    Q_UNUSED(obj);
    return PersonLocale();
}

QJsonValue PersonLocale::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"value"}, d->value);
    return obj;
}

} // namespace KGAPI2::People
