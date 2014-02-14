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

#include "permission.h"
#include "permission_p.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

Permission::Role Permission::Private::roleFromName(const QString &roleName)
{
    if (roleName == QLatin1String("owner")) {
        return Permission::OwnerRole;
    } else if (roleName == QLatin1String("reader")) {
        return Permission::ReaderRole;
    } else if (roleName == QLatin1String("writer")) {
        return Permission::WriterRole;
    } else if (roleName == QLatin1String("commenter")) {
        return Permission::CommenterRole;
    } else {
        return Permission::UndefinedRole;
    }
}

Permission::Type Permission::Private::typeFromName(const QString &typeName)
{
    if (typeName == QLatin1String("user")) {
        return Permission::TypeUser;
    } else if (typeName == QLatin1String("group")) {
        return Permission::TypeGroup;
    } else if (typeName == QLatin1String("domain")) {
        return Permission::TypeDomain;
    } else if (typeName == QLatin1String("anyone")) {
        return Permission::TypeAnyone;
    } else {
        return Permission::UndefinedType;
    }
}

QString Permission::Private::roleToName(Permission::Role role)
{
    switch (role) {
    case Permission::OwnerRole:
        return QLatin1String("owner");
    case Permission::ReaderRole:
        return QLatin1String("reader");
    case Permission::WriterRole:
        return QLatin1String("writer");
    case Permission::CommenterRole:
        return QLatin1String("commented");
    default:
        return QString();
    }
}

QString Permission::Private::typeToName(Permission::Type type)
{
    switch (type) {
    case Permission::TypeUser:
        return QLatin1String("user");
    case Permission::TypeGroup:
        return QLatin1String("group");
    case Permission::TypeDomain:
        return QLatin1String("domain");
    case Permission::TypeAnyone:
        return QLatin1String("anyone");
    default:
        return QString();
    }
}

PermissionPtr Permission::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("drive#permission")) {
        return PermissionPtr();
    }

    PermissionPtr permission(new Permission());
    permission->setEtag(map[QLatin1String("etag")].toString());
    permission->d->id = map[QLatin1String("id")].toString();
    permission->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    permission->d->name = map[QLatin1String("name")].toString();

    permission->d->role = Private::roleFromName(map[QLatin1String("role")].toString());

    const QStringList additionalRoles = map[QLatin1String("additionalRoles")].toStringList();
    Q_FOREACH(const QString & additionalRole, additionalRoles) {
        permission->d->additionalRoles << Private::roleFromName(additionalRole);
    }

    permission->d->type = Private::typeFromName(map[QLatin1String("type")].toString());
    permission->d->authKey = map[QLatin1String("authKey")].toString();
    permission->d->withLink = map[QLatin1String("withLink")].toBool();
    permission->d->photoLink = map[QLatin1String("photoLink")].toUrl();
    permission->d->value = map[QLatin1String("value")].toString();

    return permission;
}


Permission::Private::Private():
    role(Permission::UndefinedRole),
    type(Permission::UndefinedType),
    withLink(false)
{
}

Permission::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    name(other.name),
    role(other.role),
    additionalRoles(other.additionalRoles),
    type(other.type),
    authKey(other.authKey),
    withLink(other.withLink),
    photoLink(other.photoLink),
    value(other.value)
{
}

Permission::Permission():
    KGAPI2::Object(),
    d(new Private)
{
}

Permission::Permission(const Permission &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{ }

Permission::~Permission()
{
    delete d;
}

QString Permission::id() const
{
    return d->id;
}

void Permission::setId(const QString &id)
{
    d->id = id;
}

QUrl Permission::selfLink() const
{
    return d->selfLink;
}

QString Permission::name() const
{
    return d->name;
}

Permission::Role Permission::role() const
{
    return d->role;
}

void Permission::setRole(Permission::Role role)
{
    d->role = role;
}

QList<Permission::Role> Permission::additionalRoles() const
{
    return d->additionalRoles;
}

void Permission::setAdditionalRoles(const QList<Permission::Role> &additionalRoles)
{
    d->additionalRoles = additionalRoles;
}

Permission::Type Permission::type() const
{
    return d->type;
}

void Permission::setType(Permission::Type type)
{
    d->type = type;
}

QString Permission::authKey() const
{
    return d->authKey;
}

bool Permission::withLink() const
{
    return d->withLink;
}

void Permission::setWithLink(bool withLink)
{
    d->withLink = withLink;
}

QUrl Permission::photoLink() const
{
    return d->photoLink;
}

QString Permission::value() const
{
    return d->value;
}

void Permission::setValue(const QString &value)
{
    d->value = value;
}

PermissionPtr Permission::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant json = parser.parse(jsonData, &ok);
    if (!ok) {
        return PermissionPtr();
    }
    const QVariantMap map = json.toMap();

    return Private::fromJSON(map);
}

PermissionsList Permission::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant json = parser.parse(jsonData, &ok);
    if (!ok) {
        return PermissionsList();
    }
    const QVariantMap map = json.toMap();
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("drive#permissionList")) {
        return PermissionsList();
    }

    PermissionsList permissions;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH(const QVariant & item, items) {
        const PermissionPtr permission = Private::fromJSON(item.toMap());
        if (!permission.isNull()) {
            permissions << permission;
        }
    }
    return permissions;
}

QByteArray Permission::toJSON(const PermissionPtr &permission)
{
    QVariantMap map;

    if (permission->role() != Permission::UndefinedRole) {
        map[QLatin1String("role")] = Private::roleToName(permission->role());
    }
    if (permission->type() != Permission::UndefinedType) {
        map[QLatin1String("type")] = Private::typeToName(permission->type());
    }

    QVariantList additionalRoles;
    Q_FOREACH(Permission::Role additionalRole, permission->additionalRoles()) {  // krazy:exclude=foreach it's POD
        additionalRoles << Private::roleToName(additionalRole);
    }
    if (!additionalRoles.isEmpty()) {
        map[QLatin1String("additionalRoles")] = additionalRoles;
    }

    map[QLatin1String("withLink")] = permission->withLink();

    if (!permission->value().isEmpty()) {
        map[QLatin1String("value")] = permission->value();
    }

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
