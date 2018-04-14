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
    url.setPath(Private::FilesBasePath % QLatin1Char('/') % fileId % QLatin1String("/premissions"));
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

} // namespace DriveService

} // namespace KGAPI2

