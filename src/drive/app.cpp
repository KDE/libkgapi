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

#include <QVariantMap>
#include <QJsonDocument>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class App::Icon::Private
{
  public:
    Private();
    Private(const Private &other);

    Category category;
    int size;
    QUrl iconUrl;

    static Category categoryFromName(const QString &categoryName);
};

App::Icon::Private::Private():
    size(-1)
{
}

App::Icon::Private::Private(const Private &other):
    category(other.category),
    size(other.size),
    iconUrl(other.iconUrl)
{
}

App::Icon::Category App::Icon::Private::categoryFromName(const QString &categoryName)
{
    if (categoryName == QLatin1String("application")) {
        return App::Icon::ApplicationCategory;
    } else if (categoryName == QLatin1String("document")) {
        return App::Icon::DocumentCategory;
    } else if (categoryName == QLatin1String("documentShared")) {
        return App::Icon::DocumentSharedCategory;
    } else {
        return App::Icon::UndefinedCategory;
    }
}


App::Icon::Icon():
    d(new Private)
{
}

App::Icon::Icon(const App::Icon &other):
    d(new Private(*(other.d)))
{
}

App::Icon::~Icon()
{
    delete d;
}

App::Icon::Category App::Icon::category() const
{
    return d->category;
}

int App::Icon::size() const
{
    return d->size;
}

QUrl App::Icon::iconUrl() const
{
    return d->iconUrl;
}


///// DriveApp

class App::Private
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

    static AppPtr fromJSON(const QVariantMap &map);
};

App::Private::Private():
    supportsCreate(false),
    supportsImport(false),
    installed(false),
    authorized(false),
    useByDefault(false)
{
}

App::Private::Private(const App::Private &other):
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

AppPtr App::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("kind#app")) {
        return AppPtr();
    }

    AppPtr app(new App);
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
    Q_FOREACH(const QVariant & i, icons) {
        const QVariantMap &iconData = i.toMap();

        IconPtr icon(new Icon());
        icon->d->category = Icon::Private::categoryFromName(iconData[QLatin1String("category")].toString());
        icon->d->size = iconData[QLatin1String("size")].toInt();
        icon->d->iconUrl = iconData[QLatin1String("iconUrl")].toUrl();

        app->d->icons << icon;
    }

    return app;
}

App::App():
    KGAPI2::Object(),
    d(new Private)
{
}

App::App(const App &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

App::~App()
{
    delete d;
}

QString App::id() const
{
    return d->id;
}

QString App::name() const
{
    return d->name;
}

QString App::objectType() const
{
    return d->objectType;
}

bool App::supportsCreate() const
{
    return d->supportsCreate;
}

bool App::supportsImport() const
{
    return d->supportsImport;
}

bool App::installed() const
{
    return d->installed;
}

bool App::authorized() const
{
    return d->authorized;
}

bool App::useByDefault() const
{
    return d->useByDefault;
}

QUrl App::productUrl() const
{
    return d->productUrl;
}

QStringList App::primaryMimeTypes() const
{
    return d->primaryMimeTypes;
}

QStringList App::secondaryMimeTypes() const
{
    return d->secondaryMimeTypes;
}

QStringList App::primaryFileExtensions() const
{
    return d->primaryFileExtensions;
}

QStringList App::secondaryFileExtensions() const
{
    return d->secondaryFileExtensions;
}


App::IconsList App::icons() const
{
    return d->icons;
}

AppPtr App::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return AppPtr();
    }
    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

AppsList App::fromJSONFeed(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return AppsList();
    }
    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("drive#appList")) {
        return AppsList();
    }

    AppsList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH(const QVariant & item, items) {
        const AppPtr app = Private::fromJSON(item.toMap());

        if (!app.isNull()) {
            list << app;
        }
    }

    return list;
}
