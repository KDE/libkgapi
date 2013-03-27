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

///// DriveFile::Labels

class DriveFile::Labels::Private
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

DriveFile::Labels::Private::Private():
    starred(false),
    hidden(false),
    trashed(false),
    restricted(false),
    viewed(false)
{
}

DriveFile::Labels::Private::Private(const Private &other):
    starred(other.starred),
    hidden(other.hidden),
    trashed(other.trashed),
    restricted(other.restricted),
    viewed(other.viewed)
{
}

DriveFile::Labels::Labels():
    d(new Private)
{
}

DriveFile::Labels::Labels(const Labels& other):
    d(new Private(*(other.d)))
{ }

DriveFile::Labels::~Labels()
{
    delete d;
}

bool DriveFile::Labels::starred() const
{
    return d->starred;
}

void DriveFile::Labels::setStarred(bool starred)
{
    d->starred = starred;
}

bool DriveFile::Labels::hidden() const
{
    return d->hidden;
}

void DriveFile::Labels::setHidden(bool hidden)
{
    d->hidden = hidden;
}

bool DriveFile::Labels::trashed() const
{
    return d->trashed;
}

void DriveFile::Labels::setTrashed(bool trashed)
{
    d->trashed = trashed;
}

bool DriveFile::Labels::restricted() const
{
    return d->restricted;
}

void DriveFile::Labels::setRestricted(bool restricted)
{
    d->restricted = restricted;
}

bool DriveFile::Labels::viewed() const
{
    return d->viewed;
}

void DriveFile::Labels::setViewed(bool viewed)
{
    d->viewed = viewed;
}

///// DriveFile::ImageMediaMetadata

class DriveFile::IndexableText::Private
{
  public:
    Private();
    Private(const Private &other);

    QString text;
};

DriveFile::IndexableText::Private::Private()
{
}

DriveFile::IndexableText::Private::Private(const Private &other):
    text(other.text)
{
}

DriveFile::IndexableText::IndexableText():
    d(new Private)
{
}

DriveFile::IndexableText::IndexableText(const IndexableText& other):
    d(new Private(*(other.d)))
{
}

DriveFile::IndexableText::~IndexableText()
{
    delete d;
}

QString DriveFile::IndexableText::text() const
{
    return d->text;
}

void DriveFile::IndexableText::setText(const QString &text)
{
    d->text = text;
}

///// DriveFile::ImageMediaMetadata::Location

class DriveFile::ImageMediaMetadata::Location::Private
{
  public:
    Private();
    Private(const Private &other);

    qreal latitude;
    qreal longitude;
    qreal altitude;
};

DriveFile::ImageMediaMetadata::Location::Private::Private():
    latitude(-1),
    longitude(-1),
    altitude(0)
{
}

DriveFile::ImageMediaMetadata::Location::Private::Private(const Private &other):
    latitude(other.latitude),
    longitude(other.longitude),
    altitude(other.altitude)
{
}

DriveFile::ImageMediaMetadata::Location::Location():
    d(new Private)
{
}

DriveFile::ImageMediaMetadata::Location::Location(const Location& other):
    d(new Private(*(other.d)))
{
}

DriveFile::ImageMediaMetadata::Location::~Location()
{
    delete d;
}

qreal DriveFile::ImageMediaMetadata::Location::latitude() const
{
    return d->latitude;
}

qreal DriveFile::ImageMediaMetadata::Location::longitude() const
{
    return d->longitude;
}

qreal DriveFile::ImageMediaMetadata::Location::altitude() const
{
    return d->altitude;
}

///// DriveFile::ImageMediaMetadata

class DriveFile::ImageMediaMetadata::Private
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

DriveFile::ImageMediaMetadata::Private::Private():
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

DriveFile::ImageMediaMetadata::Private::Private(const Private &other):
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

DriveFile::ImageMediaMetadata::ImageMediaMetadata(const QVariantMap &map):
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
    DriveFile::ImageMediaMetadata::LocationPtr location(new DriveFile::ImageMediaMetadata::Location);
    location->d->latitude = locationData[QLatin1String("latitude")].toReal();
    location->d->longitude = locationData[QLatin1String("longitude")].toReal();
    location->d->altitude = locationData[QLatin1String("altitude")].toReal();
}

DriveFile::ImageMediaMetadata::ImageMediaMetadata(const ImageMediaMetadata& other):
    d(new Private(*(other.d)))
{
}

DriveFile::ImageMediaMetadata::~ImageMediaMetadata()
{
    delete d;
}

int DriveFile::ImageMediaMetadata::width() const
{
    return d->width;
}

