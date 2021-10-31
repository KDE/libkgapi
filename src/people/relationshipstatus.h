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
 * **DEPRECATED**: No data will be returned A person's relationship status.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#relationshipstatus
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT RelationshipStatus
{
public:
    /** Constructs a new RelationshipStatus **/
    explicit RelationshipStatus();
    RelationshipStatus(const RelationshipStatus &);
    RelationshipStatus(RelationshipStatus &&) noexcept;
    RelationshipStatus &operator=(const RelationshipStatus &);
    RelationshipStatus &operator=(RelationshipStatus &&) noexcept;
    /** Destructor. **/
    ~RelationshipStatus();

    bool operator==(const RelationshipStatus &) const;
    bool operator!=(const RelationshipStatus &) const;

    static RelationshipStatus fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** The relationship status. The value can be custom or one of these predefined values: * `single` * `inARelationship` * `engaged` * `married` *
     * `itsComplicated` * `openRelationship` * `widowed` * `inDomesticPartnership` * `inCivilUnion` **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

    /** Output only. The value of the relationship status translated and formatted in the viewer's account locale or the `Accept-Language` HTTP header locale.
     * **/
    QString formattedValue() const;

    /** Metadata about the relationship status. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // RelationshipStatus

} // namespace KGAPI2::People