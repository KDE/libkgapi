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

#include "file.h"
#include "file_p.h"

using namespace KGAPI::Objects;

///// DriveFilePrivate

DriveFilePrivate::DriveFilePrivate(const DriveFilePrivate& other):
    id(other.id),
    selfLink(other.selfLink),
    title(other.title),
    mimeType(other.mimeType),
    description(other.description),
    labels(other.labels),
    createdDate(other.createdDate),
    modifiedDate(other.modifiedDate),
    modifiedByMeDate(other.modifiedByMeDate),
    downloadUrl(other.downloadUrl),
    indexableText(other.indexableText),
    userPermission(other.userPermission),
    fileExtension(other.fileExtension),
    md5Checksum(other.md5Checksum),
    fileSize(other.fileSize),
    alternateLink(other.alternateLink),
    embedLink(other.embedLink),
    sharedWithMeDate(other.sharedWithMeDate),
    parents(other.parents),
    exportLinks(other.exportLinks),
    originalFileName(other.originalFileName),
    quotaBytesUsed(other.quotaBytesUsed),
    ownerNames(other.ownerNames),
    lastModifyingUserName(other.lastModifyingUserName),
    editable(other.editable),
    writersCanShare(other.writersCanShare),
    thumbnailLink(other.thumbnailLink),
    lastViewedByMeDate(other.lastViewedByMeDate),
    webContentLink(other.webContentLink),
    explicitlyTrashed(other.explicitlyTrashed),
    imageMediaMetadata(other.imageMediaMetadata)
{ }

///// DriveFile::Labels

DriveFile::Labels::Labels()
{ }

DriveFile::Labels::Labels(const Labels& other):
    d(new DriveFilePrivate::Labels(*(other.d)))
{ }

DriveFile::Labels::~Labels()
{
    delete d;
}

bool DriveFile::Labels::starred()
{
    return d->starred;
}

void DriveFile::Labels::setStarred(const bool& starred)
{
    d->starred = starred;
}

bool DriveFile::Labels::hidden()
{
    return d->hidden;
}

void DriveFile::Labels::setHidden(const bool& hidden)
{
    d->hidden = hidden;
}

bool DriveFile::Labels::trashed()
{
    return d->trashed;
}

void DriveFile::Labels::setTrashed(const bool& trashed)
{
    d->trashed = trashed;
}

bool DriveFile::Labels::restricted()
{
    return d->restricted;
}

void DriveFile::Labels::setRestricted(const bool& restricted)
{
    d->restricted = restricted;
}

bool DriveFile::Labels::viewed()
{
    return d->viewed;
}

void DriveFile::Labels::setViewed(const bool& viewed)
{
    d->viewed = viewed;
}

///// DriveFile::IndexableText

DriveFile::IndexableText::IndexableText()
{ }

DriveFile::IndexableText::IndexableText(const IndexableText& other):
    d(new DriveFilePrivate::IndexableText(*(other.d)))
{ }

DriveFile::IndexableText::~IndexableText()
{
    delete d;
}

QString DriveFile::IndexableText::text()
{
    return d->text;
}

void DriveFile::IndexableText::setText(const QString& text)
{
    d->text = text;
}

///// DriveFile::ImageMediaMetadata::Location

DriveFile::ImageMediaMetadata::Location::Location()
{ }

DriveFile::ImageMediaMetadata::Location::Location(const Location& other):
    d(new DriveFilePrivate::ImageMediaMetadata::Location(*(other.d)))
{ }

DriveFile::ImageMediaMetadata::Location::~Location()
{
    delete d;
}

qreal DriveFile::ImageMediaMetadata::Location::latitude()
{
    return d->latitude;
}

void DriveFile::ImageMediaMetadata::Location::setLatitude(const qreal& latitude)
{
    d->latitude = latitude;
}

qreal DriveFile::ImageMediaMetadata::Location::longitude()
{
    return d->longitude;
}

void DriveFile::ImageMediaMetadata::Location::setLongitude(const qreal& longitude)
{
    d->longitude = longitude;
}

