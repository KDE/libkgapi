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
 * An identifier from an external entity related to the person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#externalid
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT ExternalId
{
public:
    /** Constructs a new ExternalId **/
    explicit ExternalId();
    ExternalId(const ExternalId &);
    ExternalId(ExternalId &&) noexcept;
    ExternalId &operator=(const ExternalId &);
    ExternalId &operator=(ExternalId &&) noexcept;
    /** Destructor. **/
    ~ExternalId();

    bool operator==(const ExternalId &) const;
    bool operator!=(const ExternalId &) const;

    static ExternalId fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The type of the external ID. The type can be custom or one of these predefined values: * `account` * `customer` * `loginId` * `network` * `organization`
     * **/
    QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Output only. The type of the event translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    QString formattedType() const;

    /** The value of the external ID. **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the external ID. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ExternalId

} // namespace KGAPI2::People