int DriveFile::ImageMediaMetadata::height() const
{
    return d->height;
}

int DriveFile::ImageMediaMetadata::rotation() const
{
    return d->rotation;
}

DriveFile::ImageMediaMetadata::LocationPtr DriveFile::ImageMediaMetadata::location() const
{
    return d->location;
}

QString DriveFile::ImageMediaMetadata::date() const
{
    return d->date;
}

QString DriveFile::ImageMediaMetadata::cameraMake() const
{
    return d->cameraMake;
}

QString DriveFile::ImageMediaMetadata::cameraModel() const
{
    return d->cameraModel;
}

float DriveFile::ImageMediaMetadata::exposureTime() const
{
    return d->exposureTime;
}

float DriveFile::ImageMediaMetadata::aperture() const
{
    return d->aperture;
}

bool DriveFile::ImageMediaMetadata::flashUsed() const
{
    return d->flashUsed;
}

float DriveFile::ImageMediaMetadata::focalLength() const
{
    return d->focalLength;
}

int DriveFile::ImageMediaMetadata::isoSpeed() const
{
    return d->isoSpeed;
}

QString DriveFile::ImageMediaMetadata::meteringMode() const
{
    return d->meteringMode;
}

QString DriveFile::ImageMediaMetadata::sensor() const
{
    return d->sensor;
}

QString DriveFile::ImageMediaMetadata::exposureMode() const
{
    return d->exposureMode;
}

QString DriveFile::ImageMediaMetadata::colorSpace() const
{
    return d->colorSpace;
}

QString DriveFile::ImageMediaMetadata::whiteBalance() const
{
    return d->whiteBalance;
}

float DriveFile::ImageMediaMetadata::exposureBias() const
{
    return d->exposureBias;
}

float DriveFile::ImageMediaMetadata::maxApertureValue() const
{
    return d->maxApertureValue;
}

int DriveFile::ImageMediaMetadata::subjectDistance() const
{
    return d->subjectDistance;
}

QString DriveFile::ImageMediaMetadata::lens() const
{
    return d->lens;
}


////// DriveFile::Thumbnail

class DriveFile::Thumbnail::Private
{
  public:
    Private();
    Private(const Private &other);

    QImage image;
    QString mimeType;
};

DriveFile::Thumbnail::Private::Private()
{
}

DriveFile::Thumbnail::Private::Private(const Private &other):
    image(other.image),
    mimeType(other.mimeType)
{
}

DriveFile::Thumbnail::Thumbnail(const QVariantMap &map):
    d(new Private)
{
    const QByteArray ba = QByteArray::fromBase64(map[QLatin1String("image")].toByteArray());
    d->image = QImage::fromData(ba);
    d->mimeType = map[QLatin1String("mimeType")].toString();
}

DriveFile::Thumbnail::Thumbnail(const DriveFile::Thumbnail &other):
    d(new Private(*(other.d)))
{
}

DriveFile::Thumbnail::~Thumbnail()
{
    delete d;
}

QImage DriveFile::Thumbnail::image() const
{
    return d->image;
}

QString DriveFile::Thumbnail::mimeType() const
{
    return d->mimeType;
}

////// DriveFile

DriveFile::Private::Private():
    fileSize(-1),
    quotaBytesUsed(-1),
    editable(false),
    writersCanShare(false),
    explicitlyTrashed(false),
    shared(false)
{
}

DriveFile::Private::Private(const Private& other):
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

