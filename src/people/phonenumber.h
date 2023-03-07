/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QSharedDataPointer>
#include "kgapipeople_export.h"

#include <QString>

#include <optional>

class QJsonObject;
class QJsonValue;
class QJsonArray;

namespace KContacts {
class PhoneNumber;
}

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's phone number.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#phonenumber
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT PhoneNumber
{
public:
    /** Constructs a new PhoneNumber **/
    explicit PhoneNumber();
    PhoneNumber(const PhoneNumber &);
    PhoneNumber(PhoneNumber &&) noexcept;
    PhoneNumber &operator=(const PhoneNumber &);
    PhoneNumber &operator=(PhoneNumber &&) noexcept;
    /** Destructor. **/
    ~PhoneNumber();

    bool operator==(const PhoneNumber &) const;
    bool operator!=(const PhoneNumber &) const;

    Q_REQUIRED_RESULT static PhoneNumber fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QVector<PhoneNumber> fromJSONArray(const QJsonArray &data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    Q_REQUIRED_RESULT static PhoneNumber fromKContactsPhoneNumber(const KContacts::PhoneNumber &phoneNumber);
    Q_REQUIRED_RESULT static QVector<PhoneNumber> fromKContactsPhoneNumberList(const QVector<KContacts::PhoneNumber> &phoneNumberList);
    Q_REQUIRED_RESULT KContacts::PhoneNumber toKContactsPhoneNumber() const;

    /** The phone number. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Output only. The type of the phone number translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    Q_REQUIRED_RESULT QString formattedType() const;

    /** Output only. The canonicalized [ITU-T E.164](https://law.resource.org/pub/us/cfr/ibr/004/itu-t.E.164.1.2008.pdf) form of the phone number. **/
    Q_REQUIRED_RESULT QString canonicalForm() const;

    /** The type of the phone number. The type can be custom or one of these predefined values: * `home` * `work` * `mobile` * `homeFax` * `workFax` *
     * `otherFax` * `pager` * `workMobile` * `workPager` * `main` * `googleVoice` * `other` **/
    Q_REQUIRED_RESULT QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Metadata about the phone number. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // PhoneNumber

} // namespace KGAPI2::People
