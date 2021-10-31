/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "location.h"

#include "fieldmetadata.h"

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

void Location::setCurrent(const bool &value)
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
    Q_UNUSED(obj);
    return Location();
}

QJsonValue Location::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"deskCode"}, d->deskCode);
    obj.insert(QStringView{u"floor"}, d->floor);
    obj.insert(QStringView{u"buildingId"}, d->buildingId);
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"current"}, d->current);
    obj.insert(QStringView{u"value"}, d->value);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"floorSection"}, d->floorSection);
    return obj;
}

} // namespace KGAPI2::People
