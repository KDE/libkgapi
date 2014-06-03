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

namespace DriveService
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

    LIBKGAPI2_EXPORT QUrl fetchChildReference(const QString &folderId,
                                              const QString &referenceId);

    LIBKGAPI2_EXPORT QUrl fetchChildReferences(const QString &folderId);

    LIBKGAPI2_EXPORT QUrl createChildReference(const QString &folderId);

    LIBKGAPI2_EXPORT QUrl deleteChildReference(const QString &folderId,
                                               const QString &referenceId);

    LIBKGAPI2_EXPORT QUrl fetchChangeUrl(const QString &changeId);

    LIBKGAPI2_EXPORT QUrl fetchChangesUrl();

    LIBKGAPI2_EXPORT QUrl copyFileUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl deleteFileUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl fetchFileUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl fetchFilesUrl();

    LIBKGAPI2_EXPORT QUrl touchFileUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl trashFileUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl untrashFileUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl uploadMetadataFileUrl();

    LIBKGAPI2_EXPORT QUrl uploadMediaFileUrl(const QString &fileId = QString());

    LIBKGAPI2_EXPORT QUrl uploadMultipartFileUrl(const QString &fileId = QString());

    LIBKGAPI2_EXPORT QUrl fetchParentReferenceUrl(const QString &fileId,
                                                  const QString &referenceId);

    LIBKGAPI2_EXPORT QUrl fetchParentReferencesUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl createParentReferenceUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl deleteParentReferenceUrl(const QString &fileId,
                                                   const QString &referenceId);

    LIBKGAPI2_EXPORT QUrl fetchPermissionsUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl fetchPermissionUrl(const QString &fileId,
                                             const QString &permissionId);

    LIBKGAPI2_EXPORT QUrl createPermissionUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl deletePermissionUrl(const QString &fileId,
                                              const QString &permissionId);
    LIBKGAPI2_EXPORT QUrl modifyPermissionUrl(const QString &fileId,
                                              const QString &permissionId);

    LIBKGAPI2_EXPORT QUrl fetchRevisionUrl(const QString &fileId,
                                           const QString &revisionId);

    LIBKGAPI2_EXPORT QUrl fetchRevisionsUrl(const QString &fileId);

    LIBKGAPI2_EXPORT QUrl deleteRevisionUrl(const QString &fileId,
                                            const QString &revisionId);

    LIBKGAPI2_EXPORT QUrl modifyRevisionUrl(const QString &fileId,
                                            const QString &revisionId);

} // namespace DriveService

} // namespace KGAPI2

#endif // KGAPI2_DRIVESERVICE_H
