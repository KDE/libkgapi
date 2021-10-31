/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "sipaddress.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class SipAddress::Private : public QSharedData
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
        return value == other.value && metadata == other.metadata && type == other.type && formattedType == other.formattedType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString value{};
    FieldMetadata metadata{};
    QString type{};
    QString formattedType{};
};

SipAddress::SipAddress()
    : d(new Private)
{
}

SipAddress::SipAddress(const SipAddress &) = default;
SipAddress::SipAddress(SipAddress &&) noexcept = default;
SipAddress &SipAddress::operator=(const SipAddress &) = default;
SipAddress &SipAddress::operator=(SipAddress &&) noexcept = default;
SipAddress::~SipAddress() = default;

bool SipAddress::operator==(const SipAddress &other) const
{
    return *d == *other.d;
}

bool SipAddress::operator!=(const SipAddress &other) const
{
    return !(*this == other);
}

QString SipAddress::value() const
{
    return d->value;
}

void SipAddress::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata SipAddress::metadata() const
{
    return d->metadata;
}

void SipAddress::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString SipAddress::type() const
{
    return d->type;
}

void SipAddress::setType(const QString &value)
{
    d->type = value;
}
QString SipAddress::formattedType() const
{
    return d->formattedType;
}

SipAddress SipAddress::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return SipAddress();
}

QJsonValue SipAddress::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    return obj;
}

} // namespace KGAPI2::People
