/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "post.h"

#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN Post::Private
{
  public:
    Private();

    static PostPtr fromJSON(const QVariant &json);
    static QVariant toJSON(const PostPtr &post);

    QString id;
    QString blogId;
    QDateTime published;
    QDateTime updated;
    QUrl url;
    QString title;
    QString content;
    QString authorId;
    QString authorName;
    QUrl authorUrl;
    QUrl authorImageUrl;
    uint commentsCount;
    QStringList labels;
    QVariant customMetaData;
    QString location;
    double latitude;
    double longitude;
    QList<QUrl> images;
    QString status;
};

Post::Private::Private()
    : commentsCount(0)
    , latitude(-1)
    , longitude(-1)
{
}


Post::Post()
  : d(new Private)
{
}

Post::~Post()
{
    delete d;
}

QString Post::id()
{
    return d->id;
}

void Post::setId(const QString &id)
{
    d->id = id;
}

QString Post::blogId()
{
    return d->blogId;
}

void Post::setBlogId(const QString &id)
{
    d->blogId = id;
}

QDateTime Post::published() const
{
    return d->published;
}

void Post::setPublished(const QDateTime &published)
{
    d->published = published;
}

QDateTime Post::updated() const
{
    return d->updated;
}

void Post::setUpdated(const QDateTime &updated)
{
    d->updated = updated;
}

QUrl Post::url() const
{
    return d->url;
}

void Post::setUrl(const QUrl &url)
{
    d->url = url;
}

QString Post::title() const
{
    return d->title;
}

void Post::setTitle(const QString &title)
{
    d->title = title;
}

QString Post::content() const
{
    return d->content;
}

void Post::setContent(const QString &content)
{
    d->content = content;
}

QString Post::authorId() const
{
    return d->authorId;
}

QString Post::authorName() const
{
    return d->authorName;
}

QUrl Post::authorUrl() const
{
    return d->authorUrl;
}

QUrl Post::authorImageUrl() const
{
    return d->authorImageUrl;
}

uint Post::commentsCount() const
{
    return d->commentsCount;
}

QStringList Post::labels() const
{
    return d->labels;
}

void Post::setLabels(const QStringList &labels)
{
    d->labels = labels;
}

QVariant Post::customMetaData() const
{
    return d->customMetaData;
}

void Post::setCustomMetaData(const QVariant &metadata)
{
    d->customMetaData = metadata;
}

QString Post::location() const
{
    return d->location;
}

void Post::setLocation(const QString &location)
{
    d->location = location;
}

double Post::latitude() const
{
    return d->latitude;
}

void Post::setLatitude(double lat)
{
    d->latitude = lat;
}

double Post::longitude() const
{
    return d->longitude;
}

void Post::setLongitute(double lng)
{
    d->longitude = lng;
}

QList<QUrl> Post::images() const
{
    return d->images;
}

void Post::setImages(const QList<QUrl> &images)
{
    d->images = images;
}

QString Post::status() const
{
    return d->status;
}


PostPtr Post::Private::fromJSON(const QVariant &json)
{
    PostPtr post(new Post);
    const QVariantMap map = json.toMap();

    post->d->id = map[QStringLiteral("id")].toString();
    post->d->blogId = map[QStringLiteral("blog")].toMap()[QStringLiteral("id")].toString();
    post->d->published = QDateTime::fromString(map[QStringLiteral("published")].toString(), Qt::ISODate);
    post->d->updated = QDateTime::fromString(map[QStringLiteral("updated")].toString(), Qt::ISODate);
    post->d->url = map[QStringLiteral("url")].toUrl();
    post->d->title = map[QStringLiteral("title")].toString();
    post->d->content = map[QStringLiteral("content")].toString();
    const QVariantMap author = map[QStringLiteral("author")].toMap();
    post->d->authorId = author[QStringLiteral("id")].toString();
    post->d->authorName = author[QStringLiteral("displayName")].toString();
    post->d->authorUrl = author[QStringLiteral("url")].toUrl();
    post->d->authorImageUrl = author[QStringLiteral("image")].toMap()[QStringLiteral("url")].toUrl();
    post->d->commentsCount = map[QStringLiteral("replies")].toMap()[QStringLiteral("totalItems")].toUInt();
    post->d->labels = map[QStringLiteral("labels")].toStringList();
    post->d->customMetaData = map[QStringLiteral("customMetaData")];
    const QVariantMap location = map[QStringLiteral("location")].toMap();
    post->d->location = location[QStringLiteral("name")].toString();
    post->d->latitude = location[QStringLiteral("lat")].toDouble();
    post->d->longitude = location[QStringLiteral("lng")].toDouble();

    const QVariantList variantList = map[QStringLiteral("images")].toList();
    for (const QVariant &url : variantList) {
        post->d->images << url.toMap()[QStringLiteral("url")].toUrl();
    }
    post->d->status = map[QStringLiteral("status")].toString();

    return post;
}

