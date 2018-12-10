/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "calendar.h"
#include "reminder.h"
#include "../debug.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN Calendar::Private
{
  public:
    Private();
    Private(const Private &other);
    ~Private() {}

    QString uid;
    QString title;
    QString details;
    QString timezone;
    QString location;
    bool editable = false;
    QColor backgroundColor;
    QColor foregroundColor;

    RemindersList reminders;
};

Calendar::Private::Private()
{
}

Calendar::Private::Private(const Private &other) :
    uid(other.uid),
    title(other.title),
    details(other.details),
    timezone(other.timezone),
    location(other.location),
    editable(other.editable),
    backgroundColor(other.backgroundColor),
    foregroundColor(other.foregroundColor),
    reminders(other.reminders)
{
}

Calendar::Calendar() :
    Object(),
    d(new Private)
{
}

Calendar::Calendar(const Calendar &other) :
    Object(other),
    d(new Private(*(other.d)))
{
}

Calendar::~Calendar()
{
    delete d;
}

bool Calendar::operator==(const Calendar &other) const
{
    if (d->uid != other.d->uid) {
        qCDebug(KGAPIDebug) << "UIDs don't match";
        return false;
    }
    if (d->title != other.d->title) {
        qCDebug(KGAPIDebug) << "Titles don't match";
        return false;
    }
    if (d->details != other.d->details) {
        qCDebug(KGAPIDebug) << "Details don't match";
        return false;
    }
    if (d->timezone != other.d->timezone) {
        qCDebug(KGAPIDebug) << "Timezones don't match";
        return false;
    }
    if (d->location != other.d->location) {
        qCDebug(KGAPIDebug) << "Locations don't match";
        return false;
    }
    if (d->editable != other.d->editable) {
        qCDebug(KGAPIDebug) << "Editable doesn't match";
        return false;
    }
    if (d->backgroundColor != other.d->backgroundColor) {
        qCDebug(KGAPIDebug) << "BackgroundColors don't match";
        return false;
    }
    if (d->foregroundColor != other.d->foregroundColor) {
        qCDebug(KGAPIDebug) << "ForegroundColors don't match";
        return false;
    }

    for (const auto &reminder : qAsConst(d->reminders)) {
        if (std::find_if(other.d->reminders.cbegin(), other.d->reminders.cend(),
                        [reminder](const ReminderPtr &otherReminder) {
                            return *reminder == *otherReminder;
                        }) == other.d->reminders.cend()) {
            qCDebug(KGAPIDebug) << "Reminders don't match";
            return false;
        }
    }

    return true;
}


void Calendar::setUid(const QString &uid)
{
    d->uid = uid;
}

QString Calendar::uid() const
{
    return d->uid;
}

QString Calendar::title() const
{
    return d->title;
}

void Calendar::setTitle(const QString &title)
{
    d->title = title;
}

QString Calendar::details() const
{
    return d->details;
}

void Calendar::setDetails(const QString &details)
{
    d->details = details;
}

QString Calendar::location() const
{
    return d->location;
}

void Calendar::setLocation(const QString &location)
{
    d->location = location;
}

QString Calendar::timezone() const
{
    return d->timezone;
}

void Calendar::setTimezone(const QString &timezone)
{
    d->timezone = timezone;
}

bool Calendar::editable() const
{
    return d->editable;
}

void Calendar::setEditable(const bool editable)
{
    d->editable = editable;
}

void Calendar::setDefaultReminders(const RemindersList &reminders)
{
    d->reminders = reminders;
}

void Calendar::addDefaultReminer(const ReminderPtr &reminder)
{
    d->reminders.append(reminder);
}

RemindersList Calendar::defaultReminders() const
{
    return d->reminders;
}

QColor Calendar::backgroundColor() const
{
    return d->backgroundColor;
}

void Calendar::setBackgroundColor(const QColor &color)
{
    d->backgroundColor = color;
}

QColor Calendar::foregroundColor() const
{
    return d->foregroundColor;
}

void Calendar::setForegroundColor(const QColor &color)
{
    d->foregroundColor = color;
}
