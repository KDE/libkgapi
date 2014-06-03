/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>
    Copyright 2013  Daniel Vrátil <dvratil@redhat.com>

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
#include "permission_p.h"
#include "parentreference_p.h"
#include "user.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

///// DriveFile::Labels

class File::Labels::Private
{
  public:
    Private();
    Private(const Private &other);

    bool starred;
    bool hidden;
    bool trashed;
    bool restricted;
    bool viewed;
};

File::Labels::Private::Private():
    starred(false),
    hidden(false),
    trashed(false),
    restricted(false),
    viewed(false)
{
}

File::Labels::Private::Private(const Private &other):
    starred(other.starred),
    hidden(other.hidden),
    trashed(other.trashed),
    restricted(other.restricted),
    viewed(other.viewed)
{
}

File::Labels::Labels():
    d(new Private)
{
}

File::Labels::Labels(const Labels& other):
    d(new Private(*(other.d)))
{ }

File::Labels::~Labels()
{
    delete d;
}

bool File::Labels::starred() const
{
    return d->starred;
}

void File::Labels::setStarred(bool starred)
{
    d->starred = starred;
}

bool File::Labels::hidden() const
{
    return d->hidden;
}

void File::Labels::setHidden(bool hidden)
{
    d->hidden = hidden;
}

bool File::Labels::trashed() const
{
    return d->trashed;
}

void File::Labels::setTrashed(bool trashed)
{
    d->trashed = trashed;
}

bool File::Labels::restricted() const
{
    return d->restricted;
}

void File::Labels::setRestricted(bool restricted)
{
    d->restricted = restricted;
}

bool File::Labels::viewed() const
{
    return d->viewed;
}

void File::Labels::setViewed(bool viewed)
{
    d->viewed = viewed;
}

///// DriveFile::ImageMediaMetadata

class File::IndexableText::Private
{
  public:
    Private();
    Private(const Private &other);

    QString text;
};

File::IndexableText::Private::Private()
{
}

File::IndexableText::Private::Private(const Private &other):
    text(other.text)
{
}

File::IndexableText::IndexableText():
    d(new Private)
{
}

File::IndexableText::IndexableText(const IndexableText& other):
    d(new Private(*(other.d)))
{
}

File::IndexableText::~IndexableText()
{
    delete d;
}

QString File::IndexableText::text() const
{
    return d->text;
}

void File::IndexableText::setText(const QString &text)
{
    d->text = text;
}

///// DriveFile::ImageMediaMetadata::Location

class File::ImageMediaMetadata::Location::Private
{
  public:
    Private();
    Private(const Private &other);

    qreal latitude;
    qreal longitude;
    qreal altitude;
};

File::ImageMediaMetadata::Location::Private::Private():
    latitude(-1),
    longitude(-1),
    altitude(0)
{
}

File::ImageMediaMetadata::Location::Private::Private(const Private &other):
    latitude(other.latitude),
    longitude(other.longitude),
    altitude(other.altitude)
{
}

File::ImageMediaMetadata::Location::Location():
    d(new Private)
{
}

File::ImageMediaMetadata::Location::Location(const Location& other):
    d(new Private(*(other.d)))
{
}

File::ImageMediaMetadata::Location::~Location()
{
    delete d;
}

qreal File::ImageMediaMetadata::Location::latitude() const
{
    return d->latitude;
}

qreal File::ImageMediaMetadata::Location::longitude() const
{
    return d->longitude;
}

qreal File::ImageMediaMetadata::Location::altitude() const
{
    return d->altitude;
}

///// DriveFile::ImageMediaMetadata

class File::ImageMediaMetadata::Private
{
  public:
    Private();
    Private(const Private &other);

    int width;
    int height;
    int rotation;
    LocationPtr location;
    QString date;
    QString cameraMake;
    QString cameraModel;
    float exposureTime;
    float aperture;
    bool flashUsed;
    float focalLength;
    int isoSpeed;
    QString meteringMode;
    QString sensor;
    QString exposureMode;
    QString colorSpace;
    QString whiteBalance;
    float exposureBias;
    float maxApertureValue;
    int subjectDistance;
    QString lens;
};

