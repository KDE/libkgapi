/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "post.h"

#include <qjson/serializer.h>
#include <qjson/parser.h>

#include <KUrl>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Post::Private
{
  public:
    Private();

    static PostPtr fromJSON(const QVariant &json);
    static QVariant toJSON(const PostPtr &post);

    QString id;
    QString blogId;
    KDateTime published;
    KDateTime updated;
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

KDateTime Post::published() const
{
    return d->published;
}

void Post::setPublished(const KDateTime &published)
{
    d->published = published;
}

KDateTime Post::updated() const
{
    return d->updated;
}

void Post::setUpdated(const KDateTime &updated)
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

    post->d->id = map[QLatin1String("id")].toString();
    post->d->blogId = map[QLatin1String("blog")].toMap()[QLatin1String("id")].toString();
    post->d->published = KDateTime::fromString(map[QLatin1String("published")].toString(), KDateTime::RFC3339Date);
    post->d->updated = KDateTime::fromString(map[QLatin1String("updated")].toString(), KDateTime::RFC3339Date);
    post->d->url = map[QLatin1String("url")].toUrl();
    post->d->title = map[QLatin1String("title")].toString();
    post->d->content = map[QLatin1String("content")].toString();
    const QVariantMap author = map[QLatin1String("author")].toMap();
    post->d->authorId = author[QLatin1String("id")].toString();
    post->d->authorName = author[QLatin1String("displayName")].toString();
    post->d->authorUrl = author[QLatin1String("url")].toUrl();
    post->d->authorImageUrl = author[QLatin1String("image")].toMap()[QLatin1String("url")].toUrl();
    post->d->commentsCount = map[QLatin1String("replies")].toMap()[QLatin1String("totalItems")].toUInt();
    post->d->labels = map[QLatin1String("labels")].toStringList();
    post->d->customMetaData = map[QLatin1String("customMetaData")];
    const QVariantMap location = map[QLatin1String("location")].toMap();
    post->d->location = location[QLatin1String("name")].toString();
    post->d->latitude = location[QLatin1String("lat")].toDouble();
    post->d->longitude = location[QLatin1String("lng")].toDouble();
    Q_FOREACH (const QVariant &url, map[QLatin1String("images")].toList()) {
        post->d->images << url.toMap()[QLatin1String("url")].toUrl();
    }
    post->d->status = map[QLatin1String("status")].toString();

    return post;
}

QVariant Post::Private::toJSON(const PostPtr &post)
{
    QVariantMap json;
    json[QLatin1String("kind")] = QLatin1String("blogger#post");
    if (!post->d->id.isEmpty()) {
        json[QLatin1String("id")] = post->d->id;
    }
    if (!post->d->blogId.isEmpty()) {
        QVariantMap blog;
        blog[QLatin1String("id")] = post->d->blogId;
        json[QLatin1String("blog")] = blog;
    }

    if (post->d->published.isValid()) {
        json[QLatin1String("published")] = post->d->published.toString(KDateTime::RFC3339Date);
    }
    if (post->d->updated.isValid()) {
        json[QLatin1String("updated")] = post->d->updated.toString(KDateTime::RFC3339Date);
    }
    json[QLatin1String("title")] = post->d->title;
    json[QLatin1String("content")] = post->d->content;
    if (!post->d->labels.isEmpty()) {
        json[QLatin1String("labels")] = post->d->labels;
    }
    if (!post->d->customMetaData.isNull()) {
        QJson::Serializer serializer;
        json[QLatin1String("customMetaData")] = serializer.serialize(post->d->customMetaData);
    }
    if (!post->d->location.isEmpty() && post->d->latitude > -1 && post->d->longitude > -1) {
        QVariantMap location;
        location[QLatin1String("name")] = post->d->location;
        location[QLatin1String("lat")] = post->d->latitude;
        location[QLatin1String("lng")] = post->d->longitude;
        json[QLatin1String("location")] = location;
    }
    if (!post->d->images.isEmpty()) {
        QVariantList images;
        Q_FOREACH (const QUrl &url, post->d->images) {
            QVariantMap image;
            image[QLatin1String("url")] = url.toString();
            images << image;
        }
        json[QLatin1String("images")] = images;
    }

    return json;
}

PostPtr Post::fromJSON(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap json = parser.parse(rawData, &ok).toMap();
    if (!ok || json[QLatin1String("kind")].toString() != QLatin1String("blogger#post")) {
        return PostPtr();
    }

    return Private::fromJSON(json);
}

ObjectsList Post::fromJSONFeed(const QByteArray &rawData, FeedData &feedData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap json = parser.parse(rawData, &ok).toMap();
    if (!ok || json[QLatin1String("kind")].toString() != QLatin1String("blogger#postList")) {
        return ObjectsList();
    }

    if (!json[QLatin1String("nextPageToken")].toString().isEmpty()) {
        KUrl requestUrl(feedData.requestUrl);
        requestUrl.removeQueryItem(QLatin1String("pageToken"));
        requestUrl.addQueryItem(QLatin1String("pageToken"), json[QLatin1String("nextPageToken")].toString());
        feedData.nextPageUrl = requestUrl;
    }
    ObjectsList list;
    Q_FOREACH (const QVariant &item, json[QLatin1String("items")].toList()) {
        list << Private::fromJSON(item);
    }
    return list;
}

QByteArray Post::toJSON(const PostPtr &post)
{
    QJson::Serializer serializer;
    return serializer.serialize(Private::toJSON(post));
}
