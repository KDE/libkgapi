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

#ifndef LIBKGAPI2_DRIVEFILE_P_H
#define LIBKGAPI2_DRIVEFILE_P_H

#include "file.h"

#include <QVariantMap>

namespace KGAPI2
{

namespace Drive
{

class Q_DECL_HIDDEN File::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QUrl selfLink;
    QString title;
    QString mimeType;
    QString description;
    LabelsPtr labels;
    QDateTime createdDate;
    QDateTime modifiedDate;
    QDateTime modifiedByMeDate;
    QUrl downloadUrl;
    IndexableTextPtr indexableText;
    PermissionPtr userPermission;
    QString fileExtension;
    QString md5Checksum;
    qlonglong fileSize;
    QUrl alternateLink;
    QUrl embedLink;
    QDateTime sharedWithMeDate;
    ParentReferencesList parents;
    QMap<QString,QUrl> exportLinks;
    QString originalFileName;
    qlonglong quotaBytesUsed;
    QList<QString> ownerNames;
    QString lastModifyingUserName;
    bool editable;
    bool writersCanShare;
    QUrl thumbnailLink;
    QDateTime lastViewedByMeDate;
    QUrl webContentLink;
    bool explicitlyTrashed;
    ImageMediaMetadataPtr imageMediaMetadata;
    ThumbnailPtr thumbnail;
    QUrl webViewLink;
    QUrl iconLink;
    bool shared;
    UsersList owners;
    UserPtr lastModifyingUser;

    static FilePtr fromJSON(const QVariantMap &map);

};

} // namespace Drive

} // namespace KGAPI2

#endif // LIBKGAPI2_DRIVEFILE_P_H
