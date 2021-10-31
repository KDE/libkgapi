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
 * **DEPRECATED**: No data will be returned A person's relationship interest .
 *
 * @see https://developers.google.com/people/api/rest/v1/people#relationshipinterest
 * @since 5.19.0
 **/
class KGAPIPEOPLE_EXPORT RelationshipInterest
{
public:
    /** Constructs a new RelationshipInterest **/
    explicit RelationshipInterest();
    RelationshipInterest(const RelationshipInterest &);
    RelationshipInterest(RelationshipInterest &&) noexcept;
    RelationshipInterest &operator=(const RelationshipInterest &);
    RelationshipInterest &operator=(RelationshipInterest &&) noexcept;
    /** Destructor. **/
    ~RelationshipInterest();

    bool operator==(const RelationshipInterest &) const;
    bool operator!=(const RelationshipInterest &) const;

    static RelationshipInterest fromJSON(const QJsonObject &);
    QJsonValue toJSON() const;

    /** Output only. The value of the relationship interest translated and formatted in the viewer's account locale or the locale specified in the
     * Accept-Language HTTP header. **/
    QString formattedValue() const;

    /** Metadata about the relationship interest. **/
    FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

    /** The kind of relationship the person is looking for. The value can be custom or one of these predefined values: * `friend` * `date` * `relationship` *
     * `networking` **/
    QString value() const;
    /** Sets value of the value property. **/
    void setValue(const QString &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // RelationshipInterest

} // namespace KGAPI2::People