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

namespace KContacts
{
class Email;
}

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's email address.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#emailaddress
 * @since 5.23.0
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

    Q_REQUIRED_RESULT static EmailAddress fromJSON(const QJsonObject &obj);
    Q_REQUIRED_RESULT static QList<EmailAddress> fromJSONArray(const QJsonArray &data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    Q_REQUIRED_RESULT static EmailAddress fromKContactsEmail(const KContacts::Email &email);
    Q_REQUIRED_RESULT static QList<EmailAddress> fromKContactsEmailList(const QList<KContacts::Email> &emailList);
    Q_REQUIRED_RESULT KContacts::Email toKContactsEmail() const;

    /** The email address. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the email address. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The type of the email address. The type can be custom or one of these predefined values: * `home` * `work` * `other` **/
    Q_REQUIRED_RESULT QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** The display name of the email. **/
    Q_REQUIRED_RESULT QString displayName() const;
    /** Sets value of the displayName property. **/
    void setDisplayName(const QString &value);

    /** Output only. The type of the email address translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    Q_REQUIRED_RESULT QString formattedType() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // EmailAddress

} // namespace KGAPI2::People
