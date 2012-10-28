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

#include "change.h"
#include "change_p.h"

using namespace KGAPI::Objects;

///// DriveChangePrivate

DriveChangePrivate::DriveChangePrivate(const DriveChangePrivate& other):
    id(other.id),
    fileId(other.fileId),
    selfLink(other.selfLink),
    deleted(other.deleted),
    file(other.file)
{ }

///// DriveChange

DriveChange::DriveChange()
{ }

DriveChange::DriveChange(const DriveChange& other):
    d(new DriveChangePrivate(*(other.d)))
{ }

DriveChange::~DriveChange()
{
    delete d;
}

qlonglong DriveChange::id()
{
    return d->id;
}

void DriveChange::setId(const qlonglong& id)
{
    d->id = id;
}

QString DriveChange::fileId()
{
    return d->fileId;
}

void DriveChange::setFileId(const QString& fileId)
{
    d->fileId = fileId;
}

QString DriveChange::selfLink()
{
    return d->selfLink;
}

void DriveChange::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QVariant DriveChange::deleted()
{
    return d->deleted;
}

void DriveChange::setDeleted(const QVariant& deleted)
{
    d->deleted = deleted;
}

DriveFile::Ptr DriveChange::file()
{
    return d->file;
}

void DriveChange::setFile(const DriveFile::Ptr& file)
{
    d->file = file;
}
