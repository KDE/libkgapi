/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "permission.h"
#include "permission_p.h"

#include <QJsonDocument>

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
        return QStringLiteral("owner");
    case Permission::ReaderRole:
        return QStringLiteral("reader");
    case Permission::WriterRole:
        return QStringLiteral("writer");
    case Permission::CommenterRole:
        return QStringLiteral("commented");
    default:
        return QString();
    }
}

QString Permission::Private::typeToName(Permission::Type type)
{
    switch (type) {
    case Permission::TypeUser:
        return QStringLiteral("user");
    case Permission::TypeGroup:
        return QStringLiteral("group");
    case Permission::TypeDomain:
        return QStringLiteral("domain");
    case Permission::TypeAnyone:
        return QStringLiteral("anyone");
    default:
        return QString();
    }
}

PermissionPtr Permission::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#permission")) {
        return PermissionPtr();
    }

    PermissionPtr permission(new Permission());
    permission->setEtag(map[QStringLiteral("etag")].toString());
    permission->d->id = map[QStringLiteral("id")].toString();
    permission->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    permission->d->name = map[QStringLiteral("name")].toString();

    permission->d->role = Private::roleFromName(map[QStringLiteral("role")].toString());

    const QStringList additionalRoles = map[QStringLiteral("additionalRoles")].toStringList();
    for (const QString & additionalRole : additionalRoles) {
        permission->d->additionalRoles << Private::roleFromName(additionalRole);
    }

    permission->d->type = Private::typeFromName(map[QStringLiteral("type")].toString());
    permission->d->authKey = map[QStringLiteral("authKey")].toString();
    permission->d->withLink = map[QStringLiteral("withLink")].toBool();
    permission->d->photoLink = map[QStringLiteral("photoLink")].toUrl();
    permission->d->value = map[QStringLiteral("value")].toString();

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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return PermissionPtr();
    }
    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();

    return Private::fromJSON(map);
}

PermissionsList Permission::fromJSONFeed(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return PermissionsList();
    }
    const QVariant json = document.toVariant();
    const QVariantMap map = json.toMap();
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#permissionList")) {
        return PermissionsList();
    }

    PermissionsList permissions;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant & item : items) {
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
        map[QStringLiteral("role")] = Private::roleToName(permission->role());
    }
    if (permission->type() != Permission::UndefinedType) {
        map[QStringLiteral("type")] = Private::typeToName(permission->type());
    }

    QVariantList additionalRoles;
    const auto roles = permission->additionalRoles();
    additionalRoles.reserve(roles.size());
    for (Permission::Role additionalRole : roles) {
        additionalRoles << Private::roleToName(additionalRole);
    }
    if (!additionalRoles.isEmpty()) {
        map[QStringLiteral("additionalRoles")] = additionalRoles;
    }

    map[QStringLiteral("withLink")] = permission->withLink();

    if (!permission->value().isEmpty()) {
        map[QStringLiteral("value")] = permission->value();
    }

    QJsonDocument document = QJsonDocument::fromVariant(map);
    return document.toJson(QJsonDocument::Compact);
}
