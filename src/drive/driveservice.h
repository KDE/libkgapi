/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBKGAPI2_DRIVESERVICE_H
#define LIBKGAPI2_DRIVESERVICE_H

#include "types.h"
#include "kgapidrive_export.h"

#include <QUrl>

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
    KGAPIDRIVE_EXPORT QUrl fetchAboutUrl(bool includeSubscribed,
                                        qlonglong maxChangeIdCount,
                                        qlonglong startChangeId);

    KGAPIDRIVE_EXPORT QUrl fetchAppUrl(const QString &appId);

    KGAPIDRIVE_EXPORT QUrl fetchAppsUrl();

    KGAPIDRIVE_EXPORT QUrl fetchChildReference(const QString &folderId,
                                              const QString &referenceId);

    KGAPIDRIVE_EXPORT QUrl fetchChildReferences(const QString &folderId);

    KGAPIDRIVE_EXPORT QUrl createChildReference(const QString &folderId);

    KGAPIDRIVE_EXPORT QUrl deleteChildReference(const QString &folderId,
                                               const QString &referenceId);

    KGAPIDRIVE_EXPORT QUrl fetchChangeUrl(const QString &changeId);

    KGAPIDRIVE_EXPORT QUrl fetchChangesUrl();

    KGAPIDRIVE_EXPORT QUrl copyFileUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl deleteFileUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl fetchFileUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl fetchFilesUrl();

    KGAPIDRIVE_EXPORT QUrl touchFileUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl trashFileUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl untrashFileUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl uploadMetadataFileUrl(const QString &fileId = QString());

    KGAPIDRIVE_EXPORT QUrl uploadMediaFileUrl(const QString &fileId = QString());

    KGAPIDRIVE_EXPORT QUrl uploadMultipartFileUrl(const QString &fileId = QString());

    KGAPIDRIVE_EXPORT QUrl fetchParentReferenceUrl(const QString &fileId,
                                                  const QString &referenceId);

    KGAPIDRIVE_EXPORT QUrl fetchParentReferencesUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl createParentReferenceUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl deleteParentReferenceUrl(const QString &fileId,
                                                   const QString &referenceId);

    KGAPIDRIVE_EXPORT QUrl fetchPermissionsUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl fetchPermissionUrl(const QString &fileId,
                                             const QString &permissionId);

    KGAPIDRIVE_EXPORT QUrl createPermissionUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl deletePermissionUrl(const QString &fileId,
                                              const QString &permissionId);
    KGAPIDRIVE_EXPORT QUrl modifyPermissionUrl(const QString &fileId,
                                              const QString &permissionId);

    KGAPIDRIVE_EXPORT QUrl fetchRevisionUrl(const QString &fileId,
                                           const QString &revisionId);

    KGAPIDRIVE_EXPORT QUrl fetchRevisionsUrl(const QString &fileId);

    KGAPIDRIVE_EXPORT QUrl deleteRevisionUrl(const QString &fileId,
                                            const QString &revisionId);

    KGAPIDRIVE_EXPORT QUrl modifyRevisionUrl(const QString &fileId,
                                            const QString &revisionId);

} // namespace DriveService

} // namespace KGAPI2

#endif // KGAPI2_DRIVESERVICE_H
