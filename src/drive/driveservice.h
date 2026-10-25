/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"

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
QUrl fetchAboutUrl(bool includeSubscribed, qlonglong maxChangeIdCount, qlonglong startChangeId);

QUrl fetchAppUrl(const QString &appId);

QUrl fetchAppsUrl();

QUrl fetchChildReference(const QString &folderId, const QString &referenceId);

QUrl fetchChildReferences(const QString &folderId);

QUrl createChildReference(const QString &folderId);

QUrl deleteChildReference(const QString &folderId, const QString &referenceId);

QUrl fetchChangeUrl(const QString &changeId);

QUrl fetchChangesUrl();

QUrl copyFileUrl(const QString &fileId);

QUrl deleteFileUrl(const QString &fileId);

QUrl fetchFileUrl(const QString &fileId);

QUrl fetchFilesUrl();

QUrl touchFileUrl(const QString &fileId);

QUrl trashFileUrl(const QString &fileId);

QUrl untrashFileUrl(const QString &fileId);

QUrl uploadMetadataFileUrl(const QString &fileId = QString());

QUrl uploadMediaFileUrl(const QString &fileId = QString());

QUrl uploadMultipartFileUrl(const QString &fileId = QString());

QUrl fetchParentReferenceUrl(const QString &fileId, const QString &referenceId);

QUrl fetchParentReferencesUrl(const QString &fileId);

QUrl createParentReferenceUrl(const QString &fileId);

QUrl deleteParentReferenceUrl(const QString &fileId, const QString &referenceId);

QUrl fetchPermissionsUrl(const QString &fileId);

QUrl fetchPermissionUrl(const QString &fileId, const QString &permissionId);

QUrl createPermissionUrl(const QString &fileId);

QUrl deletePermissionUrl(const QString &fileId, const QString &permissionId);
QUrl modifyPermissionUrl(const QString &fileId, const QString &permissionId);

QUrl fetchRevisionUrl(const QString &fileId, const QString &revisionId);

QUrl fetchRevisionsUrl(const QString &fileId);

QUrl deleteRevisionUrl(const QString &fileId, const QString &revisionId);

QUrl modifyRevisionUrl(const QString &fileId, const QString &revisionId);

QUrl fetchDrivesUrl(const QString &drivesId);

QUrl hideDrivesUrl(const QString &drivesId, bool hide);

QUrl fetchDrivesUrl();

QUrl fetchTeamdriveUrl(const QString &teamdriveId);

QUrl fetchTeamdrivesUrl();

} // namespace DriveService

} // namespace KGAPI2
