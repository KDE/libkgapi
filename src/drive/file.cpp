/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>
    Copyright 2013  Daniel Vrátil <dvratil@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "file.h"
#include "file_p.h"
#include "permission_p.h"
#include "parentreference_p.h"
#include "user.h"

#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

///// DriveFile::Labels

class Q_DECL_HIDDEN File::Labels::Private
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

class Q_DECL_HIDDEN File::IndexableText::Private
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

class Q_DECL_HIDDEN File::ImageMediaMetadata::Location::Private
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

class Q_DECL_HIDDEN File::ImageMediaMetadata::Private
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
    d->width = map[QStringLiteral("width")].toInt();
    d->height = map[QStringLiteral("height")].toInt();
    d->rotation = map[QStringLiteral("rotation")].toInt();
    d->date = map[QStringLiteral("date")].toString();
    d->cameraMake = map[QStringLiteral("cameraMake")].toString();
    d->cameraModel = map[QStringLiteral("cameraModel")].toString();
    d->exposureTime = map[QStringLiteral("exposureTime")].toFloat();
    d->aperture = map[QStringLiteral("aperture")].toFloat();
    d->flashUsed = map[QStringLiteral("flashUsed")].toBool();
    d->focalLength = map[QStringLiteral("focalLength")].toFloat();
    d->isoSpeed = map[QStringLiteral("isoSpeed")].toInt();
    d->meteringMode = map[QStringLiteral("meteringMode")].toString();
    d->sensor = map[QStringLiteral("sensor")].toString();
    d->exposureMode = map[QStringLiteral("exposureMode")].toString();
    d->colorSpace = map[QStringLiteral("colorSpace")].toString();
    d->whiteBalance = map[QStringLiteral("whiteBalance")].toString();
    d->exposureBias = map[QStringLiteral("exposureBias")].toFloat();
    d->maxApertureValue = map[QStringLiteral("maxApertureValue")].toFloat();
    d->subjectDistance = map[QStringLiteral("subjectDistance")].toFloat();
    d->lens = map[QStringLiteral("lens")].toString();

    const QVariantMap locationData = map[QStringLiteral("location")].toMap();
    File::ImageMediaMetadata::LocationPtr location(new File::ImageMediaMetadata::Location);
    location->d->latitude = locationData[QStringLiteral("latitude")].toReal();
    location->d->longitude = locationData[QStringLiteral("longitude")].toReal();
    location->d->altitude = locationData[QStringLiteral("altitude")].toReal();
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