QVariant Post::Private::toJSON(const PostPtr &post)
{
    QVariantMap json;
    json[QStringLiteral("kind")] = QStringLiteral("blogger#post");
    if (!post->d->id.isEmpty()) {
        json[QStringLiteral("id")] = post->d->id;
    }
    if (!post->d->blogId.isEmpty()) {
        QVariantMap blog;
        blog[QStringLiteral("id")] = post->d->blogId;
        json[QStringLiteral("blog")] = blog;
    }

    if (post->d->published.isValid()) {
        json[QStringLiteral("published")] = post->d->published.toString(Qt::ISODate);
    }
    if (post->d->updated.isValid()) {
        json[QStringLiteral("updated")] = post->d->updated.toString(Qt::ISODate);
    }
    json[QStringLiteral("title")] = post->d->title;
    json[QStringLiteral("content")] = post->d->content;
    if (!post->d->labels.isEmpty()) {
        json[QStringLiteral("labels")] = post->d->labels;
    }
    if (!post->d->customMetaData.isNull()) {
        QJsonDocument document = QJsonDocument::fromVariant(post->d->customMetaData);
        json[QStringLiteral("customMetaData")] = document.toJson(QJsonDocument::Compact);
    }
    if (!post->d->location.isEmpty() && post->d->latitude > -1 && post->d->longitude > -1) {
        QVariantMap location;
        location[QStringLiteral("name")] = post->d->location;
        location[QStringLiteral("lat")] = post->d->latitude;
        location[QStringLiteral("lng")] = post->d->longitude;
        json[QStringLiteral("location")] = location;
    }
    if (!post->d->images.isEmpty()) {
        QVariantList images;
        Q_FOREACH (const QUrl &url, post->d->images) {
            QVariantMap image;
            image[QStringLiteral("url")] = url.toString();
            images << image;
        }
        json[QStringLiteral("images")] = images;
    }

    return json;
}

PostPtr Post::fromJSON(const QByteArray &rawData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return PostPtr();
    }

    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QStringLiteral("kind")].toString() != QLatin1String("blogger#post")) {
        return PostPtr();
    }

    return Private::fromJSON(map);
}

ObjectsList Post::fromJSONFeed(const QByteArray &rawData, FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return ObjectsList();
    }

    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QStringLiteral("kind")].toString() != QLatin1String("blogger#postList")) {
        return ObjectsList();
    }

    if (!map[QStringLiteral("nextPageToken")].toString().isEmpty()) {
        QUrl requestUrl(feedData.requestUrl);
        requestUrl.removeQueryItem(QStringLiteral("pageToken"));
        requestUrl.addQueryItem(QStringLiteral("pageToken"), map[QStringLiteral("nextPageToken")].toString());
        feedData.nextPageUrl = requestUrl;
    }
    ObjectsList list;
    const QVariantList variantList = map[QStringLiteral("items")].toList();
    list.reserve(variantList.size());
    for (const QVariant &item : variantList) {
        list << Private::fromJSON(item);
    }
    return list;
}

QByteArray Post::toJSON(const PostPtr &post)
{
    QJsonDocument document = QJsonDocument::fromVariant(Private::toJSON(post));
    return document.toJson(QJsonDocument::Compact);
}
