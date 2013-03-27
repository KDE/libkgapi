/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI2_DRIVEFILE_P_H
#define LIBKGAPI2_DRIVEFILE_P_H

#include "file.h"

#include <QtCore/QVariantMap>

namespace KGAPI2
{

class DriveFile::Private
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
    KDateTime createdDate;
    KDateTime modifiedDate;
    KDateTime modifiedByMeDate;
    QUrl downloadUrl;
    IndexableTextPtr indexableText;
    DrivePermissionPtr userPermission;
    QString fileExtension;
    QString md5Checksum;
    qlonglong fileSize;
    QUrl alternateLink;
    QUrl embedLink;
    KDateTime sharedWithMeDate;
    DriveParentReferencesList parents;
    QMap<QString,QUrl> exportLinks;
    QString originalFileName;
    qlonglong quotaBytesUsed;
    QList<QString> ownerNames;
    QString lastModifyingUserName;
    bool editable;
    bool writersCanShare;
    QUrl thumbnailLink;
    KDateTime lastViewedByMeDate;
    QUrl webContentLink;
    bool explicitlyTrashed;
    ImageMediaMetadataPtr imageMediaMetadata;

    static DriveFilePtr fromJSON(const QVariantMap &map);

};

} // namespace KGAPI2

#endif // LIBKGAPI2_DRIVEFILE_P_H
