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

#include "blog.h"

#include <QJsonDocument>

using namespace KGAPI2::Blogger;

class Blog::Private
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
    uint postsCount;
    uint pagesCount;
    QString language;
    QString languageVariant;
    QString country;
    QVariant customMetaData;
};

Blog::Private::Private()
    : postsCount(0)
    , pagesCount(0)
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

QUrl Blog::url()
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

QString Blog::langauge() const
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
    if (map[QLatin1String("kind")].toString() != QLatin1String("blogger#blog")) {
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
    if (map[QLatin1String("kind")].toString() != QLatin1String("blogger#blogList")) {
        return BlogsList();
    }

    BlogsList items;
    const QVariantList blogs = map[QStringLiteral("items")].toList();
    Q_FOREACH (const QVariant &blog, blogs) {
        items << Blog::Private::fromJSON(blog);
    }
    return items;
}
