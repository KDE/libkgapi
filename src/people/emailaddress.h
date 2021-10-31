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
 * A person's email address.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#emailaddress
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT EmailAddress
{
public:
    /** Constructs a new EmailAddress **/
    explicit EmailAddress();
    EmailAddress(const EmailAddress &);
    EmailAddress(EmailAddress &&) noexcept;
    EmailAddress &operator=(const EmailAddress &);
    EmailAddress &operator=(EmailAddress &&) noexcept;
    /** Destructor. **/
    ~EmailAddress();

    bool operator==(const EmailAddress &) const;
    bool operator!=(const EmailAddress &) const;

    static EmailAddress fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The email address. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the email address. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The type of the email address. The type can be custom or one of these predefined values: * `home` * `work` * `other` **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** The display name of the email. **/
    QString displayName() const;
    /** Sets value of the displayName property. **/
    void setDisplayName(const QString &value);

    /** Output only. The type of the email address translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // EmailAddress

} // namespace KGAPI2::People