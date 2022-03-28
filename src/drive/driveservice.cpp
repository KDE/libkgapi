/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "driveservice.h"
#include "utils.h"

#include <QUrlQuery>

namespace KGAPI2
{

namespace Private
{
static const QUrl GoogleApisUrl(QStringLiteral("https://www.googleapis.com"));
static const QString AppsBasePath(QStringLiteral("/drive/v2/about"));
static const QString FilesBasePath(QStringLiteral("/drive/v2/files"));
static const QString ChangeBasePath(QStringLiteral("/drive/v2/changes"));
static const QString DrivesBasePath(QStringLiteral("/drive/v2/drives"));
static const QString TeamdriveBasePath(QStringLiteral("/drive/v2/teamdrives"));
}

namespace DriveService
{

QUrl fetchAboutUrl(bool includeSubscribed, qlonglong maxChangeIdCount, qlonglong startChangeId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::AppsBasePath);
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("includeSubscribed"), Utils::bool2Str(includeSubscribed));
    if (maxChangeIdCount > 0) {
        query.addQueryItem(QStringLiteral("maxChangeIdCount"), QString::number(maxChangeIdCount));
    }
    if (startChangeId > 0) {
        query.addQueryItem(QStringLiteral("startChangeId"), QString::number(startChangeId));
    }
    url.setQuery(query);

    return url;
}

QUrl fetchAppUrl(const QString &appId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::AppsBasePath % QLatin1Char('/') % appId);
    return url;
}

QUrl fetchAppsUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::AppsBasePath);
    return url;
}

QUrl fetchChildReference(const QString &folderId, const QString &referenceId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % folderId % QLatin1String("/children/") % referenceId);
    return url;
}

QUrl fetchChildReferences(const QString &folderId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % folderId % QLatin1String("/children"));
    return url;
}

QUrl createChildReference(const QString &folderId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % folderId % QLatin1String("/children"));
    return url;
}

QUrl deleteChildReference(const QString &folderId, const QString &referenceId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % folderId % QLatin1String("/children/") % referenceId);
    return url;
}

QUrl fetchChangeUrl(const QString &changeId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ChangeBasePath % QLatin1Char('/') % changeId);
    return url;
}

QUrl copyFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/copy"));
    return url;
}

QUrl deleteFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId);
    return url;
}

QUrl fetchFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId);
    return url;
}

QUrl fetchFilesUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath);
    return url;
}

QUrl fetchChangesUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::ChangeBasePath);
    return url;
}

QUrl touchFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/touch"));
    return url;
}

QUrl trashFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/trash"));
    return url;
}

QUrl untrashFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/untrash"));
    return url;
}

QUrl uploadMetadataFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    if (!fileId.isEmpty()) {
        url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId);
    } else {
        url.setPath(Private::FilesBasePath);
    }
    return url;
}

QUrl uploadMediaFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    if (!fileId.isEmpty()) {
        url.setPath(QLatin1String("/upload") % Private::FilesBasePath % QLatin1Char('/') % fileId);
    } else {
        url.setPath(QLatin1String("/upload") % Private::FilesBasePath);
    }
    return url;
}

QUrl uploadMultipartFileUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    if (!fileId.isEmpty()) {
        url.setPath(QLatin1String("/upload") % Private::FilesBasePath % QLatin1Char('/') % fileId);
    } else {
        url.setPath(QLatin1String("/upload") % Private::FilesBasePath);
    }
    return url;
}

QUrl fetchParentReferenceUrl(const QString &fileId, const QString &referenceId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/parents/") % referenceId);
    return url;
}

QUrl fetchParentReferencesUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/parents"));

    return url;
}

QUrl createParentReferenceUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/parents"));
    return url;
}

QUrl deleteParentReferenceUrl(const QString &fileId, const QString &referenceId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/parents/") % referenceId);
    return url;
}

QUrl fetchPermissionsUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/permissions"));
    return url;
}

QUrl fetchPermissionUrl(const QString &fileId, const QString &permissionId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/permissions/") % permissionId);
    return url;
}

QUrl createPermissionUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/permissions"));
    return url;
}

QUrl deletePermissionUrl(const QString &fileId, const QString &permissionId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/permissions/") % permissionId);
    return url;
}

QUrl modifyPermissionUrl(const QString &fileId, const QString &permissionId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/permissions/") % permissionId);
    return url;
}

QUrl fetchRevisionUrl(const QString &fileId, const QString &revisionId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/revisions/") % revisionId);
    return url;
}

QUrl fetchRevisionsUrl(const QString &fileId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/revisions"));
    return url;
}

QUrl deleteRevisionUrl(const QString &fileId, const QString &revisionId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/revisions/") % revisionId);
    return url;
}

QUrl modifyRevisionUrl(const QString &fileId, const QString &revisionId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/revisions/") % revisionId);
    return url;
}

QUrl fetchDrivesUrl(const QString &drivesId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::DrivesBasePath % QLatin1Char('/') % drivesId);
    return url;
}

QUrl hideDrivesUrl(const QString &drivesId, bool hide)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::DrivesBasePath % QLatin1Char('/') % drivesId % (hide ? QLatin1String("/hide") : QLatin1String("/unhide")));
    return url;
}

QUrl fetchDrivesUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::DrivesBasePath);
    return url;
}

QUrl fetchTeamdriveUrl(const QString &teamdriveId)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TeamdriveBasePath % QLatin1Char('/') % teamdriveId);
    return url;
}

QUrl fetchTeamdrivesUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TeamdriveBasePath);
    return url;
}

} // namespace DriveService

} // namespace KGAPI2
