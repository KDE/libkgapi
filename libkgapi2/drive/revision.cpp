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
#include "user.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

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
    bool publishAuto;
    bool publishedOutsideDomain;
    QUrl downloadUrl;
    QMap<QString, QUrl> exportLinks;
    QString lastModifyingUserName;
    DriveUserPtr lastModifyingUser;
    QString originalFilename;
    QString md5Checksum;
    qlonglong fileSize;

    static DriveRevisionPtr fromJSON(const QVariantMap &map);
};

DriveRevision::Private::Private():
    pinned(false),
    published(false),
    publishAuto(false),
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
    publishAuto(other.publishAuto),
    publishedOutsideDomain(other.publishedOutsideDomain),
    downloadUrl(other.downloadUrl),
    exportLinks(other.exportLinks),
    lastModifyingUserName(other.lastModifyingUserName),
    lastModifyingUser(other.lastModifyingUser),
    originalFilename(other.originalFilename),
    md5Checksum(other.md5Checksum),
    fileSize(other.fileSize)
{
}

DriveRevisionPtr DriveRevision::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#revision"))
    {
        return DriveRevisionPtr();
    }

    DriveRevisionPtr revision(new DriveRevision);
    revision->setEtag(map[QLatin1String("etag")].toString());
    revision->d->id = map[QLatin1String("id")].toString();
    revision->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    revision->d->mimeType = map[QLatin1String("mimeType")].toString();
    revision->d->modifiedDate = KDateTime::fromString(map[QLatin1String("modifiedDate")].toString(), KDateTime::RFC3339Date);
    revision->d->pinned = map[QLatin1String("pinned")].toBool();
    revision->d->published = map[QLatin1String("published")].toBool();
    revision->d->publishedLink = map[QLatin1String("publishedLink")].toUrl();
    revision->d->publishAuto = map[QLatin1String("publishAuto")].toBool();
    revision->d->publishedOutsideDomain = map[QLatin1String("publishedOutsideDomain")].toBool();
    revision->d->downloadUrl = map[QLatin1String("downloadUrl")].toUrl();
    revision->d->lastModifyingUserName = map[QLatin1String("lastModifyingUserName")].toString();
    revision->d->lastModifyingUser = DriveUser::fromJSON(map[QLatin1String("lastModifyingUser")].toMap());
    revision->d->originalFilename = map[QLatin1String("originalFilename")].toString();
    revision->d->md5Checksum = map[QLatin1String("md5Checksum")].toString();
    revision->d->fileSize = map[QLatin1String("fileSize")].toLongLong();

    const QVariantMap exportLinks = map[QLatin1String("exportLinks")].toMap();
    QVariantMap::ConstIterator iter = exportLinks.constBegin();
    for ( ; iter != exportLinks.constEnd(); ++iter) {
        revision->d->exportLinks.insert(iter.key(), iter.value().toUrl());
    }

    return revision;
}

DriveRevision::DriveRevision():
    KGAPI2::Object(),
    d(new Private)
{
}

DriveRevision::DriveRevision(const DriveRevision& other):
    KGAPI2::Object(other),
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

QUrl DriveRevision::selfLink() const
{
    return d->selfLink;
}

QString DriveRevision::mimeType() const
{
    return d->mimeType;
}

KDateTime DriveRevision::modifiedDate() const
{
    return d->modifiedDate;
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

bool DriveRevision::publishAuto() const
{
    return d->publishAuto;
}

void DriveRevision::setPublishAuto(bool publishAuto)
{
    d->publishAuto = publishAuto;
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

QMap< QString, QUrl > DriveRevision::exportLinks() const
{
    return d->exportLinks;
}

QString DriveRevision::lastModifyingUserName() const
{
    return d->lastModifyingUserName;
}

DriveUserPtr DriveRevision::lastModifyingUser() const
{
    return d->lastModifyingUser;
}

QString DriveRevision::originalFilename() const
{
    return d->originalFilename;
}

QString DriveRevision::md5Checksum() const
{
    return d->md5Checksum;
}

qlonglong DriveRevision::fileSize() const
{
    return d->fileSize;
}

DriveRevisionPtr DriveRevision::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return DriveRevisionPtr();
    }

    return Private::fromJSON(data.toMap());
}

DriveRevisionsList DriveRevision::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return DriveRevisionsList();
    }

    const QVariantMap map = data.toMap();

    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#revisionList"))
    {
        return DriveRevisionsList();
    }

    DriveRevisionsList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const DriveRevisionPtr revision = Private::fromJSON(item.toMap());

        if (!revision.isNull()) {
            list << revision;
        }
    }

    return list;
}

QByteArray DriveRevision::toJSON(const DriveRevisionPtr &revision)
{
    QVariantMap map;

    map[QLatin1String("pinned")] = revision->pinned();
    map[QLatin1String("published")] = revision->published();
    map[QLatin1String("publishAuto")] = revision->publishAuto();
    map[QLatin1String("publishedOutsideDomain")] = revision->publishedOutsideDomain();

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
