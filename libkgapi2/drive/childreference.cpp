/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

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

#include "childreference.h"

using namespace KGAPI2;

class DriveChildReference::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QString selfLink;
    QString childLink;
};

DriveChildReference::Private::Private()
{
}

DriveChildReference::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    childLink(other.childLink)
{
}

DriveChildReference::DriveChildReference():
    d(new Private)
{
}

DriveChildReference::DriveChildReference(const DriveChildReference &other):
    d(new Private(*(other.d)))
{
}

DriveChildReference::~DriveChildReference()
{
    delete d;
}

QString DriveChildReference::id() const
{
    return d->id;
}

void DriveChildReference::setId(const QString &id)
{
    d->id = id;
}

QString DriveChildReference::selfLink() const
{
    return d->selfLink;
}

void DriveChildReference::setSelfLink(const QString &selfLink)
{
    d->selfLink = selfLink;
}

QString DriveChildReference::childLink() const
{
    return d->childLink;
}

void DriveChildReference::setChildLink(const QString &childLink)
{
    d->childLink = childLink;
}
