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

using namespace KGoogle::Object;

CalendarData::CalendarData(const CalendarData &other):
  QSharedData(other),
  title(other.title),
  color(other.color),
  details(other.details),
  timezone(other.timezone),
  editable(other.editable),
  created(other.created),
  updated(other.updated)
{ }

Calendar::Calendar():
  d(new CalendarData)
{ }

Calendar::Calendar(const Calendar &other):
  KGoogleObject(other),
  d(other.d)
{ }

Calendar::~Calendar()
{ }


QString Calendar::title()
{
  return d->title.isEmpty() ? "Google Calendar" : d->title;
}

void Calendar::setTitle(const QString &title)
{
  d->title = title;
}

QString Calendar::color()
{
  return d->color.isEmpty() ? "#000000" : d->color;
}

void Calendar::setColor(const QString &color)
{
  d->color = color;
}

QString Calendar::details()
{
  return d->details;
}

void Calendar::setDetails(const QString& details)
{
  d->details = details;
}

QString Calendar::timezone()
{
  return d->timezone;
}

void Calendar::setTimezone(const QString& timezone)
{
  d->timezone = timezone;
}

bool Calendar::editable()
{
  return d->editable;
}

void Calendar::setEditable(const bool editable)
{
  d->editable = editable;
}

KDateTime Calendar::created()
{
  return d->created;
}

void Calendar::setCreated(const KDateTime& created)
{
  d->created = created;
}

KDateTime Calendar::updated()
{
  return d->updated;
}

void Calendar::setUpdated(const KDateTime& updated)
{
  d->updated = updated;
}
