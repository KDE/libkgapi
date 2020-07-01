/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LIBKGAPI2_DRIVEPERMISSION_P_H
#define LIBKGAPI2_DRIVEPERMISSION_P_H

#include "permission.h"

namespace KGAPI2
{

namespace Drive
{

class Q_DECL_HIDDEN Permission::Private
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
    QString emailAddress;
    QString domain;
    QDateTime expirationDate;
    bool deleted;
    PermissionDetailsList permissionDetails;

    static Role roleFromName(const QString &roleName);
    static Type typeFromName(const QString &typeName);
    static QString roleToName(Permission::Role role);
    static QString typeToName(Permission::Type type);
    static PermissionPtr fromJSON(const QVariantMap &map);

    friend class File::Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif //LIBKGAPI2_DRIVEPERMISSION_P_H
