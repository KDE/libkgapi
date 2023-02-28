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
#include "source.h"

#include <optional>

class QJsonObject;
class QJsonValue;

namespace KGAPI2::People
{
/**
 * Metadata about a field.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#fieldmetadata
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT FieldMetadata
{
public:
    /** Constructs a new FieldMetadata **/
    explicit FieldMetadata();
    FieldMetadata(const FieldMetadata &);
    FieldMetadata(FieldMetadata &&) noexcept;
    FieldMetadata &operator=(const FieldMetadata &);
    FieldMetadata &operator=(FieldMetadata &&) noexcept;
    /** Destructor. **/
    ~FieldMetadata();

    bool operator==(const FieldMetadata &) const;
    bool operator!=(const FieldMetadata &) const;

    Q_REQUIRED_RESULT static FieldMetadata fromJSON(const QJsonObject &);
    Q_REQUIRED_RESULT QJsonValue toJSON() const;

    /** The source of the field. **/
    Q_REQUIRED_RESULT Source source() const;
    /** Sets value of the source property. **/
    void setSource(const Source &value);

    /** True if the field is the primary field for the source. Each source must have at most one field with `source_primary` set to true. **/
    Q_REQUIRED_RESULT bool sourcePrimary() const;
    /** Sets value of the sourcePrimary property. **/
    void setSourcePrimary(const bool &value);

    /** Output only. True if the field is the primary field for all sources in the person. Each person will have at most one field with `primary` set to true.
     * **/
    Q_REQUIRED_RESULT bool primary() const;

    /** Output only. True if the field is verified; false if the field is unverified. A verified field is typically a name, email address, phone number, or
     * website that has been confirmed to be owned by the person. **/
    Q_REQUIRED_RESULT bool verified() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // FieldMetadata

} // namespace KGAPI2::People
