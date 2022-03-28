/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "blog.h"

#include <QJsonDocument>

using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN Blog::Private
{
public:
    Private();

    static BlogPtr fromJSON(const QVariant &json);

    QString id;
    QString name;
    QString description;
    QDateTime published;
    QDateTime updated;
    QUrl url;
    uint postsCount = 0;
    uint pagesCount = 0;
    QString language;
    QString languageVariant;
    QString country;
    QVariant customMetaData;
};

Blog::Private::Private()
{
}

Blog::Blog()
    : Object()
    , d(new Private)
{
}

Blog::~Blog()
{
    delete d;
}

QString Blog::id() const
{
    return d->id;
}

QString Blog::name() const
{
    return d->name;
}

QString Blog::description() const
{
    return d->description;
}

QDateTime Blog::published() const
{
    return d->published;
}

QDateTime Blog::updated() const
{
    return d->updated;
}

QUrl Blog::url() const
{
    return d->url;
}

uint Blog::postsCount() const
{
    return d->postsCount;
}

uint Blog::pagesCount() const
{
    return d->pagesCount;
}

QString Blog::language() const
{
    return d->language;
}

QString Blog::country() const
{
    return d->country;
}

QString Blog::languageVariant() const
{
    return d->languageVariant;
}

QVariant Blog::customMetaData() const
{
    return d->customMetaData;
}

BlogPtr Blog::Private::fromJSON(const QVariant &json)
{
    BlogPtr blog(new Blog);
    const QVariantMap map = json.toMap();
    blog->d->id = map[QStringLiteral("id")].toString();
    blog->d->name = map[QStringLiteral("name")].toString();
    blog->d->description = map[QStringLiteral("description")].toString();
    blog->d->published = QDateTime::fromString(map[QStringLiteral("published")].toString(), Qt::ISODate);
    blog->d->updated = QDateTime::fromString(map[QStringLiteral("updated")].toString(), Qt::ISODate);
    blog->d->url = map[QStringLiteral("url")].toUrl();
    blog->d->postsCount = map[QStringLiteral("posts")].toMap()[QStringLiteral("totalItems")].toUInt();
    blog->d->pagesCount = map[QStringLiteral("pages")].toMap()[QStringLiteral("totalItems")].toUInt();
    const QVariantMap locale = map[QStringLiteral("locale")].toMap();
    blog->d->language = locale[QStringLiteral("language")].toString();
    blog->d->country = locale[QStringLiteral("country")].toString();
    blog->d->languageVariant = locale[QStringLiteral("variant")].toString();
    const QString metadata = map[QStringLiteral("customMetaData")].toString();
    QJsonDocument document = QJsonDocument::fromJson(metadata.toUtf8());
    blog->d->customMetaData = document.toVariant();

    return blog;
}

BlogPtr Blog::fromJSON(const QByteArray &rawData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return BlogPtr();
    }

    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QStringLiteral("kind")].toString() != QLatin1String("blogger#blog")) {
        return BlogPtr();
    }

    return Private::fromJSON(map);
}

BlogsList Blog::fromJSONFeed(const QByteArray &rawData)
{
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        return BlogsList();
    }

    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (map[QStringLiteral("kind")].toString() != QLatin1String("blogger#blogList")) {
        return BlogsList();
    }

    BlogsList items;
    const QVariantList blogs = map[QStringLiteral("items")].toList();
    items.reserve(blogs.size());
    for (const QVariant &blog : blogs) {
        items << Blog::Private::fromJSON(blog);
    }
    return items;
}
