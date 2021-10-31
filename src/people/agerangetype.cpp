/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "agerangetype.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
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
    Q_UNUSED(obj);
    return AgeRangeType();
}

QJsonValue AgeRangeType::toJSON() const
{
    QJsonObject obj;

    switch (d->ageRange) {
    case AgeRange::AGE_RANGE_UNSPECIFIED:
        obj.insert(QStringView{u"ageRange"}, QStringLiteral(u"AGE_RANGE_UNSPECIFIED"));
        break;
    case AgeRange::LESS_THAN_EIGHTEEN:
        obj.insert(QStringView{u"ageRange"}, QStringLiteral(u"LESS_THAN_EIGHTEEN"));
        break;
    case AgeRange::EIGHTEEN_TO_TWENTY:
        obj.insert(QStringView{u"ageRange"}, QStringLiteral(u"EIGHTEEN_TO_TWENTY"));
        break;
    case AgeRange::TWENTY_ONE_OR_OLDER:
        obj.insert(QStringView{u"ageRange"}, QStringLiteral(u"TWENTY_ONE_OR_OLDER"));
        break;
    }
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    return obj;
}

} // namespace KGAPI2::People
