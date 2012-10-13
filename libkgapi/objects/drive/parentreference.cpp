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
#include "parentreference_p.h"

using namespace KGAPI::Objects;

///// DriveParentReferencePrivate

DriveParentReferencePrivate::DriveParentReferencePrivate(const DriveParentReferencePrivate& other):
    id(other.id),
    selfLink(other.selfLink),
    parentLink(other.parentLink),
    isRoot(other.isRoot)
{ }

///// DriveParentReference

DriveParentReference::DriveParentReference()
{ }

DriveParentReference::DriveParentReference(const DriveParentReference& other):
    d(new DriveParentReferencePrivate(*(other.d)))
{ }

DriveParentReference::~DriveParentReference()
{
    delete d;
}

QString DriveParentReference::id()
{
    return d->id;
}

void DriveParentReference::setId(const QString& id)
{
    d->id = id;
}

QString DriveParentReference::selfLink()
{
    return d->selfLink;
}

void DriveParentReference::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QString DriveParentReference::parentLink()
{
    return d->parentLink;
}

void DriveParentReference::setParentLink(const QString& parentLink)
{
    d->parentLink = parentLink;
}

bool DriveParentReference::isRoot()
{
    return d->isRoot;
}

void DriveParentReference::setIsRoot(const bool& isRoot)
{
    d->isRoot = isRoot;
}
