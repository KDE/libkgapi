/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "tagline.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Tagline::Private : public QSharedData
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

Tagline::Tagline()
    : d(new Private)
{
}

Tagline::Tagline(const Tagline &) = default;
Tagline::Tagline(Tagline &&) noexcept = default;
Tagline &Tagline::operator=(const Tagline &) = default;
Tagline &Tagline::operator=(Tagline &&) noexcept = default;
Tagline::~Tagline() = default;

bool Tagline::operator==(const Tagline &other) const
{
    return *d == *other.d;
}

bool Tagline::operator!=(const Tagline &other) const
{
    return !(*this == other);
}

QString Tagline::value() const
{
    return d->value;
}

void Tagline::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata Tagline::metadata() const
{
    return d->metadata;
}

void Tagline::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

Tagline Tagline::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Tagline();
}

QJsonValue Tagline::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
