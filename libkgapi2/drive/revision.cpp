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
using namespace KGAPI2::Drive;

class Revision::Private
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
    UserPtr lastModifyingUser;
    QString originalFilename;
    QString md5Checksum;
    qlonglong fileSize;

    static RevisionPtr fromJSON(const QVariantMap &map);
};

Revision::Private::Private():
    pinned(false),
    published(false),
    publishAuto(false),
    publishedOutsideDomain(false),
    fileSize(-1)
{
}

Revision::Private::Private(const Private& other):
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

RevisionPtr Revision::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#revision"))
    {
        return RevisionPtr();
    }

    RevisionPtr revision(new Revision);
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
    revision->d->lastModifyingUser = User::fromJSON(map[QLatin1String("lastModifyingUser")].toMap());
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

Revision::Revision():
    KGAPI2::Object(),
    d(new Private)
{
}

Revision::Revision(const Revision& other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

Revision::~Revision()
{
    delete d;
}

QString Revision::id() const
{
    return d->id;
}

QUrl Revision::selfLink() const
{
    return d->selfLink;
}

QString Revision::mimeType() const
{
    return d->mimeType;
}

KDateTime Revision::modifiedDate() const
{
    return d->modifiedDate;
}

bool Revision::pinned() const
{
    return d->pinned;
}

void Revision::setPinned(bool pinned)
{
    d->pinned = pinned;
}

bool Revision::published() const
{
    return d->published;
}

void Revision::setPublished(bool published)
{
    d->published = published;
}

QUrl Revision::publishedLink() const
{
    return d->publishedLink;
}

bool Revision::publishAuto() const
{
    return d->publishAuto;
}

void Revision::setPublishAuto(bool publishAuto)
{
    d->publishAuto = publishAuto;
}

bool Revision::publishedOutsideDomain() const
{
    return d->publishedOutsideDomain;
}

void Revision::setPublishedOutsideDomain(bool publishedOutsideDomain)
{
    d->publishedOutsideDomain = publishedOutsideDomain;
}

QUrl Revision::downloadUrl() const
{
    return d->downloadUrl;
}

QMap< QString, QUrl > Revision::exportLinks() const
{
    return d->exportLinks;
}

QString Revision::lastModifyingUserName() const
{
    return d->lastModifyingUserName;
}

UserPtr Revision::lastModifyingUser() const
{
    return d->lastModifyingUser;
}

QString Revision::originalFilename() const
{
    return d->originalFilename;
}

QString Revision::md5Checksum() const
{
    return d->md5Checksum;
}

qlonglong Revision::fileSize() const
{
    return d->fileSize;
}

RevisionPtr Revision::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return RevisionPtr();
    }

    return Private::fromJSON(data.toMap());
}

RevisionsList Revision::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return RevisionsList();
    }

    const QVariantMap map = data.toMap();

    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#revisionList"))
    {
        return RevisionsList();
    }

    RevisionsList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const RevisionPtr revision = Private::fromJSON(item.toMap());

        if (!revision.isNull()) {
            list << revision;
        }
    }

    return list;
}

QByteArray Revision::toJSON(const RevisionPtr &revision)
{
    QVariantMap map;

    map[QLatin1String("pinned")] = revision->pinned();
    map[QLatin1String("published")] = revision->published();
    map[QLatin1String("publishAuto")] = revision->publishAuto();
    map[QLatin1String("publishedOutsideDomain")] = revision->publishedOutsideDomain();

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
