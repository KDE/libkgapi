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

#ifndef LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H
#define LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DriveRevisionPrivate;

class LIBKGAPI_EXPORT DriveRevision: public KGAPI::Object
{
public:
    typedef QSharedPointer< DriveRevision > Ptr;
    typedef QList< DriveRevision > List;
    
    DriveRevision();
    DriveRevision(const DriveRevision& other);
    virtual ~DriveRevision();
    
    QString id();
    void setId(const QString& id);
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    QString mimeType();
    void setMimeType(const QString& mimeType);
    
    QDateTime modifiedDate();
    void setModifiedDate(const QDateTime& modifiedDate);
    
    bool pinned();
    void setPinned(const bool& pinned);
    
    bool published();
    void setPublished(const bool& published);
    
    QString publishedLink();
    void setPublishedLink(const QString& publishedLink);
    
    bool publishedAuto();
    void setPublishedAuto(const bool& publishedAuto);
    
    bool publishedOutsideDomain();
    void setPublishedOutsideDomain(const bool& publishedOutsideDomain);
    
    QString downloadUrl();
    void setDownloadUrl(const QString& downloadUrl);
    
    QMap<QString, QString> exportLinks();
    void setExportLinks(const QMap<QString, QString>& exportLinks);
    
    QString lastModifyingUserName();
    void setLastModifyingUserName(const QString& lastModifyingUserName);
    
    QString originalFilename();
    void setOriginalFilename(const QString& originalFilename);
    
    QString md5Checksum();
    void setMd5Checksum(const QString& md5Checksum);
    
    qulonglong fileSize();
    void setFileSize(const qulonglong& fileSize);

private:
    DriveRevisionPrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveRevision::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveRevision*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveRevision::List)

#endif // LIBKGAPI_OBJECTS_DRIVECHILDREFERENCE_H
