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

#include "permission.h"
#include "permission_p.h"

using namespace KGAPI::Objects;

///// DrivePermissionPrivate

DrivePermissionPrivate::DrivePermissionPrivate(const DrivePermissionPrivate& other):
    id(other.id),
    selfLink(other.selfLink),
    name(other.name),
    role(other.role),
    additionalRoles(other.additionalRoles),
    type(other.type),
    authKey(other.authKey),
    withLink(other.withLink),
    photoLink(other.photoLink),
    value(other.value)
{ }

///// DrivePermission

DrivePermission::DrivePermission()
{ }

DrivePermission::DrivePermission(const DrivePermission& other):
    d(new DrivePermissionPrivate(*(other.d)))
{ }

DrivePermission::~DrivePermission()
{
    delete d;
}

QString DrivePermission::id()
{
    return d->id;
}

void DrivePermission::setId(const QString& id)
{
    d->id = id;
}

QString DrivePermission::selfLink()
{
    return d->selfLink;
}

void DrivePermission::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QString DrivePermission::name()
{
    return d->name;
}

void DrivePermission::setName(const QString& name)
{
    d->name = name;
}

QString DrivePermission::role()
{
    return d->role;
}

void DrivePermission::setRole(const QString& role)
{
    d->role = role;
}

QStringList DrivePermission::additionalRoles()
{
    return d->additionalRoles;
}

void DrivePermission::setAdditionalRoles(const QStringList& additionalRoles)
{
    d->additionalRoles = additionalRoles;
}

QString DrivePermission::type()
{
    return d->type;
}

void DrivePermission::setType(const QString& type)
{
    d->type = type;
}

QString DrivePermission::authKey()
{
    return d->authKey;
}

void DrivePermission::setAuthKey(const QString& authKey)
{
    d->authKey = authKey;
}

QVariant DrivePermission::withLink()
{
    return d->withLink;
}

void DrivePermission::setWithLink(const QVariant& withLink)
{
    d->withLink = withLink;
}

QString DrivePermission::photoLink()
{
    return d->photoLink;
}

void DrivePermission::setPhotoLink(const QString& photoLink)
{
    d->photoLink = photoLink;
}

QString DrivePermission::value()
{
    return d->value;
}

void DrivePermission::setValue(const QString& value)
{
    d->value = value;
}