DriveFilePtr DriveFile::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#file"))
    {
        return DriveFilePtr();
    }

    DriveFilePtr file(new DriveFile());
    file->setEtag(map[QLatin1String("etag")].toString());
    file->d->id = map[QLatin1String("id")].toString();
    file->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    file->d->title = map[QLatin1String("title")].toString();
    file->d->mimeType = map[QLatin1String("mimeType")].toString();
    file->d->description = map[QLatin1String("description")].toString();

    const QVariantMap labelsData =  map[QLatin1String("lables")].toMap();
    DriveFile::LabelsPtr labels(new DriveFile::Labels());
    labels->d->starred = labelsData[QLatin1String("starred")].toBool();
    labels->d->hidden = labelsData[QLatin1String("hidden")].toBool();
    labels->d->trashed = labelsData[QLatin1String("trashed")].toBool();
    labels->d->restricted = labelsData[QLatin1String("restricted")].toBool();
    labels->d->viewed = labelsData[QLatin1String("viewed")].toBool();
    file->d->labels = labels;

    // FIXME FIXME FIXME Verify the date format
    file->d->createdDate = KDateTime::fromString(map[QLatin1String("createdDate")].toString(), KDateTime::KDateTime::RFC3339Date);
    file->d->modifiedDate = KDateTime::fromString(map[QLatin1String("modifiedDate")].toString(), KDateTime::KDateTime::RFC3339Date);
    file->d->modifiedByMeDate = KDateTime::fromString(map[QLatin1String("modifiedByMeDate")].toString(), KDateTime::RFC3339Date);
    file->d->downloadUrl = map[QLatin1String("downloadUrl")].toUrl();

    const QVariantMap indexableTextData = map[QLatin1String("indexableText")].toMap();
    DriveFile::IndexableTextPtr indexableText(new DriveFile::IndexableText());
    indexableText->d->text = indexableTextData[QLatin1String("text")].toString();
    file->d->indexableText = indexableText;

    const QVariantMap userPermissionData = map[QLatin1String("userPermission")].toMap();
    file->d->userPermission = DrivePermission::Private::fromJSON(userPermissionData);

    file->d->fileExtension = map[QLatin1String("fileExtension")].toString();
    file->d->md5Checksum = map[QLatin1String("md5Checksum")].toString();
    file->d->fileSize = map[QLatin1String("fileSize")].toLongLong();
    file->d->alternateLink = map[QLatin1String("alternateLink")].toUrl();
    file->d->embedLink = map[QLatin1String("embedLink")].toUrl();
    file->d->sharedWithMeDate = KDateTime::fromString(map[QLatin1String("sharedWithMeDate")].toString(), KDateTime::RFC3339Date);

    const QVariantList parents = map[QLatin1String("parents")].toList();
    Q_FOREACH (const QVariant &parent, parents)
    {
        file->d->parents << DriveParentReference::Private::fromJSON(parent.toMap());
    }

    const QVariantMap exportLinksData = map[QLatin1String("exportLinks")].toMap();
    Q_FOREACH (const QString &format, exportLinksData.keys()) {
        file->d->exportLinks.insert(format, exportLinksData[format].toUrl());
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
        DriveFile::ImageMediaMetadataPtr(new DriveFile::ImageMediaMetadata(imageMetaData));

    const QVariantMap thumbnailData = map[QLatin1String("thumbnail")].toMap();
    DriveFile::ThumbnailPtr thumbnail(new DriveFile::Thumbnail(thumbnailData));
    file->d->thumbnail = thumbnail;

    file->d->webViewLink = map[QLatin1String("webViewLink")].toUrl();
    file->d->iconLink = map[QLatin1String("iconLink")].toUrl();
    file->d->shared = map[QLatin1String("shared")].toBool();

    const QVariantList ownersList = map[QLatin1String("owners")].toList();
    Q_FOREACH (const QVariant &owner, ownersList) {
        file->d->owners << DriveUser::fromJSON(owner.toMap());
    }

    const QVariantMap lastModifyingUser = map[QLatin1String("lastModifyingUser")].toMap();
    file->d->lastModifyingUser = DriveUser::fromJSON(lastModifyingUser);

    return file;
}

DriveFile::DriveFile():
    KGAPI2::Object(),
    d(new Private)
{
}