qreal DriveFile::ImageMediaMetadata::Location::altitude()
{
    return d->altitude;
}

void DriveFile::ImageMediaMetadata::Location::setAltitude(const qreal& altitude)
{
    d->altitude = altitude;
}

///// DriveFile::ImageMediaMetadata

DriveFile::ImageMediaMetadata::ImageMediaMetadata()
{ }

DriveFile::ImageMediaMetadata::ImageMediaMetadata(const ImageMediaMetadata& other):
    d(new DriveFilePrivate::ImageMediaMetadata(*(other.d)))
{ }

DriveFile::ImageMediaMetadata::~ImageMediaMetadata()
{
    delete d;
}

int DriveFile::ImageMediaMetadata::width()
{
    return d->width;
}

void DriveFile::ImageMediaMetadata::setWidth(const int& width)
{
    d->width = width;
}

int DriveFile::ImageMediaMetadata::height()
{
    return d->height;
}

void DriveFile::ImageMediaMetadata::setHeight(const int& height)
{
    d->height = height;
}

int DriveFile::ImageMediaMetadata::rotation()
{
    return d->rotation;
}

void DriveFile::ImageMediaMetadata::setRotation(const int& rotation)
{
    d->rotation = rotation;
}

DriveFile::ImageMediaMetadata::Location DriveFile::ImageMediaMetadata::location()
{
    return d->location;
}

void DriveFile::ImageMediaMetadata::setLocation(const DriveFile::ImageMediaMetadata::Location& location)
{
    d->location = location;
}

///// DriveFile

DriveFile::DriveFile()
{ }

DriveFile::DriveFile(const DriveFile& other):
    d(new DriveFilePrivate(*(other.d)))
{ }

DriveFile::~DriveFile()
{
    delete d;
}

QString DriveFile::id()
{
    return d->id;
}

void DriveFile::setId(const QString& id)
{
    d->id = id;
}

QString DriveFile::selfLink()
{
    return d->selfLink;
}

void DriveFile::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QString DriveFile::title()
{
    return d->title;
}

void DriveFile::setTitle(const QString& title)
{
    d->title = title;
}

QString DriveFile::mimeType()
{
    return d->mimeType;
}

void DriveFile::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

QString DriveFile::description()
{
    return d->description;
}

void DriveFile::setDescription(const QString& description)
{
    d->description = description;
}

DriveFile::Labels DriveFile::labels()
{
    return d->labels;
}

void DriveFile::setLabels(const DriveFile::Labels& labels)
{
    d->labels = labels;
}

QDateTime DriveFile::createdDate()
{
    return d->createdDate;
}

void DriveFile::setCreatedDate(const QDateTime& createdDate)
{
    d->createdDate = createdDate;
}

QDateTime DriveFile::modifiedDate()
{
    return d->modifiedDate;
}