File::ImageMediaMetadata::Private::Private():
    width(-1),
    height(-1),
    rotation(-1),
    exposureTime(-1),
    aperture(-1),
    flashUsed(false),
    focalLength(-1),
    isoSpeed(-1),
    exposureBias(-1),
    maxApertureValue(-1),
    subjectDistance(-1)
{
}

File::ImageMediaMetadata::Private::Private(const Private &other):
    width(other.width),
    height(other.height),
    rotation(other.rotation),
    location(other.location),
    date(other.date),
    cameraMake(other.cameraMake),
    cameraModel(other.cameraModel),
    exposureTime(other.exposureTime),
    aperture(other.aperture),
    flashUsed(other.flashUsed),
    focalLength(other.focalLength),
    isoSpeed(other.isoSpeed),
    meteringMode(other.meteringMode),
    sensor(other.sensor),
    exposureMode(other.exposureMode),
    colorSpace(other.colorSpace),
    whiteBalance(other.whiteBalance),
    exposureBias(other.exposureBias),
    maxApertureValue(other.maxApertureValue),
    subjectDistance(other.subjectDistance),
    lens(other.lens)
{
}

File::ImageMediaMetadata::ImageMediaMetadata(const QVariantMap &map):
    d(new Private)
{
    d->width = map[QLatin1String("width")].toInt();
    d->height = map[QLatin1String("height")].toInt();
    d->rotation = map[QLatin1String("rotation")].toInt();
    d->date = map[QLatin1String("date")].toString();
    d->cameraMake = map[QLatin1String("cameraMake")].toString();
    d->cameraModel = map[QLatin1String("cameraModel")].toString();
    d->exposureTime = map[QLatin1String("exposureTime")].toFloat();
    d->aperture = map[QLatin1String("aperture")].toFloat();
    d->flashUsed = map[QLatin1String("flashUsed")].toBool();
    d->focalLength = map[QLatin1String("focalLength")].toFloat();
    d->isoSpeed = map[QLatin1String("isoSpeed")].toInt();
    d->meteringMode = map[QLatin1String("meteringMode")].toString();
    d->sensor = map[QLatin1String("sensor")].toString();
    d->exposureMode = map[QLatin1String("exposureMode")].toString();
    d->colorSpace = map[QLatin1String("colorSpace")].toString();
    d->whiteBalance = map[QLatin1String("whiteBalance")].toString();
    d->exposureBias = map[QLatin1String("exposureBias")].toFloat();
    d->maxApertureValue = map[QLatin1String("maxApertureValue")].toFloat();
    d->subjectDistance = map[QLatin1String("subjectDistance")].toFloat();
    d->lens = map[QLatin1String("lens")].toString();

    const QVariantMap locationData = map[QLatin1String("location")].toMap();
    File::ImageMediaMetadata::LocationPtr location(new File::ImageMediaMetadata::Location);
    location->d->latitude = locationData[QLatin1String("latitude")].toReal();
    location->d->longitude = locationData[QLatin1String("longitude")].toReal();
    location->d->altitude = locationData[QLatin1String("altitude")].toReal();
}

File::ImageMediaMetadata::ImageMediaMetadata(const ImageMediaMetadata& other):
    d(new Private(*(other.d)))
{
}

File::ImageMediaMetadata::~ImageMediaMetadata()
{
    delete d;
}

int File::ImageMediaMetadata::width() const
{
    return d->width;
}

int File::ImageMediaMetadata::height() const
{
    return d->height;
}

int File::ImageMediaMetadata::rotation() const
{
    return d->rotation;
}

File::ImageMediaMetadata::LocationPtr File::ImageMediaMetadata::location() const
{
    return d->location;
}

QString File::ImageMediaMetadata::date() const
{
    return d->date;
}

QString File::ImageMediaMetadata::cameraMake() const
{
    return d->cameraMake;
}

QString File::ImageMediaMetadata::cameraModel() const
{
    return d->cameraModel;
}

float File::ImageMediaMetadata::exposureTime() const
{
    return d->exposureTime;
}

float File::ImageMediaMetadata::aperture() const
{
    return d->aperture;
}

bool File::ImageMediaMetadata::flashUsed() const
{
    return d->flashUsed;
}

float File::ImageMediaMetadata::focalLength() const
{
    return d->focalLength;
}

int File::ImageMediaMetadata::isoSpeed() const
{
    return d->isoSpeed;
}

QString File::ImageMediaMetadata::meteringMode() const
{
    return d->meteringMode;
}