DriveFile::DriveFile(const DriveFile& other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{ }

DriveFile::~DriveFile()
{
    delete d;
}

QString DriveFile::id() const
{
    return d->id;
}

QUrl DriveFile::selfLink() const
{
    return d->selfLink;
}

QString DriveFile::title() const
{
    return d->title;
}

void DriveFile::setTitle(const QString& title)
{
    d->title = title;
}

QString DriveFile::mimeType() const
{
    return d->mimeType;
}

void DriveFile::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

QString DriveFile::description() const
{
    return d->description;
}

void DriveFile::setDescription(const QString& description)
{
    d->description = description;
}

DriveFile::LabelsPtr DriveFile::labels() const
{
    return d->labels;
}

void DriveFile::setLabels(const DriveFile::LabelsPtr &labels)
{
    d->labels = labels;
}

KDateTime DriveFile::createdDate() const
{
    return d->createdDate;
}

KDateTime DriveFile::modifiedDate() const
{
    return d->modifiedDate;
}

void DriveFile::setModifiedDate(const KDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

KDateTime DriveFile::modifiedByMeDate() const
{
    return d->modifiedByMeDate;
}

QUrl DriveFile::downloadUrl() const
{
    return d->downloadUrl;
}

DriveFile::IndexableTextPtr& DriveFile::indexableText()
{
    return d->indexableText;
}

DrivePermissionPtr DriveFile::userPermission() const
{
    return d->userPermission;
}

QString DriveFile::fileExtension() const
{
    return d->fileExtension;
}

QString DriveFile::md5Checksum() const
{
    return d->md5Checksum;
}

qlonglong DriveFile::fileSize() const
{
    return d->fileSize;
}

QUrl DriveFile::alternateLink() const
{
    return d->alternateLink;
}

QUrl DriveFile::embedLink() const
{
    return d->embedLink;
}

KDateTime DriveFile::sharedWithMeDate() const
{
    return d->sharedWithMeDate;
}

DriveParentReferencesList DriveFile::parents() const
{
    return d->parents;
}

void DriveFile::setParents(const DriveParentReferencesList &parents)
{
    d->parents = parents;
}

QMap< QString, QUrl > DriveFile::exportLinks() const
{
    return d->exportLinks;
}

QString DriveFile::originalFileName() const
{
    return d->originalFileName;
}

qlonglong DriveFile::quotaBytesUsed() const
{
    return d->quotaBytesUsed;
}

QStringList DriveFile::ownerNames() const
{
    return d->ownerNames;
}

QString DriveFile::lastModifyingUserName() const
{
    return d->lastModifyingUserName;
}

bool DriveFile::editable() const
{
    return d->editable;
}

bool DriveFile::writersCanShare() const
{
    return d->writersCanShare;
}

QUrl DriveFile::thumbnailLink() const
{
    return d->thumbnailLink;
}

KDateTime DriveFile::lastViewedByMeDate() const
{
    return d->lastViewedByMeDate;
}

void DriveFile::setLastViewedByMeDate(const KDateTime& lastViewedByMeDate)
{
    d->lastViewedByMeDate = lastViewedByMeDate;
}

QUrl DriveFile::webContentLink() const
{
    return d->webContentLink;
}

bool DriveFile::explicitlyTrashed() const
{
    return d->explicitlyTrashed;
}

DriveFile::ImageMediaMetadataPtr DriveFile::imageMediaMetadata() const
{
    return d->imageMediaMetadata;
}

DriveFile::ThumbnailPtr DriveFile::thumbnail() const
{
    return d->thumbnail;
}

QUrl DriveFile::webViewLink() const
{
    return d->webViewLink;
}

QUrl DriveFile::iconLink() const
{
    return d->iconLink;
}

bool DriveFile::shared() const
{
    return d->shared;
}

DriveUsersList DriveFile::owners() const
{
    return d->owners;
}

DriveUserPtr DriveFile::lastModifyingUser() const
{
    return d->lastModifyingUser;
}

DriveFilePtr DriveFile::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return DriveFilePtr();
    }

    return Private::fromJSON(data.toMap());
}

DriveFilesList DriveFile::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return DriveFilesList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#fileList"))
    {
        return DriveFilesList();
    }

    DriveFilesList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const DriveFilePtr file = Private::fromJSON(item.toMap());

        if (!file.isNull()) {
            list << file;
        }
    }

    if (map.contains(QLatin1String("nextLink"))) {
        feedData.nextPageUrl = map[QLatin1String("nextLink")].toUrl();
    }

    return list;
}

QByteArray DriveFile::toJSON(const DriveFilePtr &file)
{
    QVariantMap map;

    map[QLatin1String("description")] = file->description();

    QVariantMap indexableText;
    indexableText[QLatin1String("text")] = file->indexableText()->text();
    map[QLatin1String("indexableText")] = indexableText;

    QVariantMap labels;
    labels[QLatin1String("hidden")] = file->labels()->hidden();
    labels[QLatin1String("restricted")] = file->labels()->restricted();
    labels[QLatin1String("starred")] = file->labels()->starred();
    labels[QLatin1String("trashed")] = file->labels()->trashed();
    labels[QLatin1String("viewed")] = file->labels()->viewed();
    map[QLatin1String("labels")] = labels;

    map[QLatin1String("lastViewedByMeDate")] = file->lastViewedByMeDate().toString(KDateTime::RFC3339Date);
    map[QLatin1String("mimeType")] = file->mimeType();
    map[QLatin1String("modifiedDate")] = file->modifiedDate().toString(KDateTime::RFC3339Date);
    map[QLatin1String("title")] = file->title();

    QVariantList parents;
    Q_FOREACH (const DriveParentReferencePtr &parent, file->parents()) {
        parents << DriveParentReference::Private::toJSON(parent);
    }
    map[QLatin1String("parents")] = parents;

    QJson::Serializer serializer;
    return serializer.serialize(map);
}






