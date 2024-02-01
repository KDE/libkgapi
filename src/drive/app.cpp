/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "app.h"
#include "utils_p.h"

#include <QJsonDocument>
#include <QVariantMap>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN App::Icon::Private
{
public:
    Private();
    Private(const Private &other);

    Category category;
    int size = -1;
    QUrl iconUrl;

    static Category categoryFromName(const QString &categoryName);
};

App::Icon::Private::Private()
{
}

App::Icon::Private::Private(const Private &other)
    : category(other.category)
    , size(other.size)
    , iconUrl(other.iconUrl)
{
}

App::Icon::Category App::Icon::Private::categoryFromName(const QString &categoryName)
{
    if (categoryName == QLatin1StringView("application")) {
        return App::Icon::ApplicationCategory;
    } else if (categoryName == QLatin1StringView("document")) {
        return App::Icon::DocumentCategory;
    } else if (categoryName == QLatin1StringView("documentShared")) {
        return App::Icon::DocumentSharedCategory;
    } else {
        return App::Icon::UndefinedCategory;
    }
}

App::Icon::Icon()
    : d(new Private)
{
}

App::Icon::Icon(const App::Icon &other)
    : d(new Private(*(other.d)))
{
}

App::Icon::~Icon()
{
    delete d;
}

bool App::Icon::operator==(const Icon &other) const
{
    GAPI_COMPARE(category)
    GAPI_COMPARE(size)
    GAPI_COMPARE(iconUrl)
    return true;
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

class Q_DECL_HIDDEN App::Private
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

App::Private::Private()
    : supportsCreate(false)
    , supportsImport(false)
    , installed(false)
    , authorized(false)
    , useByDefault(false)
{
}

App::Private::Private(const App::Private &other)
    : id(other.id)
    , name(other.name)
    , objectType(other.objectType)
    , supportsCreate(other.supportsCreate)
    , supportsImport(other.supportsImport)
    , installed(other.installed)
    , authorized(other.authorized)
    , useByDefault(other.useByDefault)
    , productUrl(other.productUrl)
    , primaryMimeTypes(other.primaryMimeTypes)
    , secondaryMimeTypes(other.secondaryMimeTypes)
    , primaryFileExtensions(other.primaryFileExtensions)
    , secondaryFileExtensions(other.secondaryFileExtensions)
    , icons(other.icons)
{
}

AppPtr App::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#app")) {
        return AppPtr();
    }

    AppPtr app(new App);
    app->setEtag(map[QStringLiteral("etag")].toString());
    app->d->id = map[QStringLiteral("id")].toString();
    app->d->name = map[QStringLiteral("map")].toString();
    app->d->objectType = map[QStringLiteral("objectType")].toString();
    app->d->supportsCreate = map[QStringLiteral("supportsCreate")].toBool();
    app->d->supportsImport = map[QStringLiteral("supportsImport")].toBool();
    app->d->installed = map[QStringLiteral("installed")].toBool();
    app->d->authorized = map[QStringLiteral("authorized")].toBool();
    app->d->useByDefault = map[QStringLiteral("useByDefault")].toBool();
    app->d->productUrl = map[QStringLiteral("productUrl")].toUrl();
    app->d->primaryMimeTypes = map[QStringLiteral("primaryMimeTypes")].toStringList();
    app->d->secondaryMimeTypes = map[QStringLiteral("secondaryMimeTypes")].toStringList();
    app->d->primaryFileExtensions = map[QStringLiteral("primaryFileExtensions")].toStringList();
    app->d->secondaryFileExtensions = map[QStringLiteral("secondaryFileExtensions")].toStringList();

    const QVariantList icons = map[QStringLiteral("icons")].toList();
    for (const QVariant &i : icons) {
        const QVariantMap &iconData = i.toMap();

        IconPtr icon(new Icon());
        icon->d->category = Icon::Private::categoryFromName(iconData[QStringLiteral("category")].toString());
        icon->d->size = iconData[QStringLiteral("size")].toInt();
        icon->d->iconUrl = iconData[QStringLiteral("iconUrl")].toUrl();

        app->d->icons << icon;
    }

    return app;
}

App::App()
    : KGAPI2::Object()
    , d(new Private)
{
}

App::App(const App &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

App::~App()
{
    delete d;
}

bool App::operator==(const App &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }

    GAPI_COMPARE(id)
    GAPI_COMPARE(name)
    GAPI_COMPARE(objectType)
    GAPI_COMPARE(supportsCreate)
    GAPI_COMPARE(supportsImport)
    GAPI_COMPARE(installed)
    GAPI_COMPARE(authorized)
    GAPI_COMPARE(useByDefault)
    GAPI_COMPARE(productUrl)
    GAPI_COMPARE(primaryMimeTypes)
    GAPI_COMPARE(secondaryMimeTypes)
    GAPI_COMPARE(primaryFileExtensions)
    GAPI_COMPARE(secondaryFileExtensions)
    GAPI_COMPARE_CONTAINERS(icons)
    return true;
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
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#appList")) {
        return AppsList();
    }

    AppsList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : items) {
        const AppPtr app = Private::fromJSON(item.toMap());

        if (!app.isNull()) {
            list << app;
        }
    }

    return list;
}