QString File::ImageMediaMetadata::sensor() const
{
    return d->sensor;
}

QString File::ImageMediaMetadata::exposureMode() const
{
    return d->exposureMode;
}

QString File::ImageMediaMetadata::colorSpace() const
{
    return d->colorSpace;
}

QString File::ImageMediaMetadata::whiteBalance() const
{
    return d->whiteBalance;
}

float File::ImageMediaMetadata::exposureBias() const
{
    return d->exposureBias;
}

float File::ImageMediaMetadata::maxApertureValue() const
{
    return d->maxApertureValue;
}

int File::ImageMediaMetadata::subjectDistance() const
{
    return d->subjectDistance;
}

QString File::ImageMediaMetadata::lens() const
{
    return d->lens;
}


////// DriveFile::Thumbnail

class File::Thumbnail::Private
{
  public:
    Private();
    Private(const Private &other);

    QImage image;
    QString mimeType;
};

File::Thumbnail::Private::Private()
{
}

File::Thumbnail::Private::Private(const Private &other):
    image(other.image),
    mimeType(other.mimeType)
{
}

File::Thumbnail::Thumbnail(const QVariantMap &map):
    d(new Private)
{
    const QByteArray ba = QByteArray::fromBase64(map[QLatin1String("image")].toByteArray());
    d->image = QImage::fromData(ba);
    d->mimeType = map[QLatin1String("mimeType")].toString();
}

File::Thumbnail::Thumbnail(const File::Thumbnail &other):
    d(new Private(*(other.d)))
{
}

File::Thumbnail::~Thumbnail()
{
    delete d;
}

QImage File::Thumbnail::image() const
{
    return d->image;
}

QString File::Thumbnail::mimeType() const
{
    return d->mimeType;
}

////// DriveFile

File::Private::Private():
    fileSize(-1),
    quotaBytesUsed(-1),
    editable(false),
    writersCanShare(false),
    explicitlyTrashed(false),
    shared(false)
{
}

File::Private::Private(const Private& other):
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
    imageMediaMetadata(other.imageMediaMetadata),
    thumbnail(other.thumbnail),
    webViewLink(other.webViewLink),
    iconLink(other.iconLink),
    shared(other.shared),
    owners(other.owners),
    lastModifyingUser(other.lastModifyingUser)
{
}

