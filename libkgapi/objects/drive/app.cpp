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
#include "app_p.h"

using namespace KGAPI::Objects;

///// DriveAppPrivate

DriveAppPrivate::DriveAppPrivate(const DriveAppPrivate& other):
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
{ }

///// DriveApp::Icon

DriveApp::Icon::Icon()
{ }

DriveApp::Icon::Icon(const DriveApp::Icon& other):
    d(new DriveAppPrivate::Icon(*(other.d)))
{ }

DriveApp::Icon::~Icon()
{
    delete d;
}

QString DriveApp::Icon::category()
{
    return d->category;
}

void DriveApp::Icon::setCategory(const QString& category)
{
    d->category = category;
}

int DriveApp::Icon::size()
{
    return d->size;
}

void DriveApp::Icon::setSize(const int& size)
{
    d->size = size;
}

QString DriveApp::Icon::iconUrl()
{
    return d->iconUrl;
}

void DriveApp::Icon::setIconUrl(const QString& iconUrl)
{
    d->iconUrl = iconUrl;
}

///// DriveApp

DriveApp::DriveApp()
{ }

DriveApp::DriveApp(const DriveApp& other):
    d(new DriveAppPrivate(*(other.d)))
{ }

DriveApp::~DriveApp()
{
    delete d;
}

QString DriveApp::id()
{
    return d->id;
}

void DriveApp::setId(const QString& id)
{
    d->id = id;
}

QString DriveApp::name()
{
    return d->name;
}

void DriveApp::setName(const QString& name)
{
    d->name = name;
}

QString DriveApp::objectType()
{
    return d->objectType;
}

void DriveApp::setObjectType(const QString& objectType)
{
    d->objectType = objectType;
}

bool DriveApp::supportsCreate()
{
    return d->supportsCreate;
}

void DriveApp::setSupportsCreate(const bool& supportsCreate)
{
    d->supportsCreate = supportsCreate;
}

bool DriveApp::supportsImport()
{
    return d->supportsImport;
}

void DriveApp::setSupportsImport(const bool& supportsImport)
{
    d->supportsImport = supportsImport;
}

bool DriveApp::installed()
{
    return d->installed;
}

void DriveApp::setInstalled(const bool& installed)
{
    d->installed = installed;
}

bool DriveApp::authorized()
{
    return d->authorized;
}

void DriveApp::setAuthorized(const bool& authorized)
{
    d->authorized = authorized;
}

bool DriveApp::useByDefault()
{
    return d->useByDefault;
}

void DriveApp::setUseByDefault(const bool& useByDefault)
{
    d->useByDefault = useByDefault;
}

QString DriveApp::productUrl()
{
    return d->productUrl;
}

void DriveApp::setProductUrl(const QString& productUrl)
{
    d->productUrl = productUrl;
}

QStringList DriveApp::primaryMimeTypes()
{
    return d->primaryMimeTypes;
}

void DriveApp::setPrimaryMimeTypes(const QStringList& primaryMimeTypes)
{
    d->primaryMimeTypes = primaryMimeTypes;
}

QStringList DriveApp::secondaryMimeTypes()
{
    return d->secondaryMimeTypes;
}

void DriveApp::setSecondaryMimeTypes(const QStringList& secondaryMimeTypes)
{
    d->secondaryMimeTypes = secondaryMimeTypes;
}

QStringList DriveApp::primaryFileExtensions()
{
    return d->primaryFileExtensions;
}

void DriveApp::setPrimaryFileExtensions(const QStringList& primaryFileExtensions)
{
    d->primaryFileExtensions = primaryFileExtensions;
}

QStringList DriveApp::secondaryFileExtensions()
{
    return d->secondaryFileExtensions;
}

void DriveApp::setSecondaryFileExtensions(const QStringList& secondaryFileExtensions)
{
    d->secondaryFileExtensions = secondaryFileExtensions;
}

QList<DriveApp::Icon> DriveApp::icons()
{
    return d->icons;
}

void DriveApp::setIcons(const QList<DriveApp::Icon>& icons)
{
    d->icons = icons;
}

