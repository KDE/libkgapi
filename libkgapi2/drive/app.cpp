/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "app.h"

#include <QtCore/QVariantMap>

#include <qjson/parser.h>

using namespace KGAPI2;


class DriveApp::Icon::Private
{
  public:
    Private();
    Private(const Private &other);

    Category category;
    int size;
    QUrl iconUrl;

    static Category categoryFromName(const QString &categoryName);
};

DriveApp::Icon::Private::Private():
    size(-1)
{
}

DriveApp::Icon::Private::Private(const Private &other):
    category(other.category),
    size(other.size),
    iconUrl(other.iconUrl)
{
}

DriveApp::Icon::Category DriveApp::Icon::Private::categoryFromName(const QString &categoryName)
{
    if (categoryName == QLatin1String("application")) {
        return DriveApp::Icon::ApplicationCategory;
    } else if (categoryName == QLatin1String("document")) {
        return DriveApp::Icon::DocumentCategory;
    } else if (categoryName == QLatin1String("documentShared")) {
        return DriveApp::Icon::DocumentSharedCategory;
    } else {
        return DriveApp::Icon::UndefinedCategory;
    }
}


DriveApp::Icon::Icon():
    d(new Private)
{
}

DriveApp::Icon::Icon(const DriveApp::Icon &other):
    d(new Private(*(other.d)))
{
}

DriveApp::Icon::~Icon()
{
    delete d;
}

DriveApp::Icon::Category DriveApp::Icon::category() const
{
    return d->category;
}

int DriveApp::Icon::size() const
{
    return d->size;
}

QUrl DriveApp::Icon::iconUrl() const
{
    return d->iconUrl;
}


///// DriveApp

class DriveApp::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QString name;
    QString objectType;
    bool supportsCreate;
    bool supportsImport;
    bool installed;
    bool authorized;
    bool useByDefault;
    QUrl productUrl;
    QStringList primaryMimeTypes;
    QStringList secondaryMimeTypes;
    QStringList primaryFileExtensions;
    QStringList secondaryFileExtensions;
    IconsList icons;

    static DriveAppPtr fromJSON(const QVariantMap &map);
};

DriveApp::Private::Private():
    supportsCreate(false),
    supportsImport(false),
    installed(false),
    authorized(false),
    useByDefault(false)
{
}

DriveApp::Private::Private(const DriveApp::Private &other):
    id(other.id),
    name(other.name),
    objectType(other.objectType),
    supportsCreate(other.supportsCreate),
    supportsImport(other.supportsImport),
    installed(other.installed),
    authorized(other.authorized),
    useByDefault(other.useByDefault),
    productUrl(other.productUrl),
    primaryMimeTypes(other.primaryMimeTypes),
    secondaryMimeTypes(other.secondaryMimeTypes),
    primaryFileExtensions(other.primaryFileExtensions),
    secondaryFileExtensions(other.secondaryFileExtensions),
    icons(other.icons)
{
}

DriveAppPtr DriveApp::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("kind#app"))
    {
        return DriveAppPtr();
    }

    DriveAppPtr app(new DriveApp);
    app->setEtag(map[QLatin1String("etag")].toString());
    app->d->id = map[QLatin1String("id")].toString();
    app->d->name = map[QLatin1String("map")].toString();
    app->d->objectType = map[QLatin1String("objectType")].toString();
    app->d->supportsCreate = map[QLatin1String("supportsCreate")].toBool();
    app->d->supportsImport = map[QLatin1String("supportsImport")].toBool();
    app->d->installed = map[QLatin1String("installed")].toBool();
    app->d->authorized = map[QLatin1String("authorized")].toBool();
    app->d->useByDefault = map[QLatin1String("useByDefault")].toBool();
    app->d->productUrl = map[QLatin1String("productUrl")].toUrl();
    app->d->primaryMimeTypes = map[QLatin1String("primaryMimeTypes")].toStringList();
    app->d->secondaryMimeTypes = map[QLatin1String("secondaryMimeTypes")].toStringList();
    app->d->primaryFileExtensions = map[QLatin1String("primaryFileExtensions")].toStringList();
    app->d->secondaryFileExtensions = map[QLatin1String("secondaryFileExtensions")].toStringList();

    const QVariantList icons = map[QLatin1String("icons")].toList();
    Q_FOREACH (const QVariant &i, icons) {
        const QVariantMap &iconData = i.toMap();

        IconPtr icon(new Icon());
        icon->d->category = Icon::Private::categoryFromName(iconData[QLatin1String("category")].toString());
        icon->d->size = iconData[QLatin1String("size")].toInt();
        icon->d->iconUrl = iconData[QLatin1String("iconUrl")].toUrl();

        app->d->icons << icon;
    }

    return app;
}

DriveApp::DriveApp():
    KGAPI2::Object(),
    d(new Private)
{
}

DriveApp::DriveApp(const DriveApp &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

DriveApp::~DriveApp()
{
    delete d;
}

QString DriveApp::id() const
{
    return d->id;
}

QString DriveApp::name() const
{
    return d->name;
}

QString DriveApp::objectType() const
{
    return d->objectType;
}

bool DriveApp::supportsCreate() const
{
    return d->supportsCreate;
}

bool DriveApp::supportsImport() const
{
    return d->supportsImport;
}

bool DriveApp::installed() const
{
    return d->installed;
}

bool DriveApp::authorized() const
{
    return d->authorized;
}

bool DriveApp::useByDefault() const
{
    return d->useByDefault;
}

QUrl DriveApp::productUrl() const
{
    return d->productUrl;
}

QStringList DriveApp::primaryMimeTypes() const
{
    return d->primaryMimeTypes;
}

QStringList DriveApp::secondaryMimeTypes() const
{
    return d->secondaryMimeTypes;
}

QStringList DriveApp::primaryFileExtensions() const
{
    return d->primaryFileExtensions;
}

QStringList DriveApp::secondaryFileExtensions() const
{
    return d->secondaryFileExtensions;
}


DriveApp::IconsList DriveApp::icons() const
{
    return d->icons;
}

DriveAppPtr DriveApp::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);
    if (!ok) {
        return DriveAppPtr();
    }

    return Private::fromJSON(data.toMap());
}

DriveAppsList DriveApp::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);
    if (!ok) {
        return DriveAppsList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#appList"))
    {
        return DriveAppsList();
    }

    DriveAppsList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const DriveAppPtr app = Private::fromJSON(item.toMap());

        if (!app.isNull()) {
            list << app;
        }
    }

    return list;
}
