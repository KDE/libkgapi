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

#include "comment.h"

#include <QVariant>
#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Comment::Private
{
  public:
    Private();

    static CommentPtr fromJSON(const QVariant &json);

    QString id;
    QString postId;
    QString blogId;
    QDateTime published;
    QDateTime updated;
    QString content;
    QString authorId;
    QString authorName;
    QUrl authorUrl;
    QUrl authorImageUrl;
    QString inReplyTo;
    QString status;
};

Comment::Private::Private()
{
}

Comment::Comment()
    : Object()
    , d(new Private)
{
}

Comment::~Comment()
{
    delete d;
}

QString Comment::id() const
{
    return d->id;
}

void Comment::setId(const QString &id)
{
    d->id = id;
}

QString Comment::postId() const
{
    return d->postId;
}

void Comment::setPostId(const QString &postId)
{
    d->postId = postId;
}

QString Comment::blogId() const
{
    return d->blogId;
}

void Comment::setBlogId(const QString &blogId)
{
    d->blogId = blogId;
}

QDateTime Comment::published() const
{
    return d->published;
}

void Comment::setPublished(const QDateTime &published)
{
    d->published = published;
}

QDateTime Comment::updated() const
{
    return d->updated;
}

void Comment::setUpdated(const QDateTime &updated)
{
    d->updated = updated;
}

QString Comment::content() const
{
    return d->content;
}

void Comment::setContent(const QString &content)
{
    d->content = content;
}

QString Comment::authorId() const
{
    return d->authorId;
}

void Comment::setAuthorId(const QString &authorId)
{
    d->authorId = authorId;
}

QString Comment::authorName() const
{
    return d->authorName;
}

void Comment::setAuthorName(const QString &authorName)
{
    d->authorName = authorName;
}

QUrl Comment::authorUrl() const
{
    return d->authorUrl;
}

void Comment::setAuthorUrl(const QUrl &url)
{
    d->authorUrl = url;
}

QUrl Comment::authorImageUrl() const
{
    return d->authorImageUrl;
}

void Comment::setAuthorImageUrl(const QUrl &authorImageUrl)
{
    d->authorImageUrl = authorImageUrl;
}

QString Comment::inReplyTo() const
{
    return d->inReplyTo;
}

void Comment::setInReplyTo(const QString &inReplyTo)
{
    d->inReplyTo = inReplyTo;
}

QString Comment::status() const
{
    return d->status;
}

void Comment::setStatus(const QString &status)
{
    d->status = status;
}

CommentPtr Comment::Private::fromJSON(const QVariant &json)
{
    CommentPtr comment(new Comment);
    const QVariantMap map = json.toMap();

    comment->d->id = map[QStringLiteral("id")].toString();
    comment->d->postId = map[QStringLiteral("post")].toMap()[QStringLiteral("id")].toString();
    comment->d->blogId = map[QStringLiteral("blog")].toMap()[QStringLiteral("id")].toString();
    comment->d->published = QDateTime::fromString(map[QStringLiteral("published")].toString(), Qt::ISODate);
    comment->d->updated = QDateTime::fromString(map[QStringLiteral("updated")].toString(), Qt::ISODate);
    comment->d->content = map[QStringLiteral("content")].toString();
    const QVariantMap author = map[QStringLiteral("author")].toMap();
    comment->d->authorId = author[QStringLiteral("id")].toString();
    comment->d->authorName = author[QStringLiteral("displayName")].toString();
    comment->d->authorUrl = author[QStringLiteral("url")].toUrl();
    comment->d->authorImageUrl = author[QStringLiteral("image")].toMap()[QStringLiteral("url")].toUrl();
    comment->d->inReplyTo = map[QStringLiteral("inReplyTo")].toMap()[QStringLiteral("id")].toString();
    comment->d->status = map[QStringLiteral("status")].toString();

    return comment;
}

CommentPtr Comment::fromJSON(const QByteArray &rawData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return CommentPtr();
    }
    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QLatin1String("kind")].toString() != QLatin1String("blogger#comment")) {
        return CommentPtr();
    }

    return Comment::Private::fromJSON(map);
}

ObjectsList Comment::fromJSONFeed(const QByteArray &rawData, FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return ObjectsList();
    }
    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QLatin1String("kind")].toString() != QLatin1String("blogger#commentList")) {
        return ObjectsList();
    }

    ObjectsList items;
    if (!map[QLatin1String("nextPageToken")].toString().isEmpty()) {
        QUrl requestUrl(feedData.requestUrl);
        requestUrl.removeQueryItem(QLatin1String("pageToken"));
        requestUrl.addQueryItem(QLatin1String("pageToken"), map[QLatin1String("nextPageToken")].toString());
        feedData.nextPageUrl = requestUrl;
    }
    Q_FOREACH (const QVariant &v, map[QLatin1String("items")].toList()) {
        items << Comment::Private::fromJSON(v);
    }

    return items;
}
