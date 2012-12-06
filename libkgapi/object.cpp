/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "object.h"
#include "object_p.h"

using namespace KGAPI;

ObjectPrivate::ObjectPrivate(const ObjectPrivate& other):
    etag(other.etag)
{ }

Object::Object():
    d(new ObjectPrivate)
{ }

Object::Object(const Object& other):
    d(new ObjectPrivate(*(other.d)))
{ }

Object::~Object()
{
    delete d;
}

void Object::setEtag(const QString& etag)
{
    d->etag = etag;
}

QString Object::etag() const
{
    return d->etag;
}
