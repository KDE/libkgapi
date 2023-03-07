/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "location.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{

class Location::Private : public QSharedData
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
        return deskCode == other.deskCode && floor == other.floor && buildingId == other.buildingId && type == other.type && current == other.current
            && value == other.value && metadata == other.metadata && floorSection == other.floorSection;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString deskCode{};
    QString floor{};
    QString buildingId{};
    QString type{};
    bool current{};
    QString value{};
    FieldMetadata metadata{};
    QString floorSection{};
};

Location::Location()
    : d(new Private)
{
}

Location::Location(const Location &) = default;
Location::Location(Location &&) noexcept = default;
Location &Location::operator=(const Location &) = default;
Location &Location::operator=(Location &&) noexcept = default;
Location::~Location() = default;

bool Location::operator==(const Location &other) const
{
    return *d == *other.d;
}

bool Location::operator!=(const Location &other) const
{
    return !(*this == other);
}

QString Location::deskCode() const
{
    return d->deskCode;
}

void Location::setDeskCode(const QString &value)
{
    d->deskCode = value;
}
QString Location::floor() const
{
    return d->floor;
}

void Location::setFloor(const QString &value)
{
    d->floor = value;
}
QString Location::buildingId() const
{
    return d->buildingId;
}

void Location::setBuildingId(const QString &value)
{
    d->buildingId = value;
}
QString Location::type() const
{
    return d->type;
}

void Location::setType(const QString &value)
{
    d->type = value;
}
bool Location::current() const
{
    return d->current;
}

void Location::setCurrent(bool value)
{
    d->current = value;
}
QString Location::value() const
{
    return d->value;
}

void Location::setValue(const QString &value)
{
    d->value = value;
}
FieldMetadata Location::metadata() const
{
    return d->metadata;
}

void Location::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString Location::floorSection() const
{
    return d->floorSection;
}

void Location::setFloorSection(const QString &value)
{
    d->floorSection = value;
}

Location Location::fromJSON(const QJsonObject &obj)
{
    Location location;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        location.setMetadata(FieldMetadata::fromJSON(metadata));
        location.setValue(obj.value(QStringLiteral("value")).toString());
        location.setType(obj.value(QStringLiteral("type")).toString());
        location.setCurrent(obj.value(QStringLiteral("current")).toBool());
        location.setBuildingId(obj.value(QStringLiteral("buildingId")).toString());
        location.setFloor(obj.value(QStringLiteral("floor")).toString());
        location.setFloorSection(obj.value(QStringLiteral("floorSection")).toString());
        location.setDeskCode(obj.value(QStringLiteral("deskCode")).toString());
    }

    return location;
}

QVector<Location> Location::fromJSONArray(const QJsonArray &data)
{
    QVector<Location> locations;

    for(const auto &location : data) {
        if(location.isObject()) {
            const auto objectifiedLocation = location.toObject();
            locations.append(fromJSON(objectifiedLocation));
        }
    }

    return locations;
}

QJsonValue Location::toJSON() const
{
    QJsonObject obj;

    PeopleUtils::addValueToJsonObjectIfValid(obj, "deskCode", d->deskCode);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "floor", d->floor);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "buildingId", d->buildingId);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "type", d->type);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "current", d->current);
    PeopleUtils::addValueToJsonObjectIfValid(obj, "value", d->value);
    // Skip, field metadata is only useful for receiving -> PeopleUtils::addValueToJsonObjectIfValid(obj, "metadata"}, d->metadata.toJSON());
    PeopleUtils::addValueToJsonObjectIfValid(obj, "floorSection", d->floorSection);
    return obj;
}

} // namespace KGAPI2::People
