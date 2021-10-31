/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "relationshipstatus.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class RelationshipStatus::Private : public QSharedData
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
        return value == other.value && formattedValue == other.formattedValue && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    QString formattedValue{};
    FieldMetadata metadata{};
};

RelationshipStatus::RelationshipStatus()
    : d(new Private)
{
}

RelationshipStatus::RelationshipStatus(const RelationshipStatus &) = default;
RelationshipStatus::RelationshipStatus(RelationshipStatus &&) noexcept = default;
RelationshipStatus &RelationshipStatus::operator=(const RelationshipStatus &) = default;
RelationshipStatus &RelationshipStatus::operator=(RelationshipStatus &&) noexcept = default;
RelationshipStatus::~RelationshipStatus() = default;

bool RelationshipStatus::operator==(const RelationshipStatus &other) const
{
    return *d == *other.d;
}

bool RelationshipStatus::operator!=(const RelationshipStatus &other) const
{
    return !(*this == other);
}

QString RelationshipStatus::value() const
{
    return d->value;
}

void RelationshipStatus::setValue(const QString &value)
{
    d->value = value;
}
QString RelationshipStatus::formattedValue() const
{
    return d->formattedValue;
}
FieldMetadata RelationshipStatus::metadata() const
{
    return d->metadata;
}

void RelationshipStatus::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

RelationshipStatus RelationshipStatus::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return RelationshipStatus();
}

QJsonValue RelationshipStatus::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"formattedValue"}, d->formattedValue);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
