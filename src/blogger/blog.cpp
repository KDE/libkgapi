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

#include <qjson/parser.h>

using namespace KGAPI2::Blogger;

class Blog::Private
{
  public:
    Private();

    static BlogPtr fromJSON(const QVariant &json);

    QString id;
    QString name;
    QString description;
    KDateTime published;
    KDateTime updated;
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

KDateTime Blog::published() const
{
    return d->published;
}

KDateTime Blog::updated() const
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
    blog->d->id = map[QLatin1String("id")].toString();
    blog->d->name = map[QLatin1String("name")].toString();
    blog->d->description = map[QLatin1String("description")].toString();
    blog->d->published = KDateTime::fromString(map[QLatin1String("published")].toString(), KDateTime::RFC3339Date);
    blog->d->updated = KDateTime::fromString(map[QLatin1String("updated")].toString(), KDateTime::KDateTime::RFC3339Date);
    blog->d->url = map[QLatin1String("url")].toUrl();
    blog->d->postsCount = map[QLatin1String("posts")].toMap()[QLatin1String("totalItems")].toUInt();
    blog->d->pagesCount = map[QLatin1String("pages")].toMap()[QLatin1String("totalItems")].toUInt();
    const QVariantMap locale = map[QLatin1String("locale")].toMap();
    blog->d->language = locale[QLatin1String("language")].toString();
    blog->d->country = locale[QLatin1String("country")].toString();
    blog->d->languageVariant = locale[QLatin1String("variant")].toString();
    const QString metadata = map[QLatin1String("customMetaData")].toString();
    QJson::Parser parser;
    blog->d->customMetaData = parser.parse(metadata.toUtf8());

    return blog;
}


BlogPtr Blog::fromJSON(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap json = parser.parse(rawData, &ok).toMap();
    if (!ok || json[QLatin1String("kind")].toString() != QLatin1String("blogger#blog")) {
        return BlogPtr();
    }

    return Private::fromJSON(json);
}

BlogsList Blog::fromJSONFeed(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap json = parser.parse(rawData, &ok).toMap();
    if (!ok || json[QLatin1String("kind")].toString() != QLatin1String("blogger#blogList")) {
        return BlogsList();
    }

    BlogsList items;
    const QVariantList blogs = json[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &blog, blogs) {
        items << Blog::Private::fromJSON(blog);
    }
    return items;
}