class Q_DECL_HIDDEN File::Thumbnail::Private
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
    const QByteArray ba = QByteArray::fromBase64(map[QStringLiteral("image")].toByteArray());
    d->image = QImage::fromData(ba);
    d->mimeType = map[QStringLiteral("mimeType")].toString();
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
    if (!map.contains(QStringLiteral("kind")) ||
        map[QStringLiteral("kind")].toString() != QLatin1String("drive#file"))
    {
        return FilePtr();
    }

    FilePtr file(new File());
    file->setEtag(map[QStringLiteral("etag")].toString());
    file->d->id = map[QStringLiteral("id")].toString();
    file->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    file->d->title = map[QStringLiteral("title")].toString();
    file->d->mimeType = map[QStringLiteral("mimeType")].toString();
    file->d->description = map[QStringLiteral("description")].toString();

    const QVariantMap labelsData =  map[QStringLiteral("labels")].toMap();
    File::LabelsPtr labels(new File::Labels());
    labels->d->starred = labelsData[QStringLiteral("starred")].toBool();
    labels->d->hidden = labelsData[QStringLiteral("hidden")].toBool();
    labels->d->trashed = labelsData[QStringLiteral("trashed")].toBool();
    labels->d->restricted = labelsData[QStringLiteral("restricted")].toBool();
    labels->d->viewed = labelsData[QStringLiteral("viewed")].toBool();
    file->d->labels = labels;

    // FIXME FIXME FIXME Verify the date format
    file->d->createdDate = QDateTime::fromString(map[QStringLiteral("createdDate")].toString(), Qt::ISODate);
    file->d->modifiedDate = QDateTime::fromString(map[QStringLiteral("modifiedDate")].toString(), Qt::ISODate);
    file->d->modifiedByMeDate = QDateTime::fromString(map[QStringLiteral("modifiedByMeDate")].toString(), Qt::ISODate);
    file->d->downloadUrl = map[QStringLiteral("downloadUrl")].toUrl();

    const QVariantMap indexableTextData = map[QStringLiteral("indexableText")].toMap();
    File::IndexableTextPtr indexableText(new File::IndexableText());
    indexableText->d->text = indexableTextData[QStringLiteral("text")].toString();
    file->d->indexableText = indexableText;

    const QVariantMap userPermissionData = map[QStringLiteral("userPermission")].toMap();
    file->d->userPermission = Permission::Private::fromJSON(userPermissionData);

    file->d->fileExtension = map[QStringLiteral("fileExtension")].toString();
    file->d->md5Checksum = map[QStringLiteral("md5Checksum")].toString();
    file->d->fileSize = map[QStringLiteral("fileSize")].toLongLong();
    file->d->alternateLink = map[QStringLiteral("alternateLink")].toUrl();
    file->d->embedLink = map[QStringLiteral("embedLink")].toUrl();
    file->d->sharedWithMeDate = QDateTime::fromString(map[QStringLiteral("sharedWithMeDate")].toString(), Qt::ISODate);

    const QVariantList parents = map[QStringLiteral("parents")].toList();
    for (const QVariant &parent : parents)
    {
        file->d->parents << ParentReference::Private::fromJSON(parent.toMap());
    }

    const QVariantMap exportLinksData = map[QStringLiteral("exportLinks")].toMap();
    QVariantMap::ConstIterator iter = exportLinksData.constBegin();
    for ( ; iter != exportLinksData.constEnd(); ++iter) {
        file->d->exportLinks.insert(iter.key(), iter.value().toUrl());
    }

    file->d->originalFileName = map[QStringLiteral("originalFileName")].toString();
    file->d->quotaBytesUsed = map[QStringLiteral("quotaBytesUsed")].toLongLong();
    file->d->ownerNames = map[QStringLiteral("ownerNames")].toStringList();
    file->d->lastModifyingUserName = map[QStringLiteral("lastModifyingUserName")].toString();
    file->d->editable = map[QStringLiteral("editable")].toBool();
    file->d->writersCanShare = map[QStringLiteral("writersCanShare")].toBool();
    file->d->thumbnailLink = map[QStringLiteral("thumbnailLink")].toUrl();
    file->d->lastViewedByMeDate = QDateTime::fromString(map[QStringLiteral("lastViewedByMeDate")].toString(), Qt::ISODate);
    file->d->webContentLink = map[QStringLiteral("webContentLink")].toUrl();
    file->d->explicitlyTrashed = map[QStringLiteral("explicitlyTrashed")].toBool();

    const QVariantMap imageMetaData = map[QStringLiteral("imageMediaMetadata")].toMap();
    file->d->imageMediaMetadata =
        File::ImageMediaMetadataPtr(new File::ImageMediaMetadata(imageMetaData));

    const QVariantMap thumbnailData = map[QStringLiteral("thumbnail")].toMap();
    File::ThumbnailPtr thumbnail(new File::Thumbnail(thumbnailData));
    file->d->thumbnail = thumbnail;

    file->d->webViewLink = map[QStringLiteral("webViewLink")].toUrl();
    file->d->iconLink = map[QStringLiteral("iconLink")].toUrl();
    file->d->shared = map[QStringLiteral("shared")].toBool();

    const QVariantList ownersList = map[QStringLiteral("owners")].toList();
    for (const QVariant &owner : ownersList) {
        file->d->owners << User::fromJSON(owner.toMap());
    }

    const QVariantMap lastModifyingUser = map[QStringLiteral("lastModifyingUser")].toMap();
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
    return QStringLiteral("application/vnd.google-apps.folder");
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

QDateTime File::createdDate() const
{
    return d->createdDate;
}

QDateTime File::modifiedDate() const
{
    return d->modifiedDate;
}

void File::setModifiedDate(const QDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

QDateTime File::modifiedByMeDate() const
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

QDateTime File::sharedWithMeDate() const
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

QDateTime File::lastViewedByMeDate() const
{
    return d->lastViewedByMeDate;
}

void File::setLastViewedByMeDate(const QDateTime& lastViewedByMeDate)
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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return FilePtr();
    }
    const QVariant data = document.toVariant();
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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return FilesList();
    }
    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(QStringLiteral("kind")) ||
        map[QStringLiteral("kind")].toString() != QLatin1String("drive#fileList"))
    {
        return FilesList();
    }

    FilesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : items) {
        const FilePtr file = Private::fromJSON(item.toMap());

        if (!file.isNull()) {
            list << file;
        }
    }

    if (map.contains(QStringLiteral("nextLink"))) {
        feedData.nextPageUrl = map[QStringLiteral("nextLink")].toUrl();
    }

    return list;
}

