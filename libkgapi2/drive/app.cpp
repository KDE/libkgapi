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

using namespace KGAPI2;


class DriveApp::Icon::Private
{
  public:
    Private();
    Private(const Private &other);

    QString category;
    int size;
    QString iconUrl;
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

QString DriveApp::Icon::category() const
{
    return d->category;
}

void DriveApp::Icon::setCategory(const QString &category)
{
    d->category = category;
}

int DriveApp::Icon::size() const
{
    return d->size;
}

void DriveApp::Icon::setSize(const int &size)
{
    d->size = size;
}

QString DriveApp::Icon::iconUrl() const
{
    return d->iconUrl;
}

void DriveApp::Icon::setIconUrl(const QString &iconUrl)
{
    d->iconUrl = iconUrl;
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
    QString productUrl;
    QStringList primaryMimeTypes;
    QStringList secondaryMimeTypes;
    QStringList primaryFileExtensions;
    QStringList secondaryFileExtensions;
    IconsList icons;
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

DriveApp::DriveApp():
    d(new Private)
{
}

DriveApp::DriveApp(const DriveApp &other):
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

void DriveApp::setId(const QString &id)
{
    d->id = id;
}

QString DriveApp::name() const
{
    return d->name;
}

void DriveApp::setName(const QString &name)
{
    d->name = name;
}

QString DriveApp::objectType() const
{
    return d->objectType;
}

void DriveApp::setObjectType(const QString &objectType)
{
    d->objectType = objectType;
}

bool DriveApp::supportsCreate() const
{
    return d->supportsCreate;
}

void DriveApp::setSupportsCreate(bool supportsCreate)
{
    d->supportsCreate = supportsCreate;
}

bool DriveApp::supportsImport() const
{
    return d->supportsImport;
}

void DriveApp::setSupportsImport(bool supportsImport)
{
    d->supportsImport = supportsImport;
}

bool DriveApp::installed() const
{
    return d->installed;
}

void DriveApp::setInstalled(bool installed)
{
    d->installed = installed;
}

bool DriveApp::authorized() const
{
    return d->authorized;
}

void DriveApp::setAuthorized(bool authorized)
{
    d->authorized = authorized;
}

bool DriveApp::useByDefault() const
{
    return d->useByDefault;
}

void DriveApp::setUseByDefault(bool useByDefault)
{
    d->useByDefault = useByDefault;
}

QString DriveApp::productUrl() const
{
    return d->productUrl;
}

void DriveApp::setProductUrl(const QString &productUrl)
{
    d->productUrl = productUrl;
}

QStringList DriveApp::primaryMimeTypes() const
{
    return d->primaryMimeTypes;
}

void DriveApp::setPrimaryMimeTypes(const QStringList &primaryMimeTypes)
{
    d->primaryMimeTypes = primaryMimeTypes;
}

QStringList DriveApp::secondaryMimeTypes() const
{
    return d->secondaryMimeTypes;
}

void DriveApp::setSecondaryMimeTypes(const QStringList &secondaryMimeTypes)
{
    d->secondaryMimeTypes = secondaryMimeTypes;
}

QStringList DriveApp::primaryFileExtensions() const
{
    return d->primaryFileExtensions;
}

void DriveApp::setPrimaryFileExtensions(const QStringList &primaryFileExtensions)
{
    d->primaryFileExtensions = primaryFileExtensions;
}

QStringList DriveApp::secondaryFileExtensions() const
{
    return d->secondaryFileExtensions;
}

void DriveApp::setSecondaryFileExtensions(const QStringList &secondaryFileExtensions)
{
    d->secondaryFileExtensions = secondaryFileExtensions;
}

DriveApp::IconsList DriveApp::icons() const
{
    return d->icons;
}

void DriveApp::setIcons(const IconsList &icons)
{
    d->icons = icons;
}
