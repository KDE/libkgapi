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

#include "driveservice.h"
#include "utils.h"

#include <KDE/KUrl>

namespace KGAPI2
{

namespace DriveService
{

QUrl fetchAboutUrl(bool includeSubscribed, qlonglong maxChangeIdCount, qlonglong startChangeId)
{
    KUrl url("https://www.googleapis.com/drive/v2/about");
    url.addQueryItem(QLatin1String("includeSubscribed"), Utils::bool2Str(includeSubscribed));
    if (maxChangeIdCount > 0) {
        url.addQueryItem(QLatin1String("maxChangeIdCount"), QString::number(maxChangeIdCount));
    }
    if (startChangeId > 0) {
        url.addQueryItem(QLatin1String("startChangeId"), QString::number(startChangeId));
    }

    return url;
}

QUrl fetchAppUrl(const QString &appId)
{
    KUrl url("https://www.googleapis.com/drive/v2/apps");
    url.addPath(appId);

    return url;
}

QUrl fetchAppsUrl()
{
    return KUrl("https://www.googleapis.com/drive/v2/apps/");
}

QUrl fetchParentReferenceUrl(const QString &fileId, const QString &referenceId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files");
    url.addPath(fileId);
    url.addPath(QLatin1String("parents"));
    url.addPath(referenceId);

    return url;
}

QUrl fetchParentReferencesUrl(const QString &fileId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files");
    url.addPath(fileId);
    url.addPath(QLatin1String("parents"));

    return url;
}

QUrl createParentReferenceUrl(const QString &fileId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files");
    url.addPath(fileId);
    url.addPath(QLatin1String("parents"));

    return url;
}

QUrl deleteParentReferenceUrl(const QString &fileId, const QString &referenceId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files");
    url.addPath(fileId);
    url.addPath(QLatin1String("parents"));
    url.addPath(referenceId);

    return url;
}

QUrl fetchPermissionsUrl(const QString &fileId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("permissions"));

    return url;
}

QUrl fetchPermissionUrl(const QString &fileId, const QString &permissionId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("permissions"));
    url.addPath(permissionId);

    return url;
}

QUrl createPermissionUrl(const QString &fileId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("permissions"));

    return url;
}

QUrl deletePermissionUrl(const QString &fileId, const QString &permissionId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("permissions"));
    url.addPath(permissionId);

    return url;
}

QUrl modifyPermissionUrl(const QString &fileId, const QString &permissionId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("permissions"));
    url.addPath(permissionId);

    return url;
}

QUrl fetchRevisionUrl(const QString &fileId, const QString &revisionId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("revisions"));
    url.addPath(revisionId);

    return url;
}

QUrl fetchRevisionsUrl(const QString &fileId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("revisions"));

    return url;
}

QUrl deleteRevisionUrl(const QString &fileId, const QString &revisionId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("revisions"));
    url.addPath(revisionId);

    return url;
}

QUrl modifyRevisionUrl(const QString &fileId, const QString &revisionId)
{
    KUrl url("https://www.googleapis.com/drive/v2/files/");
    url.addPath(fileId);
    url.addPath(QLatin1String("revisions"));
    url.addPath(revisionId);

    return url;
}

} // namespace DriveService

} // namespace KGAPI2

