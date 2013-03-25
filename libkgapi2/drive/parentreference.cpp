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

#include "parentreference.h"

using namespace KGAPI2;

class DriveParentReference::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QString selfLink;
    QString parentLink;
    bool isRoot;
};

DriveParentReference::Private::Private():
    isRoot(false)
{
}

DriveParentReference::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    parentLink(other.parentLink),
    isRoot(other.isRoot)
{
}

DriveParentReference::DriveParentReference():
    d(new Private)
{
}

DriveParentReference::DriveParentReference(const DriveParentReference &other):
    d(new Private(*(other.d)))
{
}

DriveParentReference::~DriveParentReference()
{
    delete d;
}

QString DriveParentReference::id() const
{
    return d->id;
}

void DriveParentReference::setId(const QString &id)
{
    d->id = id;
}

QString DriveParentReference::selfLink() const
{
    return d->selfLink;
}

void DriveParentReference::setSelfLink(const QString &selfLink)
{
    d->selfLink = selfLink;
}

QString DriveParentReference::parentLink() const
{
    return d->parentLink;
}

void DriveParentReference::setParentLink(const QString &parentLink)
{
    d->parentLink = parentLink;
}

bool DriveParentReference::isRoot() const
{
    return d->isRoot;
}

void DriveParentReference::setIsRoot(bool isRoot)
{
    d->isRoot = isRoot;
}
