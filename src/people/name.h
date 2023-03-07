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
class Addressee;
}

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's name. If the name is a mononym, the family name is empty.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#name
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT Name
{
public:
    /** Constructs a new Name **/
    explicit Name();
    Name(const Name &);
    Name(Name &&) noexcept;
    Name &operator=(const Name &);
    Name &operator=(Name &&) noexcept;
    /** Destructor. **/
    ~Name();

    bool operator==(const Name &) const;
    bool operator!=(const Name &) const;

    Q_REQUIRED_RESULT static Name fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT static QVector<Name> fromJSONArray(const QJsonArray &data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    Q_REQUIRED_RESULT static Name fromKContactsAddressee(const KContacts::Addressee &addressee);
    void applyToKContactsAddressee(KContacts::Addressee &addressee) const;

    /** The free form name value. **/
    Q_REQUIRED_RESULT QString unstructuredName() const;
    /** Sets value of the unstructuredName property. **/
    void setUnstructuredName(const QString &value);

    /** The family name. **/
    Q_REQUIRED_RESULT QString familyName() const;
    /** Sets value of the familyName property. **/
    void setFamilyName(const QString &value);

    /** The middle name(s) spelled as they sound. **/
    Q_REQUIRED_RESULT QString phoneticMiddleName() const;
    /** Sets value of the phoneticMiddleName property. **/
    void setPhoneticMiddleName(const QString &value);

    /** The middle name(s). **/
    Q_REQUIRED_RESULT QString middleName() const;
    /** Sets value of the middleName property. **/
    void setMiddleName(const QString &value);

    /** The honorific prefixes, such as `Mrs.` or `Dr.` **/
    Q_REQUIRED_RESULT QString honorificPrefix() const;
    /** Sets value of the honorificPrefix property. **/
    void setHonorificPrefix(const QString &value);

    /** The given name. **/
    Q_REQUIRED_RESULT QString givenName() const;
    /** Sets value of the givenName property. **/
    void setGivenName(const QString &value);

    /** The given name spelled as it sounds. **/
    Q_REQUIRED_RESULT QString phoneticGivenName() const;
    /** Sets value of the phoneticGivenName property. **/
    void setPhoneticGivenName(const QString &value);

    /** The honorific prefixes spelled as they sound. **/
    Q_REQUIRED_RESULT QString phoneticHonorificPrefix() const;
    /** Sets value of the phoneticHonorificPrefix property. **/
    void setPhoneticHonorificPrefix(const QString &value);

    /** Output only. The display name formatted according to the locale specified by the viewer's account or the `Accept-Language` HTTP header. **/
    Q_REQUIRED_RESULT QString displayName() const;

    /** Output only. The display name with the last name first formatted according to the locale specified by the viewer's account or the `Accept-Language` HTTP
     * header. **/
    Q_REQUIRED_RESULT QString displayNameLastFirst() const;

    /** The family name spelled as it sounds. **/
    Q_REQUIRED_RESULT QString phoneticFamilyName() const;
    /** Sets value of the phoneticFamilyName property. **/
    void setPhoneticFamilyName(const QString &value);

    /** The honorific suffixes, such as `Jr.` **/
    Q_REQUIRED_RESULT QString honorificSuffix() const;
    /** Sets value of the honorificSuffix property. **/
    void setHonorificSuffix(const QString &value);

    /** The honorific suffixes spelled as they sound. **/
    Q_REQUIRED_RESULT QString phoneticHonorificSuffix() const;
    /** Sets value of the phoneticHonorificSuffix property. **/
    void setPhoneticHonorificSuffix(const QString &value);

    /** The full name spelled as it sounds. **/
    Q_REQUIRED_RESULT QString phoneticFullName() const;
    /** Sets value of the phoneticFullName property. **/
    void setPhoneticFullName(const QString &value);

    /** Metadata about the name. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // Name

} // namespace KGAPI2::People
