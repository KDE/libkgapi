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

#include "page.h"

#include <QVariant>
#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Page::Private
{
  public:
    Private();
    ~Private();

    static PagePtr fromJSON(const QVariant &json);
    static QVariant toJSON(const PagePtr &page);

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
    Status status;
};

Page::Private::Private()
    : status(Page::UnknownStatus)
{
}

Page::Private::~Private()
{
}

Page::Page()
    : Object()
    , d(new Private)
{
}

Page::~Page()
{
    delete d;
}

QString Page::id() const
{
    return d->id;
}

void Page::setId(const QString &id)
{
    d->id = id;
}

QString Page::blogId() const
{
    return d->blogId;
}

void Page::setBlogId(const QString &blogId)
{
    d->blogId = blogId;
}

QDateTime Page::published() const
{
    return d->published;
}

void Page::setPublished(const QDateTime &published)
{
    d->published = published;
}

QDateTime Page::updated() const
{
    return d->updated;
}

void Page::setUpdated(QDateTime &updated)
{
    d->updated = updated;
}

QUrl Page::url() const
{
    return d->url;
}

void Page::setUrl(const QUrl &url)
{
    d->url = url;
}

QString Page::title() const
{
    return d->title;
}

void Page::setTitle(const QString &title)
{
    d->title = title;
}

QString Page::content() const
{
    return d->content;
}

void Page::setContent(const QString &content)
{
    d->content = content;
}

QString Page::authorId() const
{
    return d->authorId;
}

QString Page::authorName() const
{
    return d->authorName;
}

QUrl Page::authorUrl() const
{
    return d->authorUrl;
}

QUrl Page::authorImageUrl() const
{
    return d->authorImageUrl;
}

Page::Status Page::status() const
{
    return d->status;
}

void Page::setStatus(Page::Status status)
{
    d->status = status;
}

PagePtr Page::Private::fromJSON(const QVariant &json)
{
    PagePtr page(new Page);
    const QVariantMap map = json.toMap();

    page->d->id = map[QStringLiteral("id")].toString();
    page->d->blogId = map[QStringLiteral("blog")].toMap()[QStringLiteral("id")].toString();
    page->d->published = QDateTime::fromString(map[QStringLiteral("published")].toString(), Qt::ISODate);
    page->d->updated = QDateTime::fromString(map[QStringLiteral("updated")].toString(), Qt::ISODate);
    page->d->url = map[QStringLiteral("url")].toUrl();
    page->d->title = map[QStringLiteral("title")].toString();
    page->d->content = map[QStringLiteral("content")].toString();
    const QVariantMap author = map[QStringLiteral("author")].toMap();
    page->d->authorId = author[QStringLiteral("id")].toString();
    page->d->authorName = author[QStringLiteral("displayName")].toString();
    page->d->authorUrl = author[QStringLiteral("url")].toUrl();
    page->d->authorImageUrl = author[QStringLiteral("image")].toMap()[QStringLiteral("url")].toUrl();

    const QString status = map[QStringLiteral("status")].toString();
    if (status == QLatin1String("LIVE")) {
        page->d->status = Live;
    } else if (status == QLatin1String("DRAFT")) {
        page->d->status = Draft;
    } else if (status == QLatin1String("IMPORTED")) {
        page->d->status = Imported;
    } else {
        page->d->status = UnknownStatus;
    }

    return page;
}

QVariant Page::Private::toJSON(const PagePtr &page)
{
    QVariantMap map;
    map[QLatin1String("kind")] = QLatin1String("blogger#page");

    if (!page->d->id.isEmpty()) {
        map[QLatin1String("id")] = page->d->id;
    }
    map[QLatin1String("blogId")] = page->d->blogId;
    if (page->d->published.isValid()) {
        map[QLatin1String("published")] = page->d->published.toString(Qt::ISODate);
    }
    if (page->d->updated.isValid()) {
        map[QLatin1String("updated")] = page->d->updated.toString(Qt::ISODate);
    }
    map[QLatin1String("url")] = page->d->url.toString();
    map[QLatin1String("title")] = page->d->title;
    map[QLatin1String("content")] = page->d->content;
    switch (page->d->status) {
    case Blogger::Page::Draft:
        map[QLatin1String("status")] = QLatin1String("DRAFT");
        break;
    case Blogger::Page::Live:
        map[QLatin1String("status")] = QLatin1String("LIVE");
        break;
    default:
        break; // not supported for writing
    }

    return map;
}

PagePtr Page::fromJSON(const QByteArray &rawData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return PagePtr();
    }

    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QLatin1String("kind")].toString() != QLatin1String("blogger#page")) {
        return PagePtr();
    }

    return Private::fromJSON(map);
}

ObjectsList Page::fromJSONFeed(const QByteArray &rawData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return ObjectsList();
    }
    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QLatin1String("kind")].toString() != QLatin1String("blogger#pageList")) {
        return ObjectsList();
    }

    ObjectsList list;
    Q_FOREACH (const QVariant &item, map[QLatin1String("items")].toList()) {
        list << Private::fromJSON(item);
    }
    return list;
}

QByteArray Page::toJSON(const PagePtr &page)
{
    QJsonDocument document = QJsonDocument::fromVariant(Private::toJSON(page));
    return document.toJson(QJsonDocument::Compact);
}
