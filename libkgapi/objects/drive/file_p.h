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

#ifndef LIBKGAPI_OBJECTS_DRIVEFILE_P_H
#define LIBKGAPI_OBJECTS_DRIVEFILE_P_H

#include <libkgapi/objects/drive/permission.h>
#include <libkgapi/objects/drive/parentreference.h>

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMap>

namespace KGAPI
{
    
namespace Objects
{

class DriveFilePrivate_Labels
{
public:
    bool starred;
    bool hidden;
    bool trashed;
    bool restricted;
    bool viewed;
};

class DriveFilePrivate_IndexableText
{
public:
    QString text;
};

class DriveFilePrivate_ImageMediaMetadata_Location
{
public:
    qreal latitude;
    qreal longitude;
    qreal altitude;
};

class DriveFilePrivate_ImageMediaMetadata
{
public:
    typedef DriveFilePrivate_ImageMediaMetadata_Location Location;
    
    int width;
    int height;
    int rotation;
    DriveFile::ImageMediaMetadata::Location location;
};

class DriveFilePrivate
{
public:
    
    // need that because nested classes can't be forward-declared
    typedef DriveFilePrivate_Labels Labels;
    typedef DriveFilePrivate_IndexableText IndexableText;
    typedef DriveFilePrivate_ImageMediaMetadata ImageMediaMetadata;
    
    DriveFilePrivate() { };
    DriveFilePrivate(const DriveFilePrivate& other);
    ~DriveFilePrivate() { };

    QString id;
    QString selfLink;
    QString title;
    QString mimeType;
    QString description;
    DriveFile::Labels labels;
    QDateTime createdDate;
    QDateTime modifiedDate;
    QDateTime modifiedByMeDate;
    QString downloadUrl;
    DriveFile::IndexableText indexableText;
    DrivePermission::Ptr userPermission;
    QString fileExtension;
    QString md5Checksum;
    qulonglong fileSize;
    QString alternateLink;
    QString embedLink;
    QDateTime sharedWithMeDate;
    QList<DriveParentReference> parents;
    QMap<QString, QString> exportLinks;
    QString originalFileName;
    qulonglong quotaBytesUsed;
    QStringList ownerNames;
    QString lastModifyingUserName;
    bool editable;
    bool writersCanShare;
    QString thumbnailLink;
    QDateTime lastViewedByMeDate;
    QString webContentLink;
    bool explicitlyTrashed;
    DriveFile::ImageMediaMetadata imageMediaMetadata;
};

} /* namespace Objects */
    
} /* namespace KGAPI */
    
#endif // LIBKGAPI_OBJECTS_DRIVEFILE_P_H