QByteArray File::toJSON(const FilePtr &file, SerializationOptions options)
{
    QVariantMap map;

    map[QStringLiteral("kind")] = QLatin1String("drive#file");
    if (!file->description().isEmpty()) {
        map[QStringLiteral("description")] = file->description();
    }

    if (file->indexableText() && !file->indexableText()->text().isEmpty()) {
        QVariantMap indexableText;
        indexableText[QStringLiteral("text")] = file->indexableText()->text();
        map[QStringLiteral("indexableText")] = indexableText;
    }

    if (file->labels()) {
        QVariantMap labels;
        labels[QStringLiteral("hidden")] = file->labels()->hidden();
        labels[QStringLiteral("restricted")] = file->labels()->restricted();
        labels[QStringLiteral("starred")] = file->labels()->starred();
        labels[QStringLiteral("trashed")] = file->labels()->trashed();
        labels[QStringLiteral("viewed")] = file->labels()->viewed();
        map[QStringLiteral("labels")] = labels;
    }

    if (file->lastViewedByMeDate().isValid()) {
        map[QStringLiteral("lastViewedByMeDate")] = file->lastViewedByMeDate().toString(Qt::ISODate);
    }

    if (!file->mimeType().isEmpty()) {
        map[QStringLiteral("mimeType")] = file->mimeType();
    }

    if (file->modifiedDate().isValid()) {
        map[QStringLiteral("modifiedDate")] = file->modifiedDate().toString(Qt::ISODate);
    }
    if (file->createdDate().isValid() && !(options & ExcludeCreationDate)) {
        map[QStringLiteral("createdDate")] = file->createdDate().toString(Qt::ISODate);
    }
    if (file->modifiedByMeDate().isValid()) {
        map[QStringLiteral("modifiedByMeDate")] = file->modifiedByMeDate().toString(Qt::ISODate);
    }

    if (file->fileSize() > 0) {
        map[QStringLiteral("fileSize")] = file->fileSize();
    }

    if (!file->title().isEmpty()) {
        map[QStringLiteral("title")] = file->title();
    }

    QVariantList parents;
    const auto parentReferences = file->parents();
    parents.reserve(parentReferences.size());
    for (const ParentReferencePtr &parent : parentReferences) {
        parents << ParentReference::Private::toJSON(parent);
    }
    if (!parents.isEmpty()) {
        map[QStringLiteral("parents")] = parents;
    }
    if (!file->etag().isEmpty()) {
        map[QStringLiteral("etag")] = file->etag();
    }
    if (!file->d->id.isEmpty()) {
        map[QStringLiteral("id")] = file->d->id;
    }
    if (!file->d->selfLink.isEmpty()) {
        map[QStringLiteral("selfLink")] = file->d->selfLink;
    }
    if (!file->d->downloadUrl.isEmpty()) {
        map[QStringLiteral("downloadUrl")] = file->d->downloadUrl;
    }

    if (!file->d->fileExtension.isEmpty()) {
        map[QStringLiteral("fileExtension")] = file->d->fileExtension;
    }
    if (!file->d->md5Checksum.isEmpty()) {
        map[QStringLiteral("md5Checksum")] = file->d->md5Checksum;
    }
    if (!file->d->alternateLink.isEmpty()) {
        map[QStringLiteral("alternateLink")] = file->d->alternateLink;
    }
    if (!file->d->embedLink.isEmpty()) {
        map[QStringLiteral("embedLink")] = file->d->embedLink;
    }
    if (!file->d->sharedWithMeDate.isNull()) {
        map[QStringLiteral("sharedWithMeDate")] = file->d->sharedWithMeDate.toString(Qt::ISODate);
    }


    if (!file->d->originalFileName.isEmpty()) {
        map[QStringLiteral("originalFileName")] = file->d->originalFileName;
    }
    if (file->d->quotaBytesUsed > 0) {
        map[QStringLiteral("quotaBytesUsed")] = file->d->quotaBytesUsed;
    }
    if (!file->d->ownerNames.isEmpty()) {
        map[QStringLiteral("ownerNames")] = QVariant(file->d->ownerNames);
    }
    if (!file->d->lastModifyingUserName.isEmpty()) {
        map[QStringLiteral("lastModifyingUserName")] = file->d->lastModifyingUserName;
    }
    if (!file->d->editable) { // default is true
        map[QStringLiteral("editable")] = file->d->editable;
    }
    if (file->d->writersCanShare) { // default is false
        map[QStringLiteral("writersCanShare")] = file->d->writersCanShare;
    }
    if (!file->d->thumbnailLink.isEmpty()) {
        map[QStringLiteral("thumbnailLink")] = file->d->thumbnailLink;
    }
    if (!file->d->lastViewedByMeDate.isNull()) {
        map[QStringLiteral("lastViewedByMeDate")] = file->d->lastViewedByMeDate.toString(Qt::ISODate);
    }
    if (!file->d->webContentLink.isEmpty()) {
        map[QStringLiteral("webContentLink")] = file->d->webContentLink;
    }
    if (file->d->explicitlyTrashed) {
        map[QStringLiteral("explicitlyTrashed")] = file->d->explicitlyTrashed;
    }

    if (!file->d->webViewLink.isEmpty()) {
        map[QStringLiteral("webViewLink")] = file->d->webViewLink;
    }
    if (!file->d->iconLink.isEmpty()) {
        map[QStringLiteral("iconLink")] = file->d->iconLink;
    }
    if (file->d->shared) {
        map[QStringLiteral("shared")] = file->d->shared;
    }

#if 0
    const QVariantMap userPermissionData = map[QLatin1String("userPermission")].toMap();
    file->d->userPermission = Permission::Private::fromJSON(userPermissionData);

    const QVariantList parents = map[QLatin1String("parents")].toList();
    for (const QVariant &parent : parents)
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
    for (const QVariant &owner : ownersList) {
        file->d->owners << User::fromJSON(owner.toMap());
    }

    const QVariantMap lastModifyingUser = map[QLatin1String("lastModifyingUser")].toMap();
    file->d->lastModifyingUser = User::fromJSON(lastModifyingUser);

#endif

    QJsonDocument document = QJsonDocument::fromVariant(map);
    return document.toJson(QJsonDocument::Compact);
}






