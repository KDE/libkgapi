/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "address.h"

#include "fieldmetadata.h"

#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Address::Private : public QSharedData
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
        return formattedType == other.formattedType && city == other.city && metadata == other.metadata && countryCode == other.countryCode
            && postalCode == other.postalCode && poBox == other.poBox && type == other.type && formattedValue == other.formattedValue
            && extendedAddress == other.extendedAddress && region == other.region && streetAddress == other.streetAddress && country == other.country;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString formattedType{};
    QString city{};
    FieldMetadata metadata{};
    QString countryCode{};
    QString postalCode{};
    QString poBox{};
    QString type{};
    QString formattedValue{};
    QString extendedAddress{};
    QString region{};
    QString streetAddress{};
    QString country{};
};

Address::Address()
    : d(new Private)
{
}

Address::Address(const Address &) = default;
Address::Address(Address &&) noexcept = default;
Address &Address::operator=(const Address &) = default;
Address &Address::operator=(Address &&) noexcept = default;
Address::~Address() = default;

bool Address::operator==(const Address &other) const
{
    return *d == *other.d;
}

bool Address::operator!=(const Address &other) const
{
    return !(*this == other);
}

QString Address::formattedType() const
{
    return d->formattedType;
}
QString Address::city() const
{
    return d->city;
}

void Address::setCity(const QString &value)
{
    d->city = value;
}
FieldMetadata Address::metadata() const
{
    return d->metadata;
}

void Address::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Address::countryCode() const
{
    return d->countryCode;
}

void Address::setCountryCode(const QString &value)
{
    d->countryCode = value;
}
QString Address::postalCode() const
{
    return d->postalCode;
}

void Address::setPostalCode(const QString &value)
{
    d->postalCode = value;
}
QString Address::poBox() const
{
    return d->poBox;
}

void Address::setPoBox(const QString &value)
{
    d->poBox = value;
}
QString Address::type() const
{
    return d->type;
}

void Address::setType(const QString &value)
{
    d->type = value;
}
QString Address::formattedValue() const
{
    return d->formattedValue;
}

void Address::setFormattedValue(const QString &value)
{
    d->formattedValue = value;
}
QString Address::extendedAddress() const
{
    return d->extendedAddress;
}

void Address::setExtendedAddress(const QString &value)
{
    d->extendedAddress = value;
}
QString Address::region() const
{
    return d->region;
}

void Address::setRegion(const QString &value)
{
    d->region = value;
}
QString Address::streetAddress() const
{
    return d->streetAddress;
}

void Address::setStreetAddress(const QString &value)
{
    d->streetAddress = value;
}
QString Address::country() const
{
    return d->country;
}

void Address::setCountry(const QString &value)
{
    d->country = value;
}

Address Address::fromJSON(const QJsonObject &obj)
{
    Address address;

    if(!obj.isEmpty()) {
        address.setMetadata(FieldMetadata::fromJSON(obj.value(QStringLiteral("metadata")).toObject()));
        address.setFormattedValue(obj.value(QStringLiteral("formattedValue")).toString());
        address.setType(obj.value(QStringLiteral("type")).toString());
        address.setPoBox(obj.value(QStringLiteral("poBox")).toString());
        address.setStreetAddress(obj.value(QStringLiteral("streetAddress")).toString());
        address.setExtendedAddress(obj.value(QStringLiteral("extendedAddress")).toString());
        address.setCity(obj.value(QStringLiteral("city")).toString());
        address.setRegion(obj.value(QStringLiteral("region")).toString());
        address.setPostalCode(obj.value(QStringLiteral("postalCode")).toString());
        address.setCountry(obj.value(QStringLiteral("country")).toString());
        address.setCountryCode(obj.value(QStringLiteral("countryCode")).toString());
    }

    return address;
}

QVector<Address> Address::fromJSONArray(const QJsonArray& data)
{
    QVector<People::Address> addresses;

    for(const auto &address : data) {
        if(address.isObject()) {
            const auto objectifiedAddress = address.toObject();
            addresses.append(fromJSON(objectifiedAddress));
        }
    }

    return addresses;
}

QJsonValue Address::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    obj.insert(QStringView{u"city"}, d->city);
    // Skip, field metadata is only useful for receiving -> obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"countryCode"}, d->countryCode);
    obj.insert(QStringView{u"postalCode"}, d->postalCode);
    obj.insert(QStringView{u"poBox"}, d->poBox);
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"formattedValue"}, d->formattedValue);
    obj.insert(QStringView{u"extendedAddress"}, d->extendedAddress);
    obj.insert(QStringView{u"region"}, d->region);
    obj.insert(QStringView{u"streetAddress"}, d->streetAddress);
    obj.insert(QStringView{u"country"}, d->country);
    return obj;
}

} // namespace KGAPI2::People