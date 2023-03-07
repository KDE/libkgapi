/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "event.h"

#include "fieldmetadata.h"
#include "peopleservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class Event::Private : public QSharedData
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
        return metadata == other.metadata && date == other.date && type == other.type && formattedType == other.formattedType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    FieldMetadata metadata{};
    QDate date{};
    QString type{};
    QString formattedType{};
};

Event::Event()
    : d(new Private)
{
}

Event::Event(const Event &) = default;
Event::Event(Event &&) noexcept = default;
Event &Event::operator=(const Event &) = default;
Event &Event::operator=(Event &&) noexcept = default;
Event::~Event() = default;

bool Event::operator==(const Event &other) const
{
    return *d == *other.d;
}

bool Event::operator!=(const Event &other) const
{
    return !(*this == other);
}

FieldMetadata Event::metadata() const
{
    return d->metadata;
}

void Event::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QDate Event::date() const
{
    return d->date;
}

void Event::setDate(const QDate &value)
{
    d->date = value;
}
QString Event::type() const
{
    return d->type;
}

void Event::setType(const QString &value)
{
    d->type = value;
}
QString Event::formattedType() const
{
    return d->formattedType;
}

Event Event::fromJSON(const QJsonObject &obj)
{
    Event event;

    if(!obj.isEmpty()) {
        const auto metadata = obj.value(QStringLiteral("metadata")).toObject();
        event.d->metadata = FieldMetadata::fromJSON(metadata);

        const auto jsonDate = obj.value(QStringLiteral("date")).toObject();
        const auto year = jsonDate.value(QStringLiteral("year")).toInt();
        const auto month = jsonDate.value(QStringLiteral("month")).toInt();
        const auto day = jsonDate.value(QStringLiteral("day")).toInt();
        event.d->date = QDate(year, month, day);

        event.d->type = obj.value(QStringLiteral("type")).toString();
        event.d->formattedType = obj.value(QStringLiteral("formattedType")).toString();
    }

    return event;
}

QVector<Event> Event::fromJSONArray(const QJsonArray &data)
{
    QVector<Event> events;

    for(const auto &event : data) {
        if(event.isObject()) {
            const auto objectifiedEvent = event.toObject();
            events.append(fromJSON(objectifiedEvent));
        }
    }

    return events;
}

QJsonValue Event::toJSON() const
{
    // Skip field metadata as is only useful for receiving
    // formattedType is output only
    QJsonObject obj {
        { QStringLiteral("date"), QJsonObject {
            { QStringLiteral("year"), d->date.year() },
            { QStringLiteral("month"), d->date.month() },
            { QStringLiteral("day"), d->date.day() }
        } }
    };

    PeopleUtils::addValueToJsonObjectIfValid(obj, "type", d->type);

    return obj;
}

} // namespace KGAPI2::People
