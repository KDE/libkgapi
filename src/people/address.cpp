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
#include "peopleservice.h"

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

QList<Address> Address::fromJSONArray(const QJsonArray &data)
{
    QList<People::Address> addresses;

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

    // Output only -> PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedType", d->formattedType);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "city", d->city);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "countryCode", d->countryCode);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "postalCode", d->postalCode);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "poBox", d->poBox);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "type", d->type);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "formattedValue", d->formattedValue);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "extendedAddress", d->extendedAddress);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "region", d->region);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "streetAddress", d->streetAddress);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "country", d->country);
    return obj;
}

Address Address::fromKContactsAddress(const KContacts::Address &address)
{
    Address convertedAddress;
    convertedAddress.setCity(address.locality());
    convertedAddress.setCountry(address.country());
    convertedAddress.setExtendedAddress(address.extended());
    convertedAddress.setPoBox(address.postOfficeBox());
    convertedAddress.setPostalCode(address.postalCode());
    convertedAddress.setRegion(address.region());
    convertedAddress.setStreetAddress(address.street());

    if (address.type() & KContacts::Address::Home) {
        convertedAddress.setType(QStringLiteral("home"));
    } else if (address.type() & KContacts::Address::Work) {
        convertedAddress.setType(QStringLiteral("work"));
    } else {
        convertedAddress.setType(QStringLiteral("other"));
    }

    return convertedAddress;
}

QList<Address> Address::fromKContactsAddressList(const QList<KContacts::Address> &addressList)
{
    QList<Address> convertedAddresses;
    std::transform(addressList.cbegin(), addressList.cend(), std::back_inserter(convertedAddresses), [](const KContacts::Address &address) {
        return Address::fromKContactsAddress(address);
    });
    return convertedAddresses;
}

KContacts::Address Address::toKContactsAddress() const
{
    KContacts::Address convertedAddress;
    convertedAddress.setLocality(city());
    convertedAddress.setCountry(country());
    convertedAddress.setExtended(extendedAddress());
    convertedAddress.setPostOfficeBox(poBox());
    convertedAddress.setPostalCode(postalCode());
    convertedAddress.setRegion(region());
    convertedAddress.setStreet(streetAddress());

    const auto addressType = type();
    if (addressType.compare(QStringLiteral("home"), Qt::CaseInsensitive) == 0) {
        convertedAddress.setType(KContacts::Address::Home);
    } else if (addressType.compare(QStringLiteral("work"), Qt::CaseInsensitive) == 0) {
        convertedAddress.setType(KContacts::Address::Work);
    }

    return convertedAddress;
}

} // namespace KGAPI2::People
