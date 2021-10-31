/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "url.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Url::Private : public QSharedData
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
        return value == other.value && type == other.type && metadata == other.metadata && formattedType == other.formattedType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    QString type{};
    FieldMetadata metadata{};
    QString formattedType{};
};

Url::Url()
    : d(new Private)
{
}

Url::Url(const Url &) = default;
Url::Url(Url &&) noexcept = default;
Url &Url::operator=(const Url &) = default;
Url &Url::operator=(Url &&) noexcept = default;
Url::~Url() = default;

bool Url::operator==(const Url &other) const
{
    return *d == *other.d;
}

bool Url::operator!=(const Url &other) const
{
    return !(*this == other);
}

QString Url::value() const
{
    return d->value;
}

void Url::setValue(const QString &value)
{
    d->value = value;
}
QString Url::type() const
{
    return d->type;
}

void Url::setType(const QString &value)
{
    d->type = value;
}
FieldMetadata Url::metadata() const
{
    return d->metadata;
}

void Url::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Url::formattedType() const
{
    return d->formattedType;
}

Url Url::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return Url();
}

QJsonValue Url::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    return obj;
}

} // namespace KGAPI2::People
