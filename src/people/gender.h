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
 * A person's gender.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#gender
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT Gender
{
public:
    /** Constructs a new Gender **/
    explicit Gender();
    Gender(const Gender &);
    Gender(Gender &&) noexcept;
    Gender &operator=(const Gender &);
    Gender &operator=(Gender &&) noexcept;
    /** Destructor. **/
    ~Gender();

    bool operator==(const Gender &) const;
    bool operator!=(const Gender &) const;

    static Gender fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Metadata about the gender. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** Output only. The value of the gender translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. Unspecified or
     * custom value are not localized. **/
    QString formattedValue() const;

    /** The gender for the person. The gender can be custom or one of these predefined values: * `male` * `female` * `unspecified` **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Free form text field for pronouns that should be used to address the person. Common values are: * `he`/`him` * `she`/`her` * `they`/`them` **/
    QString addressMeAs() const;
    /** Sets value of the addressMeAs property. **/
    void setAddressMeAs(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Gender

} // namespace KGAPI2::People