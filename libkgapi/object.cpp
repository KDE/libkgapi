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

namespace KGAPI
{

/**
 * \internal
 */
class ObjectPrivate: public QSharedData
{
  public:
    ObjectPrivate() { }

    QString etag;
};

}

using namespace KGAPI;

Object::Object():
    d(new ObjectPrivate)
{ }

Object::Object(const Object& other):
    d(other.d)
{ }


Object::~Object()
{ }

void Object::setEtag(const QString& etag)
{
    d->etag = etag;
}

const QString& Object::etag() const
{
    return d->etag;
}
