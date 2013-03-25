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

#include "revision.h"

using namespace KGAPI2;

class DriveRevision::Private
{
public:
    Private();
    Private(const Private& other);

    QString id;
    QUrl selfLink;
    QString mimeType;
    KDateTime modifiedDate;
    bool pinned;
    bool published;
    QUrl publishedLink;
    bool publishedAuto;
    bool publishedOutsideDomain;
    QUrl downloadUrl;
    QMap<QString, QUrl> exportLinks;
    QString lastModifyingUserName;
    QString originalFilename;
    QString md5Checksum;
    qlonglong fileSize;
};

DriveRevision::Private::Private():
    pinned(false),
    published(false),
    publishedAuto(false),
    publishedOutsideDomain(false),
    fileSize(-1)
{
}

DriveRevision::Private::Private(const Private& other):
    id(other.id),
    selfLink(other.selfLink),
    mimeType(other.mimeType),
    modifiedDate(other.modifiedDate),
    pinned(other.pinned),
    published(other.published),
    publishedLink(other.publishedLink),
    publishedAuto(other.publishedAuto),
    publishedOutsideDomain(other.publishedOutsideDomain),
    downloadUrl(other.downloadUrl),
    exportLinks(other.exportLinks),
    lastModifyingUserName(other.lastModifyingUserName),
    originalFilename(other.originalFilename),
    md5Checksum(other.md5Checksum),
    fileSize(other.fileSize)
{
}

DriveRevision::DriveRevision():
    d(new Private)
{
}

DriveRevision::DriveRevision(const DriveRevision& other):
    d(new Private(*(other.d)))
{
}

DriveRevision::~DriveRevision()
{
    delete d;
}

QString DriveRevision::id() const
{
    return d->id;
}

void DriveRevision::setId(const QString& id)
{
    d->id = id;
}

QUrl DriveRevision::selfLink() const
{
    return d->selfLink;
}

void DriveRevision::setSelfLink(const QUrl &selfLink)
{
    d->selfLink = selfLink;
}

QString DriveRevision::mimeType() const
{
    return d->mimeType;
}

void DriveRevision::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

KDateTime DriveRevision::modifiedDate() const
{
    return d->modifiedDate;
}

void DriveRevision::setModifiedDate(const KDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

bool DriveRevision::pinned() const
{
    return d->pinned;
}

void DriveRevision::setPinned(bool pinned)
{
    d->pinned = pinned;
}

bool DriveRevision::published() const
{
    return d->published;
}

void DriveRevision::setPublished(bool published)
{
    d->published = published;
}

QUrl DriveRevision::publishedLink() const
{
    return d->publishedLink;
}

void DriveRevision::setPublishedLink(const QUrl &publishedLink)
{
    d->publishedLink = publishedLink;
}

bool DriveRevision::publishedAuto() const
{
    return d->publishedAuto;
}

void DriveRevision::setPublishedAuto(bool publishedAuto)
{
    d->publishedAuto = publishedAuto;
}

bool DriveRevision::publishedOutsideDomain() const
{
    return d->publishedOutsideDomain;
}

void DriveRevision::setPublishedOutsideDomain(bool publishedOutsideDomain)
{
    d->publishedOutsideDomain = publishedOutsideDomain;
}

QUrl DriveRevision::downloadUrl() const
{
    return d->downloadUrl;
}

void DriveRevision::setDownloadUrl(const QUrl &downloadUrl)
{
    d->downloadUrl = downloadUrl;
}

QMap< QString, QUrl > DriveRevision::exportLinks() const
{
    return d->exportLinks;
}

void DriveRevision::setExportLinks(const QMap<QString, QUrl>& exportLinks)
{
    d->exportLinks = exportLinks;
}

QString DriveRevision::lastModifyingUserName() const
{
    return d->lastModifyingUserName;
}

void DriveRevision::setLastModifyingUserName(const QString& lastModifyingUserName)
{
    d->lastModifyingUserName = lastModifyingUserName;
}

QString DriveRevision::originalFilename() const
{
    return d->originalFilename;
}

void DriveRevision::setOriginalFilename(const QString& originalFilename)
{
    d->originalFilename = originalFilename;
}

QString DriveRevision::md5Checksum() const
{
    return d->md5Checksum;
}

void DriveRevision::setMd5Checksum(const QString& md5Checksum)
{
    d->md5Checksum = md5Checksum;
}

qlonglong DriveRevision::fileSize() const
{
    return d->fileSize;
}

void DriveRevision::setFileSize(qlonglong fileSize)
{
    d->fileSize = fileSize;
}
