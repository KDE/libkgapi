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

#include <qjson/parser.h>
#include <KUrl>

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
    KDateTime published;
    KDateTime updated;
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

KDateTime Comment::published() const
{
    return d->published;
}

void Comment::setPublished(const KDateTime &published)
{
    d->published = published;
}

KDateTime Comment::updated() const
{
    return d->updated;
}

void Comment::setUpdated(const KDateTime &updated)
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

    comment->d->id = map[QLatin1String("id")].toString();
    comment->d->postId = map[QLatin1String("post")].toMap()[QLatin1String("id")].toString();
    comment->d->blogId = map[QLatin1String("blog")].toMap()[QLatin1String("id")].toString();
    comment->d->published = KDateTime::fromString(map[QLatin1String("published")].toString(), KDateTime::RFC3339Date);
    comment->d->updated = KDateTime::fromString(map[QLatin1String("updated")].toString(), KDateTime::RFC3339Date);
    comment->d->content = map[QLatin1String("content")].toString();
    const QVariantMap author = map[QLatin1String("author")].toMap();
    comment->d->authorId = author[QLatin1String("id")].toString();
    comment->d->authorName = author[QLatin1String("displayName")].toString();
    comment->d->authorUrl = author[QLatin1String("url")].toUrl();
    comment->d->authorImageUrl = author[QLatin1String("image")].toMap()[QLatin1String("url")].toUrl();
    comment->d->inReplyTo = map[QLatin1String("inReplyTo")].toMap()[QLatin1String("id")].toString();
    comment->d->status = map[QLatin1String("status")].toString();

    return comment;
}

CommentPtr Comment::fromJSON(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap json = parser.parse(rawData, &ok).toMap();
    if (!ok || json[QLatin1String("kind")].toString() != QLatin1String("blogger#comment")) {
        return CommentPtr();
    }

    return Comment::Private::fromJSON(json);
}

ObjectsList Comment::fromJSONFeed(const QByteArray &rawData, FeedData &feedData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap json = parser.parse(rawData, &ok).toMap();
    if (!ok || json[QLatin1String("kind")].toString() != QLatin1String("blogger#commentList")) {
        return ObjectsList();
    }

    ObjectsList items;
    if (!json[QLatin1String("nextPageToken")].toString().isEmpty()) {
        KUrl requestUrl(feedData.requestUrl);
        requestUrl.removeQueryItem(QLatin1String("pageToken"));
        requestUrl.addQueryItem(QLatin1String("pageToken"), json[QLatin1String("nextPageToken")].toString());
        feedData.nextPageUrl = requestUrl;
    }
    Q_FOREACH (const QVariant &v, json[QLatin1String("items")].toList()) {
        items << Comment::Private::fromJSON(v);
    }

    return items;
}