FilePtr File::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#file"))
    {
        return FilePtr();
    }

    FilePtr file(new File());
    file->setEtag(map[QLatin1String("etag")].toString());
    file->d->id = map[QLatin1String("id")].toString();
    file->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    file->d->title = map[QLatin1String("title")].toString();
    file->d->mimeType = map[QLatin1String("mimeType")].toString();
    file->d->description = map[QLatin1String("description")].toString();

    const QVariantMap labelsData =  map[QLatin1String("labels")].toMap();
    File::LabelsPtr labels(new File::Labels());
    labels->d->starred = labelsData[QLatin1String("starred")].toBool();
    labels->d->hidden = labelsData[QLatin1String("hidden")].toBool();
    labels->d->trashed = labelsData[QLatin1String("trashed")].toBool();
    labels->d->restricted = labelsData[QLatin1String("restricted")].toBool();
    labels->d->viewed = labelsData[QLatin1String("viewed")].toBool();
    file->d->labels = labels;

    // FIXME FIXME FIXME Verify the date format
    file->d->createdDate = KDateTime::fromString(map[QLatin1String("createdDate")].toString(), KDateTime::RFC3339Date);
    file->d->modifiedDate = KDateTime::fromString(map[QLatin1String("modifiedDate")].toString(), KDateTime::RFC3339Date);
    file->d->modifiedByMeDate = KDateTime::fromString(map[QLatin1String("modifiedByMeDate")].toString(), KDateTime::RFC3339Date);
    file->d->downloadUrl = map[QLatin1String("downloadUrl")].toUrl();

    const QVariantMap indexableTextData = map[QLatin1String("indexableText")].toMap();
    File::IndexableTextPtr indexableText(new File::IndexableText());
    indexableText->d->text = indexableTextData[QLatin1String("text")].toString();
    file->d->indexableText = indexableText;

    const QVariantMap userPermissionData = map[QLatin1String("userPermission")].toMap();
    file->d->userPermission = Permission::Private::fromJSON(userPermissionData);

    file->d->fileExtension = map[QLatin1String("fileExtension")].toString();
    file->d->md5Checksum = map[QLatin1String("md5Checksum")].toString();
    file->d->fileSize = map[QLatin1String("fileSize")].toLongLong();
    file->d->alternateLink = map[QLatin1String("alternateLink")].toUrl();
    file->d->embedLink = map[QLatin1String("embedLink")].toUrl();
    file->d->sharedWithMeDate = KDateTime::fromString(map[QLatin1String("sharedWithMeDate")].toString(), KDateTime::RFC3339Date);

    const QVariantList parents = map[QLatin1String("parents")].toList();
    Q_FOREACH (const QVariant &parent, parents)
    {
        file->d->parents << ParentReference::Private::fromJSON(parent.toMap());
    }

    const QVariantMap exportLinksData = map[QLatin1String("exportLinks")].toMap();
    QVariantMap::ConstIterator iter = exportLinksData.constBegin();
    for ( ; iter != exportLinksData.constEnd(); ++iter) {
        file->d->exportLinks.insert(iter.key(), iter.value().toUrl());
    }

    file->d->originalFileName = map[QLatin1String("originalFileName")].toString();
    file->d->quotaBytesUsed = map[QLatin1String("quotaBytesUsed")].toLongLong();
    file->d->ownerNames = map[QLatin1String("ownerNames")].toStringList();
    file->d->lastModifyingUserName = map[QLatin1String("lastModifyingUserName")].toString();
    file->d->editable = map[QLatin1String("editable")].toBool();
    file->d->writersCanShare = map[QLatin1String("writersCanShare")].toBool();
    file->d->thumbnailLink = map[QLatin1String("thumbnailLink")].toUrl();
    file->d->lastViewedByMeDate = KDateTime::fromString(map[QLatin1String("lastViewedByMeDate")].toString(), KDateTime::RFC3339Date);
    file->d->webContentLink = map[QLatin1String("webContentLink")].toUrl();
    file->d->explicitlyTrashed = map[QLatin1String("explicitlyTrashed")].toBool();

    const QVariantMap imageMetaData = map[QLatin1String("imageMediaMetadata")].toMap();
    file->d->imageMediaMetadata =
        File::ImageMediaMetadataPtr(new File::ImageMediaMetadata(imageMetaData));

    const QVariantMap thumbnailData = map[QLatin1String("thumbnail")].toMap();
    File::ThumbnailPtr thumbnail(new File::Thumbnail(thumbnailData));
    file->d->thumbnail = thumbnail;

    file->d->webViewLink = map[QLatin1String("webViewLink")].toUrl();
    file->d->iconLink = map[QLatin1String("iconLink")].toUrl();
    file->d->shared = map[QLatin1String("shared")].toBool();

    const QVariantList ownersList = map[QLatin1String("owners")].toList();
    Q_FOREACH (const QVariant &owner, ownersList) {
        file->d->owners << User::fromJSON(owner.toMap());
    }

    const QVariantMap lastModifyingUser = map[QLatin1String("lastModifyingUser")].toMap();
    file->d->lastModifyingUser = User::fromJSON(lastModifyingUser);

    return file;
}

File::File():
    KGAPI2::Object(),
    d(new Private)
{
}

