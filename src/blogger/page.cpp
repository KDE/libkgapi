/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "page.h"

#include <QVariant>
#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN Page::Private
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

void Page::setUpdated(const QDateTime &updated)
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
    map[QStringLiteral("kind")] = QLatin1String("blogger#page");

    if (!page->d->id.isEmpty()) {
        map[QStringLiteral("id")] = page->d->id;
    }
    map[QStringLiteral("blogId")] = page->d->blogId;
    if (page->d->published.isValid()) {
        map[QStringLiteral("published")] = page->d->published.toString(Qt::ISODate);
    }
    if (page->d->updated.isValid()) {
        map[QStringLiteral("updated")] = page->d->updated.toString(Qt::ISODate);
    }
    map[QStringLiteral("url")] = page->d->url.toString();
    map[QStringLiteral("title")] = page->d->title;
    map[QStringLiteral("content")] = page->d->content;
    switch (page->d->status) {
    case Blogger::Page::Draft:
        map[QStringLiteral("status")] = QLatin1String("DRAFT");
        break;
    case Blogger::Page::Live:
        map[QStringLiteral("status")] = QLatin1String("LIVE");
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
    if (map[QStringLiteral("kind")].toString() != QLatin1String("blogger#page")) {
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
    if (map[QStringLiteral("kind")].toString() != QLatin1String("blogger#pageList")) {
        return ObjectsList();
    }

    ObjectsList list;
    const QVariantList variantList = map[QStringLiteral("items")].toList();
    list.reserve(variantList.size());
    for (const QVariant &item : variantList) {
        list << Private::fromJSON(item);
    }
    return list;
}

QByteArray Page::toJSON(const PagePtr &page)
{
    QJsonDocument document = QJsonDocument::fromVariant(Private::toJSON(page));
    return document.toJson(QJsonDocument::Compact);
}
