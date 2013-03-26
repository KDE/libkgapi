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

#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGAPI2;

class DrivePermission::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QUrl selfLink;
    QString name;
    Role role;
    QList<Role> additionalRoles;
    Type type;
    QString authKey;
    bool withLink;
    QUrl photoLink;
    QString value;

    static Role roleFromName(const QString &roleName);
    static Type typeFromName(const QString &typeName);
    static QString roleToName(DrivePermission::Role role);
    static QString typeToName(DrivePermission::Type type);
    static DrivePermissionPtr fromJSON(const QVariantMap &map);
};

DrivePermission::Role DrivePermission::Private::roleFromName(const QString &roleName)
{
    if (roleName == QLatin1String("owner")) {
        return DrivePermission::OwnerRole;
    } else if (roleName == QLatin1String("reader")) {
        return DrivePermission::ReaderRole;
    } else if (roleName == QLatin1String("writer")) {
        return DrivePermission::WriterRole;
    } else if (roleName == QLatin1String("commenter")) {
        return DrivePermission::CommenterRole;
    } else {
        return DrivePermission::UndefinedRole;
    }
}

DrivePermission::Type DrivePermission::Private::typeFromName(const QString &typeName)
{
    if (typeName == QLatin1String("user")) {
        return DrivePermission::TypeUser;
    } else if (typeName == QLatin1String("group")) {
        return DrivePermission::TypeGroup;
    } else if (typeName == QLatin1String("domain")) {
        return DrivePermission::TypeDomain;
    } else if (typeName == QLatin1String("anyone")) {
        return DrivePermission::TypeAnyone;
    } else {
        return DrivePermission::UndefinedType;
    }
}

QString DrivePermission::Private::roleToName(DrivePermission::Role role)
{
    switch (role) {
        case DrivePermission::OwnerRole:
            return QLatin1String("owner");
        case DrivePermission::ReaderRole:
            return QLatin1String("reader");
        case DrivePermission::WriterRole:
            return QLatin1String("writer");
        case DrivePermission::CommenterRole:
            return QLatin1String("commented");
        default:
            return QString();
    }
}

QString DrivePermission::Private::typeToName(DrivePermission::Type type)
{
    switch (type) {
        case DrivePermission::TypeUser:
            return QLatin1String("user");
        case DrivePermission::TypeGroup:
            return QLatin1String("group");
        case DrivePermission::TypeDomain:
            return QLatin1String("domain");
        case DrivePermission::TypeAnyone:
            return QLatin1String("anyone");
        default:
            return QString();
    }
}

DrivePermissionPtr DrivePermission::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#permission"))
    {
        return DrivePermissionPtr();
    }

    DrivePermissionPtr permission(new DrivePermission());
    permission->setEtag(map[QLatin1String("etag")].toString());
    permission->d->id = map[QLatin1String("id")].toString();
    permission->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    permission->d->name = map[QLatin1String("name")].toString();

    permission->d->role = Private::roleFromName(map[QLatin1String("role")].toString());

    const QStringList additionalRoles = map[QLatin1String("additionalRoles")].toStringList();
    Q_FOREACH (const QString &additionalRole, additionalRoles) {
        permission->d->additionalRoles << Private::roleFromName(additionalRole);
    }

    permission->d->type = Private::typeFromName(map[QLatin1String("type")].toString());
    permission->d->authKey = map[QLatin1String("authKey")].toString();
    permission->d->withLink = map[QLatin1String("withLink")].toBool();
    permission->d->photoLink = map[QLatin1String("photoLink")].toUrl();
    permission->d->value = map[QLatin1String("value")].toString();

    return permission;
}


DrivePermission::Private::Private():
    role(KGAPI2::DrivePermission::UndefinedRole),
    type(KGAPI2::DrivePermission::UndefinedType),
    withLink(false)
{
}

DrivePermission::Private::Private(const Private &other):
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

DrivePermission::DrivePermission():
    KGAPI2::Object(),
    d(new Private)
{
}

DrivePermission::DrivePermission(const DrivePermission &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{ }

DrivePermission::~DrivePermission()
{
    delete d;
}

QString DrivePermission::id() const
{
    return d->id;
}

void DrivePermission::setId(const QString &id)
{
    d->id = id;
}

QUrl DrivePermission::selfLink() const
{
    return d->selfLink;
}

QString DrivePermission::name() const
{
    return d->name;
}

DrivePermission::Role DrivePermission::role() const
{
    return d->role;
}

void DrivePermission::setRole(DrivePermission::Role role)
{
    d->role = role;
}

QList<DrivePermission::Role> DrivePermission::additionalRoles() const
{
    return d->additionalRoles;
}

void DrivePermission::setAdditionalRoles(const QList<DrivePermission::Role> &additionalRoles)
{
    d->additionalRoles = additionalRoles;
}

DrivePermission::Type DrivePermission::type() const
{
    return d->type;
}

void DrivePermission::setType(DrivePermission::Type type)
{
    d->type = type;
}

QString DrivePermission::authKey() const
{
    return d->authKey;
}

bool DrivePermission::withLink() const
{
    return d->withLink;
}

void DrivePermission::setWithLink(bool withLink)
{
    d->withLink = withLink;
}

QUrl DrivePermission::photoLink() const
{
    return d->photoLink;
}

QString DrivePermission::value() const
{
    return d->value;
}

void DrivePermission::setValue(const QString &value)
{
    d->value = value;
}

DrivePermissionPtr DrivePermission::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant json = parser.parse(jsonData, &ok);
    if (!ok) {
        return DrivePermissionPtr();
    }
    const QVariantMap map = json.toMap();

    return Private::fromJSON(map);
}

DrivePermissionsList DrivePermission::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant json = parser.parse(jsonData, &ok);
    if (!ok) {
        return DrivePermissionsList();
    }
    const QVariantMap map = json.toMap();
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#permissionList"))
    {
        return DrivePermissionsList();
    }

    DrivePermissionsList permissions;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items)
    {
        const DrivePermissionPtr permission = Private::fromJSON(item.toMap());
        if (!permission.isNull()) {
            permissions << permission;
        }
    }
    return permissions;
}

QByteArray DrivePermission::toJSON(const DrivePermissionPtr &permission)
{
    QVariantMap map;

    if (!permission->role() == DrivePermission::UndefinedRole) {
        map[QLatin1String("role")] = Private::roleToName(permission->role());
    }
    if (!permission->type() == DrivePermission::UndefinedType) {
        map[QLatin1String("type")] = Private::typeToName(permission->type());
    }

    QVariantList additionalRoles;
    Q_FOREACH (DrivePermission::Role additionalRole, permission->additionalRoles()) {
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

ObjectsList operator<<(ObjectsList &objectsList, const DrivePermissionsList &permissionsList)
{
    Q_FOREACH (const DrivePermissionPtr &permission, permissionsList) {
        objectsList << permission.dynamicCast<Object>();
    }

    return objectsList;
}
