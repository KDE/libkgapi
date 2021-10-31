/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "relationshipinterest.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class RelationshipInterest::Private : public QSharedData
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
        return formattedValue == other.formattedValue && metadata == other.metadata && value == other.value;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString formattedValue{};
    FieldMetadata metadata{};
    QString value{};
};

RelationshipInterest::RelationshipInterest()
    : d(new Private)
{
}

RelationshipInterest::RelationshipInterest(const RelationshipInterest &) = default;
RelationshipInterest::RelationshipInterest(RelationshipInterest &&) noexcept = default;
RelationshipInterest &RelationshipInterest::operator=(const RelationshipInterest &) = default;
RelationshipInterest &RelationshipInterest::operator=(RelationshipInterest &&) noexcept = default;
RelationshipInterest::~RelationshipInterest() = default;

bool RelationshipInterest::operator==(const RelationshipInterest &other) const
{
    return *d == *other.d;
}

bool RelationshipInterest::operator!=(const RelationshipInterest &other) const
{
    return !(*this == other);
}

QString RelationshipInterest::formattedValue() const
{
    return d->formattedValue;
}
FieldMetadata RelationshipInterest::metadata() const
{
    return d->metadata;
}

void RelationshipInterest::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString RelationshipInterest::value() const
{
    return d->value;
}

void RelationshipInterest::setValue(const QString &value)
{
    d->value = value;
}

RelationshipInterest RelationshipInterest::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return RelationshipInterest();
}

QJsonValue RelationshipInterest::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"formattedValue"}, d->formattedValue);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"value"}, d->value);
    return obj;
}

} // namespace KGAPI2::People
