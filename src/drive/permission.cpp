/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "permission.h"
#include "permission_p.h"
#include "utils_p.h"

#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN Permission::PermissionDetails::Private
{
public:
    Private() = default;
    Private(const Private &other) = default;

    PermissionDetails::PermissionType permissionType = PermissionType::UndefinedType;
    Role role;
    QList<Role> additionalRoles;
    QString inheritedFrom;
    bool inherited = false;

    static PermissionType permissionTypeFromName(const QString &typeName);
    static QString permissionTypeToName(PermissionType permissionType);
};

Permission::PermissionDetails::PermissionDetails()
    : d(new Private)
{
}

Permission::PermissionDetails::PermissionDetails(const Permission::PermissionDetails &other)
    : d(new Private(*(other.d)))
{
}

Permission::PermissionDetails::~PermissionDetails() = default;

bool Permission::PermissionDetails::operator==(const Permission::PermissionDetails &other) const
{
    GAPI_COMPARE(permissionType);
    GAPI_COMPARE(role);
    GAPI_COMPARE(additionalRoles);
    GAPI_COMPARE(inheritedFrom);
    GAPI_COMPARE(inherited);
    return true;
}

QString Permission::PermissionDetails::Private::permissionTypeToName(Permission::PermissionDetails::PermissionType permissionType)
{
    switch (permissionType) {
    case Permission::PermissionDetails::TypeFile:
        return QStringLiteral("file");
    case Permission::PermissionDetails::TypeMember:
        return QStringLiteral("member");
    default:
        return QString();
    }
}

Permission::PermissionDetails::PermissionType Permission::PermissionDetails::Private::permissionTypeFromName(const QString &typeName)
{
    if (typeName == QLatin1String("file")) {
        return Permission::PermissionDetails::TypeFile;
    } else if (typeName == QLatin1String("member")) {
        return Permission::PermissionDetails::TypeMember;
    } else {
        return Permission::PermissionDetails::UndefinedType;
    }
}

Permission::PermissionDetails::PermissionType Permission::PermissionDetails::permissionType() const
{
    return d->permissionType;
}

Permission::Role Permission::PermissionDetails::role() const
{
    return d->role;
}

QList<Permission::Role> Permission::PermissionDetails::additionalRoles() const
{
    return d->additionalRoles;
}

QString Permission::PermissionDetails::inheritedFrom() const
{
    return d->inheritedFrom;
}

bool Permission::PermissionDetails::inherited() const
{
    return d->inherited;
}

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
    } else if (roleName == QLatin1String("organizer")) {
        return Permission::OrganizerRole;
    } else if (roleName == QLatin1String("fileOrganizer")) {
        return Permission::FileOrganizerRole;
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
        return QStringLiteral("commenter");
    case Permission::OrganizerRole:
        return QStringLiteral("organizerRole");
    case Permission::FileOrganizerRole:
        return QStringLiteral("fileOrganizerRole");
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
    if (!map.contains(QLatin1String("kind")) || map[QStringLiteral("kind")].toString() != QLatin1String("drive#permission")) {
        return PermissionPtr();
    }

    PermissionPtr permission(new Permission());
    permission->setEtag(map[QStringLiteral("etag")].toString());
    permission->d->id = map[QStringLiteral("id")].toString();
    permission->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    permission->d->name = map[QStringLiteral("name")].toString();

    permission->d->role = Private::roleFromName(map[QStringLiteral("role")].toString());

    const QStringList additionalRoles = map[QStringLiteral("additionalRoles")].toStringList();
    for (const QString &additionalRole : additionalRoles) {
        permission->d->additionalRoles << Private::roleFromName(additionalRole);
    }

    permission->d->type = Private::typeFromName(map[QStringLiteral("type")].toString());
    permission->d->authKey = map[QStringLiteral("authKey")].toString();
    permission->d->withLink = map[QStringLiteral("withLink")].toBool();
    permission->d->photoLink = map[QStringLiteral("photoLink")].toUrl();
    permission->d->value = map[QStringLiteral("value")].toString();
    permission->d->emailAddress = map[QStringLiteral("emailAddress")].toString();
    permission->d->domain = map[QStringLiteral("domain")].toString();
    permission->d->expirationDate = QDateTime::fromString(map[QStringLiteral("expirationDate")].toString(), Qt::ISODate);
    permission->d->deleted = map[QStringLiteral("deleted")].toBool();

    if (map.contains(QStringLiteral("permissionDetails"))) {
        const QVariantList permissionDetailsList = map[QStringLiteral("permissionDetails")].toList();
        for (const QVariant &variant : permissionDetailsList) {
            const QVariantMap permissionDetailsMap = variant.toMap();
            auto permissionDetails = PermissionDetailsPtr::create();
            permissionDetails->d->permissionType =
                PermissionDetails::Private::permissionTypeFromName(permissionDetailsMap[QStringLiteral("permissionType")].toString());
            permissionDetails->d->role = Private::roleFromName(permissionDetailsMap[QStringLiteral("role")].toString());
            const QStringList permissionDetailsAdditionalRoles = permissionDetailsMap[QStringLiteral("additionalRoles")].toStringList();
            for (const QString &additionalRole : permissionDetailsAdditionalRoles) {
                permissionDetails->d->additionalRoles << Private::roleFromName(additionalRole);
            }
            permissionDetails->d->inheritedFrom = permissionDetailsMap[QStringLiteral("inheritedFrom")].toString();
            permissionDetails->d->inherited = map[QStringLiteral("inherited")].toBool();

            permission->d->permissionDetails << permissionDetails;
        }
    }

    return permission;
}

Permission::Private::Private()
    : role(Permission::UndefinedRole)
    , type(Permission::UndefinedType)
    , withLink(false)
{
}

Permission::Private::Private(const Private &other) = default;

Permission::Permission()
    : KGAPI2::Object()
    , d(new Private)
{
}

Permission::Permission(const Permission &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

Permission::~Permission()
{
    delete d;
}

bool Permission::operator==(const Permission &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }
    GAPI_COMPARE(id)
    GAPI_COMPARE(selfLink)
    GAPI_COMPARE(name)
    GAPI_COMPARE(role)
    GAPI_COMPARE(additionalRoles)
    GAPI_COMPARE(type)
    GAPI_COMPARE(authKey)
    GAPI_COMPARE(withLink)
    GAPI_COMPARE(photoLink)
    GAPI_COMPARE(value)
    GAPI_COMPARE(emailAddress)
    GAPI_COMPARE(domain)
    GAPI_COMPARE(expirationDate)
    GAPI_COMPARE(deleted)
    GAPI_COMPARE(permissionDetails)
    return true;
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

QString Permission::emailAddress() const
{
    return d->emailAddress;
}

QString Permission::domain() const
{
    return d->domain;
}

QDateTime Permission::expirationDate() const
{
    return d->expirationDate;
}

bool Permission::deleted() const
{
    return d->deleted;
}

Permission::PermissionDetailsList Permission::permissionDetails() const
{
    return d->permissionDetails;
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
    if (!map.contains(QLatin1String("kind")) || map[QStringLiteral("kind")].toString() != QLatin1String("drive#permissionList")) {
        return PermissionsList();
    }

    PermissionsList permissions;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : items) {
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
