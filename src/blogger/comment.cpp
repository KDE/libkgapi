/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "comment.h"

#include <QJsonDocument>
#include <QUrlQuery>
#include <QVariant>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN Comment::Private
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
    if (map[QStringLiteral("kind")].toString() != QLatin1StringView("blogger#comment")) {
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
    if (map[QStringLiteral("kind")].toString() != QLatin1StringView("blogger#commentList")) {
        return ObjectsList();
    }

    ObjectsList items;
    if (!map[QStringLiteral("nextPageToken")].toString().isEmpty()) {
        feedData.nextPageUrl = feedData.requestUrl;
        QUrlQuery query(feedData.nextPageUrl);
        query.removeQueryItem(QStringLiteral("pageToken"));
        query.addQueryItem(QStringLiteral("pageToken"), map[QStringLiteral("nextPageToken")].toString());
        feedData.nextPageUrl.setQuery(query);
    }
    const QVariantList variantList = map[QStringLiteral("items")].toList();
    items.reserve(variantList.size());
    for (const QVariant &v : variantList) {
        items << Comment::Private::fromJSON(v);
    }

    return items;
}
