/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "revision.h"
#include "user.h"
#include "utils_p.h"

#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN Revision::Private
{
public:
    Private();
    Private(const Private &other);

    QString id;
    QUrl selfLink;
    QString mimeType;
    QDateTime modifiedDate;
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

Revision::Private::Private()
    : pinned(false)
    , published(false)
    , publishAuto(false)
    , publishedOutsideDomain(false)
    , fileSize(-1)
{
}

Revision::Private::Private(const Private &other)
    : id(other.id)
    , selfLink(other.selfLink)
    , mimeType(other.mimeType)
    , modifiedDate(other.modifiedDate)
    , pinned(other.pinned)
    , published(other.published)
    , publishedLink(other.publishedLink)
    , publishAuto(other.publishAuto)
    , publishedOutsideDomain(other.publishedOutsideDomain)
    , downloadUrl(other.downloadUrl)
    , exportLinks(other.exportLinks)
    , lastModifyingUserName(other.lastModifyingUserName)
    , lastModifyingUser(other.lastModifyingUser)
    , originalFilename(other.originalFilename)
    , md5Checksum(other.md5Checksum)
    , fileSize(other.fileSize)
{
}

RevisionPtr Revision::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#revision")) {
        return RevisionPtr();
    }

    RevisionPtr revision(new Revision);
    revision->setEtag(map[QStringLiteral("etag")].toString());
    revision->d->id = map[QStringLiteral("id")].toString();
    revision->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    revision->d->mimeType = map[QStringLiteral("mimeType")].toString();
    revision->d->modifiedDate = QDateTime::fromString(map[QStringLiteral("modifiedDate")].toString(), Qt::ISODate);
    revision->d->pinned = map[QStringLiteral("pinned")].toBool();
    revision->d->published = map[QStringLiteral("published")].toBool();
    revision->d->publishedLink = map[QStringLiteral("publishedLink")].toUrl();
    revision->d->publishAuto = map[QStringLiteral("publishAuto")].toBool();
    revision->d->publishedOutsideDomain = map[QStringLiteral("publishedOutsideDomain")].toBool();
    revision->d->downloadUrl = map[QStringLiteral("downloadUrl")].toUrl();
    revision->d->lastModifyingUserName = map[QStringLiteral("lastModifyingUserName")].toString();
    revision->d->lastModifyingUser = User::fromJSON(map[QStringLiteral("lastModifyingUser")].toMap());
    revision->d->originalFilename = map[QStringLiteral("originalFilename")].toString();
    revision->d->md5Checksum = map[QStringLiteral("md5Checksum")].toString();
    revision->d->fileSize = map[QStringLiteral("fileSize")].toLongLong();

    const QVariantMap exportLinks = map[QStringLiteral("exportLinks")].toMap();
    QVariantMap::ConstIterator iter = exportLinks.constBegin();
    for (; iter != exportLinks.constEnd(); ++iter) {
        revision->d->exportLinks.insert(iter.key(), iter.value().toUrl());
    }

    return revision;
}

Revision::Revision()
    : KGAPI2::Object()
    , d(new Private)
{
}

Revision::Revision(const Revision &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

Revision::~Revision()
{
    delete d;
}

bool Revision::operator==(const Revision &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }
    GAPI_COMPARE(id)
    GAPI_COMPARE(selfLink)
    GAPI_COMPARE(mimeType)
    GAPI_COMPARE(modifiedDate)
    GAPI_COMPARE(pinned)
    GAPI_COMPARE(published)
    GAPI_COMPARE(publishedLink)
    GAPI_COMPARE(publishAuto)
    GAPI_COMPARE(publishedOutsideDomain)
    GAPI_COMPARE(downloadUrl)
    GAPI_COMPARE(exportLinks)
    GAPI_COMPARE(lastModifyingUserName)
    GAPI_COMPARE_SHAREDPTRS(lastModifyingUser)
    GAPI_COMPARE(originalFilename)
    GAPI_COMPARE(md5Checksum)
    GAPI_COMPARE(fileSize)
    return true;
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

QDateTime Revision::modifiedDate() const
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

QMap<QString, QUrl> Revision::exportLinks() const
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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return RevisionPtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

RevisionsList Revision::fromJSONFeed(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return RevisionsList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();

    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#revisionList")) {
        return RevisionsList();
    }

    RevisionsList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : std::as_const(items)) {
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

    map[QStringLiteral("pinned")] = revision->pinned();
    map[QStringLiteral("published")] = revision->published();
    map[QStringLiteral("publishAuto")] = revision->publishAuto();
    map[QStringLiteral("publishedOutsideDomain")] = revision->publishedOutsideDomain();

    QJsonDocument document = QJsonDocument::fromVariant(map);
    return document.toJson(QJsonDocument::Compact);
}
