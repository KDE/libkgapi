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