File::File(const File& other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{ }

File::~File()
{
    delete d;
}

QString File::folderMimeType()
{
    return QLatin1String("application/vnd.google-apps.folder");
}

QString File::id() const
{
    return d->id;
}

QUrl File::selfLink() const
{
    return d->selfLink;
}

QString File::title() const
{
    return d->title;
}

void File::setTitle(const QString& title)
{
    d->title = title;
}

QString File::mimeType() const
{
    return d->mimeType;
}

void File::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

QString File::description() const
{
    return d->description;
}

void File::setDescription(const QString& description)
{
    d->description = description;
}

File::LabelsPtr File::labels() const
{
    return d->labels;
}

void File::setLabels(const File::LabelsPtr &labels)
{
    d->labels = labels;
}

KDateTime File::createdDate() const
{
    return d->createdDate;
}

KDateTime File::modifiedDate() const
{
    return d->modifiedDate;
}

void File::setModifiedDate(const KDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

KDateTime File::modifiedByMeDate() const
{
    return d->modifiedByMeDate;
}

QUrl File::downloadUrl() const
{
    return d->downloadUrl;
}

File::IndexableTextPtr& File::indexableText()
{
    return d->indexableText;
}

PermissionPtr File::userPermission() const
{
    return d->userPermission;
}

QString File::fileExtension() const
{
    return d->fileExtension;
}

QString File::md5Checksum() const
{
    return d->md5Checksum;
}

qlonglong File::fileSize() const
{
    return d->fileSize;
}

QUrl File::alternateLink() const
{
    return d->alternateLink;
}

QUrl File::embedLink() const
{
    return d->embedLink;
}

KDateTime File::sharedWithMeDate() const
{
    return d->sharedWithMeDate;
}

ParentReferencesList File::parents() const
{
    return d->parents;
}

void File::setParents(const ParentReferencesList &parents)
{
    d->parents = parents;
}

QMap< QString, QUrl > File::exportLinks() const
{
    return d->exportLinks;
}

QString File::originalFileName() const
{
    return d->originalFileName;
}

qlonglong File::quotaBytesUsed() const
{
    return d->quotaBytesUsed;
}

QStringList File::ownerNames() const
{
    return d->ownerNames;
}

QString File::lastModifyingUserName() const
{
    return d->lastModifyingUserName;
}

bool File::editable() const
{
    return d->editable;
}

bool File::writersCanShare() const
{
    return d->writersCanShare;
}

QUrl File::thumbnailLink() const
{
    return d->thumbnailLink;
}

KDateTime File::lastViewedByMeDate() const
{
    return d->lastViewedByMeDate;
}

void File::setLastViewedByMeDate(const KDateTime& lastViewedByMeDate)
{
    d->lastViewedByMeDate = lastViewedByMeDate;
}

QUrl File::webContentLink() const
{
    return d->webContentLink;
}

bool File::explicitlyTrashed() const
{
    return d->explicitlyTrashed;
}

File::ImageMediaMetadataPtr File::imageMediaMetadata() const
{
    return d->imageMediaMetadata;
}

File::ThumbnailPtr File::thumbnail() const
{
    return d->thumbnail;
}

QUrl File::webViewLink() const
{
    return d->webViewLink;
}

QUrl File::iconLink() const
{
    return d->iconLink;
}

bool File::shared() const
{
    return d->shared;
}

UsersList File::owners() const
{
    return d->owners;
}

UserPtr File::lastModifyingUser() const
{
    return d->lastModifyingUser;
}

bool File::isFolder() const
{
    return (d->mimeType == File::folderMimeType());
}

FilePtr File::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return FilePtr();
    }
    return Private::fromJSON(data.toMap());
}

FilePtr File::fromJSON(const QVariantMap &jsonData)
{
    if (jsonData.isEmpty()) {
        return FilePtr();
    }
    return Private::fromJSON(jsonData);
}


FilesList File::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return FilesList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#fileList"))
    {
        return FilesList();
    }

    FilesList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const FilePtr file = Private::fromJSON(item.toMap());

        if (!file.isNull()) {
            list << file;
        }
    }

    if (map.contains(QLatin1String("nextLink"))) {
        feedData.nextPageUrl = map[QLatin1String("nextLink")].toUrl();
    }

    return list;
}

