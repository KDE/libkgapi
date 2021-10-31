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
 * A person's phone number.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#phonenumber
 * @since 5.19.0
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

    static PhoneNumber fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The phone number. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Output only. The type of the phone number translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

    /** Output only. The canonicalized [ITU-T E.164](https://law.resource.org/pub/us/cfr/ibr/004/itu-t.E.164.1.2008.pdf) form of the phone number. **/
    QString canonicalForm() const;

    /** The type of the phone number. The type can be custom or one of these predefined values: * `home` * `work` * `mobile` * `homeFax` * `workFax` *
     * `otherFax` * `pager` * `workMobile` * `workPager` * `main` * `googleVoice` * `other` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Metadata about the phone number. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // PhoneNumber

} // namespace KGAPI2::People