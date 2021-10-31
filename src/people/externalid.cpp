/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "externalid.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class ExternalId::Private : public QSharedData
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
        return type == other.type && formattedType == other.formattedType && value == other.value && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString type{};
    QString formattedType{};
    QString value{};
    FieldMetadata metadata{};
};

ExternalId::ExternalId()
    : d(new Private)
{
}

ExternalId::ExternalId(const ExternalId &) = default;
ExternalId::ExternalId(ExternalId &&) noexcept = default;
ExternalId &ExternalId::operator=(const ExternalId &) = default;
ExternalId &ExternalId::operator=(ExternalId &&) noexcept = default;
ExternalId::~ExternalId() = default;

bool ExternalId::operator==(const ExternalId &other) const
{
    return *d == *other.d;
}

bool ExternalId::operator!=(const ExternalId &other) const
{
    return !(*this == other);
}

QString ExternalId::type() const
{
    return d->type;
}

void ExternalId::setType(const QString &value)
{
    d->type = value;
}
QString ExternalId::formattedType() const
{
    return d->formattedType;
}
QString ExternalId::value() const
{
    return d->value;
}

void ExternalId::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata ExternalId::metadata() const
{
    return d->metadata;
}

void ExternalId::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

ExternalId ExternalId::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return ExternalId();
}

QJsonValue ExternalId::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
