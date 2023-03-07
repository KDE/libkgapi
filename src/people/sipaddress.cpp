/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "sipaddress.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

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
    SipAddress sipAddress;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        sipAddress.d->metadata = FieldMetadata::fromJSON(metadata);
        sipAddress.d->value = obj.value(QStringLiteral("value")).toString();
        sipAddress.d->type = obj.value(QStringLiteral("type")).toString();
        sipAddress.d->formattedType = obj.value(QStringLiteral("formattedType")).toString();
    }
    return sipAddress;
}

QVector<SipAddress> SipAddress::fromJSONArray(const QJsonArray &data)
{
    QVector<SipAddress> sipAddresses;

    for(const auto &sipAddress : data) {
        if(sipAddress.isObject()) {
            const auto objectifiedSipAddress = sipAddress.toObject();
            sipAddresses.append(fromJSON(objectifiedSipAddress));
        }
    }

    return sipAddresses;
}

QJsonValue SipAddress::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "type", d->type);
    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedType", d->formattedType);
    return obj;
}

} // namespace KGAPI2::People
