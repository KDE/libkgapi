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

using namespace KGAPI2;

class DriveChange::Private
{
  public:
    Private();
    Private(const Private &other);

    qlonglong id;
    QString fileId;
    QUrl selfLink;
    bool deleted;
    DriveFilePtr file;
};

DriveChange::Private::Private():
    id(-1),
    deleted(false)
{
}

DriveChange::Private::Private(const Private &other):
    id(other.id),
    fileId(other.fileId),
    selfLink(other.selfLink),
    deleted(other.deleted),
    file(other.file)
{
}

DriveChange::DriveChange():
    d(new Private)
{
}

DriveChange::DriveChange(const DriveChange &other):
    d(new Private(*(other.d)))
{
}

DriveChange::~DriveChange()
{
    delete d;
}

qlonglong DriveChange::id() const
{
    return d->id;
}

void DriveChange::setId(const qlonglong &id)
{
    d->id = id;
}

QString DriveChange::fileId() const
{
    return d->fileId;
}

void DriveChange::setFileId(const QString &fileId)
{
    d->fileId = fileId;
}

QUrl DriveChange::selfLink() const
{
    return d->selfLink;
}

void DriveChange::setSelfLink(const QUrl &selfLink)
{
    d->selfLink = selfLink;
}

bool DriveChange::deleted() const
{
    return d->deleted;
}

void DriveChange::setDeleted(bool deleted)
{
    d->deleted = deleted;
}

DriveFilePtr DriveChange::file() const
{
    return d->file;
}

void DriveChange::setFile(const DriveFilePtr &file)
{
    d->file = file;
}
