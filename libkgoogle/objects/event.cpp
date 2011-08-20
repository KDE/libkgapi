/*
    libKGoogle - KGoogleObject - Event
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


#include "event.h"
#include "event_p.h"

#include <qvariant.h>
#include <qstring.h>
#include <qdebug.h>

using namespace KGoogle;
using namespace KCalCore;

Object::EventData::EventData(const Object::EventData &other):
    QSharedData(other),
    deleted(other.deleted),
    id(other.id),
    etag(other.etag)
{ }


Object::Event::Event()
{
    d = new EventData;
}

Object::Event::Event(const Object::Event &other):
  KGoogleObject(other),
  KCalCore::Event(other),
  d(other.d)
{ }

Object::Event::Event(const KCalCore::Event &event): 
  KCalCore::Event(event),
  d(new EventData)
{ }


Object::Event::~Event()
{ }

void Object::Event::setDeleted(const bool deleted)
{
  d->deleted = deleted;
}

bool Object::Event::deleted()
{
  return d->deleted;
}

Object::Event& Object::Event::operator=( const Object::Event& other )
{
  d = other.d;
  return *this;
}
