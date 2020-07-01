/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    KGAPIDRIVE_EXPORT QUrl fetchDrivesUrl(const QString &drivesId);

    KGAPIDRIVE_EXPORT QUrl hideDrivesUrl(const QString &drivesId, bool hide);

    KGAPIDRIVE_EXPORT QUrl fetchDrivesUrl();

    KGAPIDRIVE_EXPORT QUrl fetchTeamdriveUrl(const QString &teamdriveId);

    KGAPIDRIVE_EXPORT QUrl fetchTeamdrivesUrl();


} // namespace DriveService

} // namespace KGAPI2

#endif // KGAPI2_DRIVESERVICE_H
