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
#include <QJsonArray>
#include <QJsonObject>

#include <optional>

class QJsonValue;

namespace KGAPI2::People
{
class FieldMetadata;

/**
 * A person's age range.
 *
 * @see https://developers.google.com/people/api/rest/v1/people#agerangetype
 * @since 5.23.0
 **/
class KGAPIPEOPLE_EXPORT AgeRangeType
{
public:
    enum class AgeRange {
        AGE_RANGE_UNSPECIFIED, ///< Unspecified.
        LESS_THAN_EIGHTEEN, ///< Younger than eighteen.
        EIGHTEEN_TO_TWENTY, ///< Between eighteen and twenty.
        TWENTY_ONE_OR_OLDER, ///< Twenty-one and older.
    };

    /** Constructs a new AgeRangeType **/
    explicit AgeRangeType();
    AgeRangeType(const AgeRangeType &);
    AgeRangeType(AgeRangeType &&) noexcept;
    AgeRangeType &operator=(const AgeRangeType &);
    AgeRangeType &operator=(AgeRangeType &&) noexcept;
    /** Destructor. **/
    ~AgeRangeType();

    bool operator==(const AgeRangeType &) const;
    bool operator!=(const AgeRangeType &) const;

    [[nodiscard]] static AgeRangeType fromJSON(const QJsonObject &obj);
    [[nodiscard]] static QList<AgeRangeType> fromJSONArray(const QJsonArray &data);
    [[nodiscard]] QJsonValue toJSON() const;

    /** The age range. **/
    [[nodiscard]] AgeRangeType::AgeRange ageRange() const;
    /** Sets value of the ageRange property. **/
    void setAgeRange(AgeRangeType::AgeRange value);

    /** Metadata about the age range. **/
    [[nodiscard]] FieldMetadata metadata() const;
    /** Sets value of the metadata property. **/
    void setMetadata(const FieldMetadata &value);

private:
    class Private;
    QSharedDataPointer<Private> d;
}; // AgeRangeType

} // namespace KGAPI2::People
