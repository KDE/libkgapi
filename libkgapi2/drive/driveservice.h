/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_DRIVESERVICE_H
#define LIBKGAPI2_DRIVESERVICE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QUrl>

namespace KGAPI2
{

namespace  DriveService
{

    /**
     * @brief Returns URL for retrieving About data
     *
     * @param includeSubscribed
     * @param maxChangeIdCount
     * @param startChangeId
     */
    LIBKGAPI2_EXPORT QUrl fetchAboutUrl(bool includeSubscribed,
                                        qlonglong maxChangeIdCount,
                                        qlonglong startChangeId);

    LIBKGAPI2_EXPORT QUrl fetchAppUrl(const QString &appId);

    LIBKGAPI2_EXPORT QUrl fetchAppsUrl();

    LIBKGAPI2_EXPORT QUrl fetchPermissionsUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl fetchPermissionUrl(const QString &fileId,
                                             const QString &permissionId);

    LIBKGAPI2_EXPORT QUrl createPermissionUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl deletePermissionUrl(const QString &fileId,
                                              const QString &permissionId);
    LIBKGAPI2_EXPORT QUrl modifyPermissionUrl(const QString &fileId,
                                              const QString &permissionId);

} // namespace DriveService

} // namespace KGAPI2

#endif // KGAPI2_DRIVESERVICE_H
