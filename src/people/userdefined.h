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
 * Arbitrary user data that is populated by the end users.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#userdefined
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT UserDefined
{
public:
    /** Constructs a new UserDefined **/
    explicit UserDefined();
    UserDefined(const UserDefined &);
    UserDefined(UserDefined &&) noexcept;
    UserDefined &operator=(const UserDefined &);
    UserDefined &operator=(UserDefined &&) noexcept;
    /** Destructor. **/
    ~UserDefined();

    bool operator==(const UserDefined &) const;
    bool operator!=(const UserDefined &) const;

    static UserDefined fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The end user specified value of the user defined data. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** The end user specified key of the user defined data. **/
    QString key() const;
    /** Sets value of the key property. **/
    void setKey(const QString &value);

    /** Metadata about the user defined data. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // UserDefined

} // namespace KGAPI2::People