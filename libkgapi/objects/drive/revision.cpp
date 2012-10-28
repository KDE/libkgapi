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
#include "revision_p.h"

using namespace KGAPI::Objects;

///// DriveRevisionPrivate

DriveRevisionPrivate::DriveRevisionPrivate(const DriveRevisionPrivate& other):
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
{ }

///// DriveRevision

DriveRevision::DriveRevision()
{ }

DriveRevision::DriveRevision(const DriveRevision& other):
    d(new DriveRevisionPrivate(*(other.d)))
{ }

DriveRevision::~DriveRevision()
{
    delete d;
}

QString DriveRevision::id()
{
    return d->id;
}

void DriveRevision::setId(const QString& id)
{
    d->id = id;
}

QString DriveRevision::selfLink()
{
    return d->selfLink;
}

void DriveRevision::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QString DriveRevision::mimeType()
{
    return d->mimeType;
}

void DriveRevision::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

KDateTime DriveRevision::modifiedDate()
{
    return d->modifiedDate;
}

void DriveRevision::setModifiedDate(const KDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

QVariant DriveRevision::pinned()
{
    return d->pinned;
}

void DriveRevision::setPinned(const QVariant& pinned)
{
    d->pinned = pinned;
}

QVariant DriveRevision::published()
{
    return d->published;
}

void DriveRevision::setPublished(const QVariant& published)
{
    d->published = published;
}

QString DriveRevision::publishedLink()
{
    return d->publishedLink;
}

void DriveRevision::setPublishedLink(const QString& publishedLink)
{
    d->publishedLink = publishedLink;
}

QVariant DriveRevision::publishedAuto()
{
    return d->publishedAuto;
}

void DriveRevision::setPublishedAuto(const QVariant& publishedAuto)
{
    d->publishedAuto = publishedAuto;
}

QVariant DriveRevision::publishedOutsideDomain()
{
    return d->publishedOutsideDomain;
}

void DriveRevision::setPublishedOutsideDomain(const QVariant& publishedOutsideDomain)
{
    d->publishedOutsideDomain = publishedOutsideDomain;
}

QString DriveRevision::downloadUrl()
{
    return d->downloadUrl;
}

void DriveRevision::setDownloadUrl(const QString& downloadUrl)
{
    d->downloadUrl = downloadUrl;
}

QMap<QString, QString> DriveRevision::exportLinks()
{
    return d->exportLinks;
}

void DriveRevision::setExportLinks(const QMap<QString, QString>& exportLinks)
{
    d->exportLinks = exportLinks;
}

QString DriveRevision::lastModifyingUserName()
{
    return d->lastModifyingUserName;
}

void DriveRevision::setLastModifyingUserName(const QString& lastModifyingUserName)
{
    d->lastModifyingUserName = lastModifyingUserName;
}

QString DriveRevision::originalFilename()
{
    return d->originalFilename;
}

void DriveRevision::setOriginalFilename(const QString& originalFilename)
{
    d->originalFilename = originalFilename;
}

QString DriveRevision::md5Checksum()
{
    return d->md5Checksum;
}

void DriveRevision::setMd5Checksum(const QString& md5Checksum)
{
    d->md5Checksum = md5Checksum;
}

qlonglong DriveRevision::fileSize()
{
    return d->fileSize;
}

void DriveRevision::setFileSize(const qlonglong& fileSize)
{
    d->fileSize = fileSize;
}
