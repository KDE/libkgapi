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

#ifndef LIBKGAPI2_DRIVEPERMISSION_P_H
#define LIBKGAPI2_DRIVEPERMISSION_P_H

#include "permission.h"

namespace KGAPI2
{

namespace Drive
{

class Permission::Private
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
};

} // namespace Drive

} // namespace KGAPI2

#endif //LIBKGAPI2_DRIVEPERMISSION_P_H
