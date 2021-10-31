/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include <kgapipeople_export.h>

#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's physical address. May be a P.O. box or street address. All fields are
 * optional.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#address
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Address
{
public:
    /** Constructs a new Address **/
    explicit Address();
    Address(const Address &);
    Address(Address &&) noexcept;
    Address &operator=(const Address &);
    Address &operator=(Address &&) noexcept;
    /** Destructor. **/
    ~Address();

    bool operator==(const Address &) const;
    bool operator!=(const Address &) const;

    static Address fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Output only. The type of the address translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

    /** The city of the address. **/
    QString city() const;
    /** Sets value of the city property. **/
    void setCity(const QString &value);

    /** Metadata about the address. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The [ISO 3166-1 alpha-2](http://www.iso.org/iso/country_codes.htm) country code of the address. **/
    QString countryCode() const;
    /** Sets value of the countryCode property. **/
    void setCountryCode(const QString &value);

    /** The postal code of the address. **/
    QString postalCode() const;
    /** Sets value of the postalCode property. **/
    void setPostalCode(const QString &value);

    /** The P.O. box of the address. **/
    QString poBox() const;
    /** Sets value of the poBox property. **/
    void setPoBox(const QString &value);

    /** The type of the address. The type can be custom or one of these predefined values: * `home` * `work` * `other` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** The unstructured value of the address. If this is not set by the user it will be automatically constructed from structured values. **/
    QString formattedValue() const;
    /** Sets value of the formattedValue property. **/
    void setFormattedValue(const QString &value);

    /** The extended address of the address; for example, the apartment number. **/
    QString extendedAddress() const;
    /** Sets value of the extendedAddress property. **/
    void setExtendedAddress(const QString &value);

    /** The region of the address; for example, the state or province. **/
    QString region() const;
    /** Sets value of the region property. **/
    void setRegion(const QString &value);

    /** The street address. **/
    QString streetAddress() const;
    /** Sets value of the streetAddress property. **/
    void setStreetAddress(const QString &value);

    /** The country of the address. **/
    QString country() const;
    /** Sets value of the country property. **/
    void setCountry(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Address

} // namespace KGAPI2::People