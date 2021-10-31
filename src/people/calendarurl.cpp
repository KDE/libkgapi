/*
 * SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-License-Identifier: LGPL-3.0-only
 * SPDX-License-Identifier: LicenseRef-KDE-Accepted-LGPL
 */

#include "calendarurl.h"

#include "fieldmetadata.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QSharedData>

#include <algorithm>

namespace KGAPI2::People
{
class CalendarUrl::Private : public QSharedData
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
        return url == other.url && metadata == other.metadata && type == other.type && formattedType == other.formattedType;
    }

    bool operator!=(const Private &other) const
    {
        return !(*this == other);
    }

    QString url{};
    FieldMetadata metadata{};
    QString type{};
    QString formattedType{};
};

CalendarUrl::CalendarUrl()
    : d(new Private)
{
}

CalendarUrl::CalendarUrl(const CalendarUrl &) = default;
CalendarUrl::CalendarUrl(CalendarUrl &&) noexcept = default;
CalendarUrl &CalendarUrl::operator=(const CalendarUrl &) = default;
CalendarUrl &CalendarUrl::operator=(CalendarUrl &&) noexcept = default;
CalendarUrl::~CalendarUrl() = default;

bool CalendarUrl::operator==(const CalendarUrl &other) const
{
    return *d == *other.d;
}

bool CalendarUrl::operator!=(const CalendarUrl &other) const
{
    return !(*this == other);
}

QString CalendarUrl::url() const
{
    return d->url;
}

void CalendarUrl::setUrl(const QString &value)
{
    d->url = value;
}
FieldMetadata CalendarUrl::metadata() const
{
    return d->metadata;
}

void CalendarUrl::setMetadata(const FieldMetadata &value)
{
    d->metadata = value;
}
QString CalendarUrl::type() const
{
    return d->type;
}

void CalendarUrl::setType(const QString &value)
{
    d->type = value;
}
QString CalendarUrl::formattedType() const
{
    return d->formattedType;
}

CalendarUrl CalendarUrl::fromJSON(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    return CalendarUrl();
}

QJsonValue CalendarUrl::toJSON() const
{
    QJsonObject obj;

    obj.insert(QStringView{u"url"}, d->url);
    obj.insert(QStringView{u"metadata"}, d->metadata.toJSON());
    obj.insert(QStringView{u"type"}, d->type);
    obj.insert(QStringView{u"formattedType"}, d->formattedType);
    return obj;
}

} // namespace KGAPI2::People
