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

#ifndef LIBKGAPI_OBJECTS_DRIVEFILE_H
#define LIBKGAPI_OBJECTS_DRIVEFILE_H

#include <libkgapi/objects/drive/permission.h>
#include <libkgapi/objects/drive/parentreference.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DriveFilePrivate;
class DriveFilePrivate_Labels;
class DriveFilePrivate_IndexableText;
class DriveFilePrivate_ImageMediaMetadata;
class DriveFilePrivate_ImageMediaMetadata_Location;

class LIBKGAPI_EXPORT DriveFile: public KGAPI::Object
{
public:
    class Labels
    {
    public:
        Labels();
        Labels(const Labels& other);
        virtual ~Labels();
        
        bool starred();
        void setStarred(const bool& starred);
        
        bool hidden();
        void setHidden(const bool& hidden);
        
        bool trashed();
        void setTrashed(const bool& trashed);
        
        bool restricted();
        void setRestricted(const bool& restricted);
        
        bool viewed();
        void setViewed(const bool& viewed);
    private:
        DriveFilePrivate_Labels* d;
    };
    
    class IndexableText
    {
    public:
        IndexableText();
        IndexableText(const IndexableText& other);
        virtual ~IndexableText();
        
        QString text();
        void setText(const QString& text);
        
    private:
        DriveFilePrivate_IndexableText* d;
    };
    
    class ImageMediaMetadata
    {
    public:
        class Location
        {
        public:
            Location();
            Location(const Location& other);
            virtual ~Location();
            
            qreal latitude();
            void setLatitude(const qreal& latitude);
            
            qreal longitude();
            void setLongitude(const qreal& longitude);
            
            qreal altitude();
            void setAltitude(const qreal& altitude);
            
        private:
            DriveFilePrivate_ImageMediaMetadata_Location* d;
        };
        
        ImageMediaMetadata();
        ImageMediaMetadata(const ImageMediaMetadata& other);
        virtual ~ImageMediaMetadata();
        
        int width();
        void setWidth(const int& width);
        
        int height();
        void setHeight(const int& height);
        
        int rotation();
        void setRotation(const int& rotation);
        
        DriveFile::ImageMediaMetadata::Location location();
        void setLocation(const Location& location);
        
    private:
        DriveFilePrivate_ImageMediaMetadata* d;
    };

    typedef QSharedPointer< DriveFile > Ptr;
    typedef QList< DriveFile > List;
    
    DriveFile();
    DriveFile(const DriveFile& other);
    virtual ~DriveFile();
    
    QString id();
    void setId(const QString& id);
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    QString title();
    void setTitle(const QString& title);
    
    QString mimeType();
    void setMimeType(const QString& mimeType);
    
    QString description();
    void setDescription(const QString& description);
    
    DriveFile::Labels labels();
    void setLabels(const Labels& labels);
    
    QDateTime createdDate();
    void setCreatedDate(const QDateTime& createdDate);
    
    QDateTime modifiedDate();
    void setModifiedDate(const QDateTime& modifiedDate);
    
    QDateTime modifiedByMeDate();
    void setModifiedByMeDate(const QDateTime& modifiedByMeDate);
    
    QString downloadUrl();
    void setDownloadUrl(const QString& downloadUrl);
    
    DriveFile::IndexableText indexableText();
    void setIndexableText(const IndexableText& indexableText);
    
    DrivePermission::Ptr userPermission();
    void setUserPermission(const DrivePermission::Ptr& userPermission);
    
    QString fileExtension();
    void setFileExtension(const QString& fileExtension);
    
    QString md5Checksum();
    void setMd5Checksum(const QString& md5Checksum);
    
    qulonglong fileSize();
    void setFileSize(const qulonglong& fileSize);
    
    QString alternateLink();
    void setAlternateLink(const QString& alternateLink);
    
    QString embedLink();
    void setEmbedLink(const QString& embedLink);
    
    QDateTime sharedWithMeDate();
    void setSharedWithMeDate(const QDateTime& sharedWithMeDate);
    
    QList<DriveParentReference> parents();
    void setParents(const QList<DriveParentReference>& parents);
    
    QMap<QString, QString> exportLinks();
    void setExportLinks(const QMap<QString, QString>& exportLinks);
    
    QString originalFileName();
    void setOriginalFileName(const QString& originalFileName);
    
    qulonglong quotaBytesUsed();
    void setQuotaBytesUsed(const qulonglong& quotaBytesUsed);
    
    QStringList ownerNames();
    void setOwnerNames(const QStringList& ownerNames);
    
    QString lastModifyingUserName();
    void setLastModifyingUserName(const QString& lastModifyingUserName);
    
    bool editable();
    void setEditable(const bool& editable);
    
    bool writersCanShare();
    void setWritersCanShare(const bool& writersCanShare);
    
    QString thumbnailLink();
    void setThumbnailLink(const QString& thumbnailLink);
    
    QDateTime lastViewedByMeDate();
    void setLastViewedByMeDate(const QDateTime& lastViewedByMeDate);
    
    QString webContentLink();
    void setWebContentLink(const QString& webContentLink);
    
    bool explicitlyTrashed();
    void setExplicitlyTrashed(const bool& explicitlyTrashed);
    
    DriveFile::ImageMediaMetadata imageMediaMetadata();
    void setImageMediaMetadata(const ImageMediaMetadata& imageMediaMetadata);
    
private:
    DriveFilePrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveFile::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveFile*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveFile::List)

#endif // LIBKGAPI_OBJECTS_DRIVEFILE_H
