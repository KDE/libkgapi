/*
    libKGoogle - KGoogleObject - Calendar
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calendar.h"
#include "calendar_p.h"

#include <qdebug.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGoogle::Objects;

CalendarData::CalendarData(const CalendarData &other) :
  QSharedData(other),
  title(other.title),
  color(other.color),
  details(other.details),
  timezone(other.timezone),
  location(other.location),
  editable(other.editable),
  created(other.created),
  updated(other.updated)
{ }

Calendar::Calendar() :
  d(new CalendarData)
{ }

Calendar::Calendar(const Calendar &other) :
  KGoogle::Object(other),
  d(other.d)
{ }

Calendar::~Calendar()
{ }


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
  return d->title.isEmpty() ? "Google Calendar" : d->title;
}

void Calendar::setTitle(const QString &title)
{
  d->title = title;
}

QString Calendar::color() const
{
  return d->color.isEmpty() ? "#000000" : d->color;
}

void Calendar::setColor(const QString &color)
{
  d->color = color;
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

KDateTime Calendar::created() const
{
  return d->created;
}

void Calendar::setCreated(const KDateTime &created)
{
  d->created = created;
}

KDateTime Calendar::updated() const
{
  return d->updated;
}

void Calendar::setUpdated(const KDateTime &updated)
{
  d->updated = updated;
}
