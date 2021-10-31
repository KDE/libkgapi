/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "address.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
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
    Q_UNUSED(obj);
    return Address();
}

QJsonValue Address::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    obj.insert(QStringView{u"city"}, d->city);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
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
