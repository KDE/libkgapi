/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "braggingrights.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class BraggingRights::Private : public QSharedData
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

BraggingRights::BraggingRights()
    : d(new Private)
{
}

BraggingRights::BraggingRights(const BraggingRights &) = default;
BraggingRights::BraggingRights(BraggingRights &&) noexcept = default;
BraggingRights &BraggingRights::operator=(const BraggingRights &) = default;
BraggingRights &BraggingRights::operator=(BraggingRights &&) noexcept = default;
BraggingRights::~BraggingRights() = default;

bool BraggingRights::operator==(const BraggingRights &other) const
{
    return *d == *other.d;
}

bool BraggingRights::operator!=(const BraggingRights &other) const
{
    return !(*this == other);
}

FieldMetadata BraggingRights::metadata() const
{
    return d->metadata;
}

void BraggingRights::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString BraggingRights::value() const
{
    return d->value;
}

void BraggingRights::setValue(const QString &value)
{
    d->value = value;
}

BraggingRights BraggingRights::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return BraggingRights();
}

QJsonValue BraggingRights::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"value"}, d->value);
    return obj;
}

} // namespace KGAPI2::People
