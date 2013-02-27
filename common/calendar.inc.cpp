/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

class Calendar::Private
{
  public:
    Private();
    Private(const Private &other);
    ~Private() {};

    QString uid;
    QString title;
    QString details;
    QString timezone;
    QString location;
    bool editable;

    RemindersList reminders;
};

Calendar::Private::Private():
    editable(true)
{
}

Calendar::Private::Private(const Private &other) :
    uid(other.uid),
    title(other.title),
    details(other.details),
    timezone(other.timezone),
    location(other.location),
    editable(other.editable),
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
    return d->title.isEmpty() ? i18n("Google Calendar") : d->title;
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
