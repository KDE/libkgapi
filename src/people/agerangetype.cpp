/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "agerangetype.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class AgeRangeType::Private : public QSharedData
{
public:
    explicit Private() = default;
    Private(const Private &) = default;
    Private(Private &&) noexcept = delete;
    Private &operator=(const Private &) = delete;
    Private &operator=(Private &&) noexcept = delete;
    ~Private() = default;

    bool operator==(const Private &other) const
    {
        return ageRange == other.ageRange && metadata == other.metadata;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    AgeRangeType::AgeRange ageRange{};
    FieldMetadata metadata{};
};

AgeRangeType::AgeRangeType()
    : d(new Private)
{
}

AgeRangeType::AgeRangeType(const AgeRangeType &) = default;
AgeRangeType::AgeRangeType(AgeRangeType &&) noexcept = default;
AgeRangeType &AgeRangeType::operator=(const AgeRangeType &) = default;
AgeRangeType &AgeRangeType::operator=(AgeRangeType &&) noexcept = default;
AgeRangeType::~AgeRangeType() = default;

bool AgeRangeType::operator==(const AgeRangeType &other) const
{
    return *d == *other.d;
}

bool AgeRangeType::operator!=(const AgeRangeType &other) const
{
    return !(*this == other);
}

AgeRangeType::AgeRangeType::AgeRange AgeRangeType::ageRange() const
{
    return d->ageRange;
}

void AgeRangeType::setAgeRange(const AgeRangeType::AgeRange &value)
{
    d->ageRange = value;
}
FieldMetadata AgeRangeType::metadata() const
{
    return d->metadata;
}

void AgeRangeType::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}

AgeRangeType AgeRangeType::fromJSON(const QJsonObject &obj)
{
    AgeRangeType ageRangeType;

    if(!obj.isEmpty()) {
        const auto val = obj.value(QStringLiteral("ageRange")).toString();

        if(val == QStringLiteral("LESS_THAN_EIGHTEEN")) {
            ageRangeType.setAgeRange(AgeRange::LESS_THAN_EIGHTEEN);
        } else if(val == QStringLiteral("EIGHTEEN_TO_TWENTY")) {
            ageRangeType.setAgeRange(AgeRange::EIGHTEEN_TO_TWENTY);
        } else if(val == QStringLiteral("TWENTY_ONE_OR_OLDER")) {
            ageRangeType.setAgeRange(AgeRange::TWENTY_ONE_OR_OLDER);
        } else {
            ageRangeType.setAgeRange(AgeRange::AGE_RANGE_UNSPECIFIED);
        }
    }

    return ageRangeType;
}

QJsonValue AgeRangeType::toJSON() const
{
    QJsonObject obj;

    switch (d->ageRange) {
    case AgeRange::AGE_RANGE_UNSPECIFIED:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "ageRange", QStringLiteral("AGE_RANGE_UNSPECIFIED"));
        break;
    case AgeRange::LESS_THAN_EIGHTEEN:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "ageRange", QStringLiteral("LESS_THAN_EIGHTEEN"));
        break;
    case AgeRange::EIGHTEEN_TO_TWENTY:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "ageRange", QStringLiteral("EIGHTEEN_TO_TWENTY"));
        break;
    case AgeRange::TWENTY_ONE_OR_OLDER:
        PeopleUtils::addValueToJsonObjectIfValid(obj, "ageRange", QStringLiteral("TWENTY_ONE_OR_OLDER"));
        break;
    }
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata", d->metadata.toJSON());
    return obj;
}

QVector<AgeRangeType> AgeRangeType::fromJSONArray(const QJsonArray& data)
{
    QVector<People::AgeRangeType> ageRanges;

    for(const auto &ageRangeObj : data) {
        if(ageRangeObj.isObject()) {
            const auto objectifiedAgeRange = ageRangeObj.toObject();
            if(objectifiedAgeRange.contains(QStringLiteral("ageRange"))) {
                ageRanges.append(fromJSON(objectifiedAgeRange));
            }
        }
    }

    return ageRanges;
}

} // namespace KGAPI2::People