void DriveFile::setModifiedDate(const QDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

QDateTime DriveFile::modifiedByMeDate()
{
    return d->modifiedByMeDate;
}

void DriveFile::setModifiedByMeDate(const QDateTime& modifiedByMeDate)
{
    d->modifiedByMeDate = modifiedByMeDate;
}

QString DriveFile::downloadUrl()
{
    return d->downloadUrl;
}

void DriveFile::setDownloadUrl(const QString& downloadUrl)
{
    d->downloadUrl = downloadUrl;
}

DriveFile::IndexableText DriveFile::indexableText()
{
    return d->indexableText;
}

void DriveFile::setIndexableText(const DriveFile::IndexableText& indexableText)
{
    d->indexableText = indexableText;
}

DrivePermission::Ptr DriveFile::userPermission()
{
    return d->userPermission;
}

void DriveFile::setUserPermission(const DrivePermission::Ptr& userPermission)
{
    d->userPermission = userPermission;
}

QString DriveFile::fileExtension()
{
    return d->fileExtension;
}

void DriveFile::setFileExtension(const QString& fileExtension)
{
    d->fileExtension = fileExtension;
}

QString DriveFile::md5Checksum()
{
    return d->md5Checksum;
}

void DriveFile::setMd5Checksum(const QString& md5Checksum)
{
    d->md5Checksum = md5Checksum;
}

qulonglong DriveFile::fileSize()
{
    return d->fileSize;
}

void DriveFile::setFileSize(const qulonglong& fileSize)
{
    d->fileSize = fileSize;
}

QString DriveFile::alternateLink()
{
    return d->alternateLink;
}

void DriveFile::setAlternateLink(const QString& alternateLink)
{
    d->alternateLink = alternateLink;
}

QString DriveFile::embedLink()
{
    return d->embedLink;
}

void DriveFile::setEmbedLink(const QString& embedLink)
{
    d->embedLink = embedLink;
}

QDateTime DriveFile::sharedWithMeDate()
{
    return d->sharedWithMeDate;
}

void DriveFile::setSharedWithMeDate(const QDateTime& sharedWithMeDate)
{
    d->sharedWithMeDate = sharedWithMeDate;
}

QList<DriveParentReference> DriveFile::parents()
{
    return d->parents;
}

void DriveFile::setParents(const QList<DriveParentReference>& parents)
{
    d->parents = parents;
}

QMap<QString, QString> DriveFile::exportLinks()
{
    return d->exportLinks;
}

void DriveFile::setExportLinks(const QMap<QString, QString>& exportLinks)
{
    d->exportLinks = exportLinks;
}

QString DriveFile::originalFileName()
{
    return d->originalFileName;
}

void DriveFile::setOriginalFileName(const QString& originalFileName)
{
    d->originalFileName = originalFileName;
}

qulonglong DriveFile::quotaBytesUsed()
{
    return d->quotaBytesUsed;
}

void DriveFile::setQuotaBytesUsed(const qulonglong& quotaBytesUsed)
{
    d->quotaBytesUsed = quotaBytesUsed;
}

QStringList DriveFile::ownerNames()
{
    return d->ownerNames;
}

void DriveFile::setOwnerNames(const QStringList& ownerNames)
{
    d->ownerNames = ownerNames;
}

QString DriveFile::lastModifyingUserName()
{
    return d->lastModifyingUserName;
}

void DriveFile::setLastModifyingUserName(const QString& lastModifyingUserName)
{
    d->lastModifyingUserName = lastModifyingUserName;
}

bool DriveFile::editable()
{
    return d->editable;
}

void DriveFile::setEditable(const bool& editable)
{
    d->editable = editable;
}

bool DriveFile::writersCanShare()
{
    return d->writersCanShare;
}

void DriveFile::setWritersCanShare(const bool& writersCanShare)
{
    d->writersCanShare = writersCanShare;
}

QString DriveFile::thumbnailLink()
{
    return d->thumbnailLink;
}

void DriveFile::setThumbnailLink(const QString& thumbnailLink)
{
    d->thumbnailLink = thumbnailLink;
}

QDateTime DriveFile::lastViewedByMeDate()
{
    return d->lastViewedByMeDate;
}

void DriveFile::setLastViewedByMeDate(const QDateTime& lastViewedByMeDate)
{
    d->lastViewedByMeDate = lastViewedByMeDate;
}

QString DriveFile::webContentLink()
{
    return d->webContentLink;
}

void DriveFile::setWebContentLink(const QString& webContentLink)
{
    d->webContentLink = webContentLink;
}

bool DriveFile::explicitlyTrashed()
{
    return d->explicitlyTrashed;
}

void DriveFile::setExplicitlyTrashed(const bool& explicitlyTrashed)
{
    d->explicitlyTrashed = explicitlyTrashed;
}

DriveFile::ImageMediaMetadata DriveFile::imageMediaMetadata()
{
    return d->imageMediaMetadata;
}

void DriveFile::setImageMediaMetadata(const DriveFile::ImageMediaMetadata& imageMediaMetadata)
{
    d->imageMediaMetadata = imageMediaMetadata;
}
