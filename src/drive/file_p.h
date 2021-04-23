/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

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
    qlonglong version;
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

