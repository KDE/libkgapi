/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "fieldmetadata.h"

#include "source.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class FieldMetadata::Private : public QSharedData
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
        return source == other.source && sourcePrimary == other.sourcePrimary && primary == other.primary && verified == other.verified;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    Source source{};
    bool sourcePrimary{};
    bool primary{};
    bool verified{};
};

FieldMetadata::FieldMetadata()
    : d(new Private)
{
}

FieldMetadata::FieldMetadata(const FieldMetadata &) = default;
FieldMetadata::FieldMetadata(FieldMetadata &&) noexcept = default;
FieldMetadata &FieldMetadata::operator=(const FieldMetadata &) = default;
FieldMetadata &FieldMetadata::operator=(FieldMetadata &&) noexcept = default;
FieldMetadata::~FieldMetadata() = default;

bool FieldMetadata::operator==(const FieldMetadata &other) const
{
    return *d == *other.d;
}

bool FieldMetadata::operator!=(const FieldMetadata &other) const
{
    return !(*this == other);
}

Source FieldMetadata::source() const
{
    return d->source;
}

void FieldMetadata::setSource(const Source &value)
{
    d->source = value;
}
bool FieldMetadata::sourcePrimary() const
{
    return d->sourcePrimary;
}

void FieldMetadata::setSourcePrimary(const bool &value)
{
    d->sourcePrimary = value;
}
bool FieldMetadata::primary() const
{
    return d->primary;
}
bool FieldMetadata::verified() const
{
    return d->verified;
}

FieldMetadata FieldMetadata::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return FieldMetadata();
}

QJsonValue FieldMetadata::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"source"}, d->source.toJSON());
    obj.insert(QStringView{u"sourcePrimary"}, d->sourcePrimary);
    obj.insert(QStringView{u"primary"}, d->primary);
    obj.insert(QStringView{u"verified"}, d->verified);
    return obj;
}

} // namespace KGAPI2::People