QByteArray File::toJSON(const FilePtr &file)
{
    QVariantMap map;

    map[QLatin1String("kind")] = QLatin1String("drive#file");
    map[QLatin1String("description")] = file->description();

    if (file->indexableText()) {
        QVariantMap indexableText;
        indexableText[QLatin1String("text")] = file->indexableText()->text();
        map[QLatin1String("indexableText")] = indexableText;
    }

    if (file->labels()) {
        QVariantMap labels;
        labels[QLatin1String("hidden")] = file->labels()->hidden();
        labels[QLatin1String("restricted")] = file->labels()->restricted();
        labels[QLatin1String("starred")] = file->labels()->starred();
        labels[QLatin1String("trashed")] = file->labels()->trashed();
        labels[QLatin1String("viewed")] = file->labels()->viewed();
        map[QLatin1String("labels")] = labels;
    }

    if (file->lastViewedByMeDate().isValid()) {
        map[QLatin1String("lastViewedByMeDate")] = file->lastViewedByMeDate().toString(KDateTime::RFC3339Date);
    }

    map[QLatin1String("mimeType")] = file->mimeType();

    if (file->modifiedDate().isValid()) {
        map[QLatin1String("modifiedDate")] = file->modifiedDate().toString(KDateTime::RFC3339Date);
    }
    if (file->createdDate().isValid()) {
        map[QLatin1String("createdDate")] = file->createdDate().toString(KDateTime::RFC3339Date);
    }
    if (file->modifiedByMeDate().isValid()) {
        map[QLatin1String("modifiedByMeDate")] = file->modifiedByMeDate().toString(KDateTime::RFC3339Date);
    }

    map[QLatin1String("fileSize")] = file->d->fileSize;

    map[QLatin1String("title")] = file->title();

    QVariantList parents;
    Q_FOREACH (const ParentReferencePtr &parent, file->parents()) {
        parents << ParentReference::Private::toJSON(parent);
    }
    map[QLatin1String("parents")] = parents;
    map[QLatin1String("etag")] = file->etag();
    map[QLatin1String("id")] = file->d->id;
    map[QLatin1String("selfLink")] = file->d->selfLink;
    map[QLatin1String("downloadUrl")] = file->d->downloadUrl;

    map[QLatin1String("fileExtension")] = file->d->fileExtension;
    map[QLatin1String("md5Checksum")] = file->d->md5Checksum ;
    map[QLatin1String("alternateLink")] = file->d->alternateLink;
    map[QLatin1String("embedLink")] = file->d->embedLink;
    map[QLatin1String("sharedWithMeDate")] = file->d->sharedWithMeDate.toString(KDateTime::RFC3339Date);


    map[QLatin1String("originalFileName")] = file->d->originalFileName;
    map[QLatin1String("quotaBytesUsed")] = file->d->quotaBytesUsed;
    map[QLatin1String("ownerNames")] = QVariant(file->d->ownerNames);
    map[QLatin1String("lastModifyingUserName")] = file->d->lastModifyingUserName;
    map[QLatin1String("editable")] = file->d->editable;
    map[QLatin1String("writersCanShare")] = file->d->writersCanShare;
    map[QLatin1String("thumbnailLink")] = file->d->thumbnailLink;
    map[QLatin1String("lastViewedByMeDate")] = file->d->lastViewedByMeDate.toString(KDateTime::RFC3339Date);
    map[QLatin1String("webContentLink")] = file->d->webContentLink;
    map[QLatin1String("explicitlyTrashed")] = file->d->explicitlyTrashed;

    map[QLatin1String("webViewLink")] = file->d->webViewLink;
    map[QLatin1String("iconLink")] = file->d->iconLink;
    map[QLatin1String("shared")] = file->d->shared;

#if 0
    const QVariantMap userPermissionData = map[QLatin1String("userPermission")].toMap();
    file->d->userPermission = Permission::Private::fromJSON(userPermissionData);

    const QVariantList parents = map[QLatin1String("parents")].toList();
    Q_FOREACH (const QVariant &parent, parents)
    {
        file->d->parents << ParentReference::Private::fromJSON(parent.toMap());
    }

    const QVariantMap exportLinksData = map[QLatin1String("exportLinks")].toMap();
    QVariantMap::ConstIterator iter = exportLinksData.constBegin();
    for ( ; iter != exportLinksData.constEnd(); ++iter) {
        file->d->exportLinks.insert(iter.key(), iter.value().toUrl());
    }


    const QVariantMap imageMetaData = map[QLatin1String("imageMediaMetadata")].toMap();
    file->d->imageMediaMetadata =
        File::ImageMediaMetadataPtr(new File::ImageMediaMetadata(imageMetaData));

    const QVariantMap thumbnailData = map[QLatin1String("thumbnail")].toMap();
    File::ThumbnailPtr thumbnail(new File::Thumbnail(thumbnailData));
    file->d->thumbnail = thumbnail;


    const QVariantList ownersList = map[QLatin1String("owners")].toList();
    Q_FOREACH (const QVariant &owner, ownersList) {
        file->d->owners << User::fromJSON(owner.toMap());
    }

    const QVariantMap lastModifyingUser = map[QLatin1String("lastModifyingUser")].toMap();
    file->d->lastModifyingUser = User::fromJSON(lastModifyingUser);

#endif


    QJson::Serializer serializer;
    return serializer.serialize(map);
}






