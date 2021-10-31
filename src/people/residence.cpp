/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "residence.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Residence::Private : public QSharedData
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
        return value == other.value && metadata == other.metadata && current == other.current;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    FieldMetadata metadata{};
    bool current{};
};

Residence::Residence()
    : d(new Private)
{
}

Residence::Residence(const Residence &) = default;
Residence::Residence(Residence &&) noexcept = default;
Residence &Residence::operator=(const Residence &) = default;
Residence &Residence::operator=(Residence &&) noexcept = default;
Residence::~Residence() = default;

bool Residence::operator==(const Residence &other) const
{
    return *d == *other.d;
}

bool Residence::operator!=(const Residence &other) const
{
    return !(*this == other);
}

QString Residence::value() const
{
    return d->value;
}

void Residence::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata Residence::metadata() const
{
    return d->metadata;
}

void Residence::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
bool Residence::current() const
{
    return d->current;
}

void Residence::setCurrent(const bool &value)
{
    d->current = value;
}

Residence Residence::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Residence();
}

QJsonValue Residence::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"current"}, d->current);
    return obj;
}

} // namespace KGAPI2::People
