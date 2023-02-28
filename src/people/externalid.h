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

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * An identifier from an external entity related to the person.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#externalid
 * @since 5.23.0
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

    Q_REQUIRED_RESULT static ExternalId fromJSON(const QJsonObject &obj);
    Q_REQUIRED_RESULT static QVector<ExternalId> fromJSONArray(const QJsonArray &data);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The type of the external ID. The type can be custom or one of these predefined values: * `account` * `customer` * `loginId` * `network` * `organization`
     * **/
    Q_REQUIRED_RESULT QString type() const;
    /** Sets value of the type property. **/
    void setType(const QString &value);

    /** Output only. The type of the event translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale. **/
    Q_REQUIRED_RESULT QString formattedType() const;

    /** The value of the external ID. **/
    Q_REQUIRED_RESULT QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Metadata about the external ID. **/
    Q_REQUIRED_RESULT FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // ExternalId

} // namespace KGAPI2::People
