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

#include <KDateTime>

namespace KGAPI
{

namespace Objects
{

class DriveFilePrivate;
class DriveFilePrivate_Labels;
class DriveFilePrivate_IndexableText;
class DriveFilePrivate_ImageMediaMetadata;
class DriveFilePrivate_ImageMediaMetadata_Location;

/**
 * DriveFile contains metadata for a file.
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/files">Files</a>
 */
class LIBKGAPI_EXPORT DriveFile: public KGAPI::Object
{
public:
    /**
     * DriveFile::Labels holds the structure used for labels property.
     */
    class Labels
    {
    public:
        Labels();
        Labels(const Labels& other);
        virtual ~Labels();
        
        /**
         * Returns whether this file is starred by the user.
         */
        QVariant starred();
        
        /**
         * Sets whether this file is starred by the user.
         */
        void setStarred(const QVariant& starred);
        
        /**
         * Returns whether this file is hidden from the user.
         */
        QVariant hidden();
        
        /**
         * Sets whether this file is hidden from the user
         */
        void setHidden(const QVariant& hidden);
        
        /**
         * Returns whether this file has been trashed.
         */
        QVariant trashed();
        
        /**
         * Sets whether this file has been trashed.
         */
        void setTrashed(const QVariant& trashed);
        
        /**
         * Returns whether viewers are prevented from downloading this file.
         */
        QVariant restricted();
        
        /**
         * Sets whether viewers are prevented from downloading this file.
         */
        void setRestricted(const QVariant& restricted);
        
        /**
         * Returns whether this file has been viewed by this user.
         */
        QVariant viewed();
        
        /**
         * Sets whether this file has been viewed by this user.
         */
        void setViewed(const QVariant& viewed);
        
    private:
        DriveFilePrivate_Labels* d;
    };
    
    /**
     * DriveFile::IndexableText holds the structure used for indexableText property.
     */
    class IndexableText
    {
    public:
        IndexableText();
        IndexableText(const IndexableText& other);
        virtual ~IndexableText();
        
        /**
         * Returns the text to be indexed for this file.
         */
        QString text();
        
        /**
         * Sets the text to be indexed for this file.
         */
        void setText(const QString& text);
        
    private:
        DriveFilePrivate_IndexableText* d;
    };
    
    /**
     * DriveFile::ImageMediaMetadata holds the structure used for imageMediaMetadata property.
     */
    class ImageMediaMetadata
    {
    public:
        /**
         * DriveFile::ImageMediaMetadata::Location holds the structure used for imageMediaMetadata.location property.
         */
        class Location
        {
        public:
            Location();
            Location(const Location& other);
            virtual ~Location();
            
            /**
             * Returns the latitude stored in the image.
             */
            qreal latitude();
            
            /**
             * Sets the latitude stored in the image.
             */
            void setLatitude(const qreal& latitude);
            
            /**
             * Returns the longitude stored in the image.
             */
            qreal longitude();
            
            /**
             * Sets the longitude stored in the image.
             */
            void setLongitude(const qreal& longitude);
            
            /**
             * Returns the altitude stored in the image.
             */
            qreal altitude();
            
            /**
             * Sets the altitude stored in the image.
             */
            void setAltitude(const qreal& altitude);
            
        private:
            DriveFilePrivate_ImageMediaMetadata_Location* d;
        };
        
        ImageMediaMetadata();
        ImageMediaMetadata(const ImageMediaMetadata& other);
        virtual ~ImageMediaMetadata();
        
        /**
         * Returns the width of the image in pixels.
         */
        int width();
        
        /**
         * Sets the width of the image in pixels.
         */
        void setWidth(const int& width);
        
        /**
         * Returns the height of the image in pixels.
         */
        int height();
        
        /**
         * Sets the height of the image in pixels.
         */
        void setHeight(const int& height);
        
        /**
         * Returns the rotation in clockwise degrees from the image's original orientation.
         */
        int rotation();
        
        /**
         * Sets the rotation in clockwise degrees from the image's original orientation.
         */
        void setRotation(const int& rotation);
        
        /**
         * Returns the geographic location information stored in the image.
         */
        DriveFile::ImageMediaMetadata::Location location();
        
        /**
         * Sets the geographic location information stored in the image.
         */
        void setLocation(const Location& location);
        
    private:
        DriveFilePrivate_ImageMediaMetadata* d;
    };

    typedef QSharedPointer< DriveFile > Ptr;
    typedef QList< DriveFile > List;
    
    DriveFile();
    DriveFile(const DriveFile& other);
    virtual ~DriveFile();
    
    /**
     * Returns the id of the file.
     */
    QString id();
    
    /**
     * Sets the id of the file.
     */
    void setId(const QString& id);
    
    /**
     * Returns a link back to this file.
     */
    QString selfLink();
    
    /**
     * Sets the link back to this file.
     */
    void setSelfLink(const QString& selfLink);
    
    /**
     * Returns the title of this file.
     * Used to identify file or folder name.
     */
    QString title();
    
    /**
     * Sets the title of this file.
     * Used to identify file or folder name.
     */
    void setTitle(const QString& title);
    
    /**
     * Returns the MIME type of the file.
     */
    QString mimeType();
    
    /**
     * Sets the MIME type of the file.
     */
    void setMimeType(const QString& mimeType);
    
    /**
     * Returns a short description of the file.
     */
    QString description();
    
    /**
     * Sets a short description of the file.
     */
    void setDescription(const QString& description);
    
    /**
     * Returns a group of labels for the file.
     */
    DriveFile::Labels labels();
    
    /**
     * Sets a group of labels for the file.
     */
    void setLabels(const Labels& labels);
    
    /**
     * Returns the create time for this file.
     */
    KDateTime createdDate();
    
    /**
     * Sets the create time for this file.
     */
    void setCreatedDate(const KDateTime& createdDate);
    
    /**
     * Returns the last time this file was modified by anyone.
     * This is only mutable on update when the setModifiedDate parameter is set.
     */
    KDateTime modifiedDate();
    
    /**
     * Sets the last time this file was modified by anyone.
     * This is only mutable on update when the setModifiedDate parameter is set.
     */
    void setModifiedDate(const KDateTime& modifiedDate);
    
    /**
     * Returns the last time this file was modified by the currently authenticated user.
     */
    KDateTime modifiedByMeDate();
    
    /**
     * Sets the last time this file was modified by the currently authenticated user.
     */
    void setModifiedByMeDate(const KDateTime& modifiedByMeDate);
    
    /**
     * Returns a short lived download URL for the file. This is only populated for files with content stored in Drive.
     */
    QString downloadUrl();
    
    /**
     * Sets the short lived download URL for the file. This is only populated for files with content stored in Drive.
     */
    void setDownloadUrl(const QString& downloadUrl);
    
    /**
     * Returns the indexable text attributes for the file.
     * This property can only be written, and is not returned by files.get
     */
    DriveFile::IndexableText indexableText();
    
    /**
     * Sets the indexable text attributes for the file.
     * This property can only be written, and is not returned by files.get
     */
    void setIndexableText(const IndexableText& indexableText);
    
    /**
     * Returns the permissions for the authenticated user on this file.
     */
    DrivePermission::Ptr userPermission();
    
    /**
     * Sets the permissions for the authenticated user on this file.
     */
    void setUserPermission(const DrivePermission::Ptr& userPermission);
    
    /**
     * Returns the file extension used when downloading this file.
     * This field is read only. To set the extension, include it on title when creating the file.
     * This is populated only for files with content stored in Drive.
     */
    QString fileExtension();
    
    /**
     * Sets the file extension used when downloading this file.
     * This field is read only. To set the extension, include it on title when creating the file.
     * This is populated only for files with content stored in Drive.
     */
    void setFileExtension(const QString& fileExtension);
    
    /**
     * Returns an MD5 checksum for the content of this file.
     * This is populated only for files with content stored in Drive.
     */
    QString md5Checksum();
    
    /**
     * Sets an MD5 checksum for the content of this file.
     * This is populated only for files with content stored in Drive
     */
    void setMd5Checksum(const QString& md5Checksum);
    
    /**
     * Returns the size of the file in bytes.
     * This is populated only for files with content stored in Drive.
     */
    qlonglong fileSize();
    
    /**
     * Sets the size of the file in bytes.
     * This is populated only for files with content stored in Drive.
     */
    void setFileSize(const qlonglong& fileSize);
    
    /**
     * Returns a link for opening the file in using a relevant Google editor or viewer.
     */
    QString alternateLink();
    
    /**
     * Sets the link for opening the file in using a relevant Google editor or viewer.
     */
    void setAlternateLink(const QString& alternateLink);
    
    /**
     * Returns a link for embedding the file.
     */
    QString embedLink();
    
    /**
     * Sets the link for embedding the file.
     */
    void setEmbedLink(const QString& embedLink);
    
    /**
     * Returns the time at which this file was shared with the user.
     */
    KDateTime sharedWithMeDate();
    
    /**
     * Sets the time at which this file was shared with the user.
     */
    void setSharedWithMeDate(const KDateTime& sharedWithMeDate);
    
    /**
     * Returns the collection of parent folders which contain this file.
     * Setting this field will put the file in all of the provided folders.
     * On insert, if no folders are provided, the file will be placed in the default root folder.
     */
    QList<DriveParentReference> parents();
    
    /**
     * Sets the collection of parent folders which contain this file.
     * Setting this field will put the file in all of the provided folders.
     * On insert, if no folders are provided, the file will be placed in the default root folder.
     */
    void setParents(const QList<DriveParentReference>& parents);
    
    /**
     * Returns the links for exporting Google Docs to specific formats.
     * This is a map from the export format to URL.
     */
    QMap<QString, QString> exportLinks();
    
    /**
     * Sets the links for exporting Google Docs to specific formats.
     * This is a map from the export format to URL.
     */
    void setExportLinks(const QMap<QString, QString>& exportLinks);
    
    /**
     * Returns the original filename if the file was uploaded manually,
     * or the original title if the file was inserted through the API.
     * Note that renames of the title will not change the original filename.
     * This will only be populated on files with content stored in Drive.
     */
    QString originalFileName();
    
    /**
     * Returns the original filename if the file was uploaded manually,
     * or the original title if the file was inserted through the API.
     * Note that renames of the title will not change the original filename.
     * This will only be populated on files with content stored in Drive.
     */
    void setOriginalFileName(const QString& originalFileName);
    
    /**
     * Returns the number of quota bytes used by this file.
     */
    qlonglong quotaBytesUsed();
    
    /**
     * Sets the number of quota bytes used by this file.
     */
    void setQuotaBytesUsed(const qlonglong& quotaBytesUsed);
    
    /**
     * Return the name(s) of the owner(s) of this file.
     */
    QStringList ownerNames();
    
    /**
     * Sets the name(s) of the owner(s) of this file.
     */
    void setOwnerNames(const QStringList& ownerNames);
    
    /**
     * Returns the name of the last user to modify this file. This will only be populated if a user has edited this file.
     */
    QString lastModifyingUserName();
    
    /**
     * Sets the name of the last user to modify this file. This will only be populated if a user has edited this file.
     */
    void setLastModifyingUserName(const QString& lastModifyingUserName);
    
    /**
     * Returns whether the file can be edited by the current user.
     */
    QVariant editable();
    
    /**
     * Sets whether the file can be edited by the current user.
     */
    void setEditable(const QVariant& editable);
    
    /**
     * Returns whether writers can share the document with other users.
     */
    QVariant writersCanShare();
    
    /**
     * Sets whether writers can share the document with other users.
     */
    void setWritersCanShare(const QVariant& writersCanShare);
    
    /**
     * Returns a link to the file's thumbnail.
     */
    QString thumbnailLink();
    
    /**
     * Sets the link to the file's thumbnail.
     */
    void setThumbnailLink(const QString& thumbnailLink);
    
    /**
     * Returns the last time this file was viewed by the user.
     */
    KDateTime lastViewedByMeDate();
    
    /**
     * Sets the last time this file was viewed by the user.
     */
    void setLastViewedByMeDate(const KDateTime& lastViewedByMeDate);
    
    /**
     * Returns a link for downloading the content of the file in a browser using cookie based authentication.
     * In cases where the content is shared publicly, the content can be downloaded without any credentials.
     */
    QString webContentLink();
    
    /**
     * Sets a link for downloading the content of the file in a browser using cookie based authentication.
     * In cases where the content is shared publicly, the content can be downloaded without any credentials.
     */
    void setWebContentLink(const QString& webContentLink);
    
    /**
     * Returns whether this file has been explicitly trashed, as opposed to recursively trashed.
     * This will only be populated if the file is trashed.
     */
    QVariant explicitlyTrashed();
    
    /**
     * Sets whether this file has been explicitly trashed, as opposed to recursively trashed.
     * This will only be populated if the file is trashed.
     */
    void setExplicitlyTrashed(const QVariant& explicitlyTrashed);
    
    /**
     * Returns metadata about image media.
     * This will only be present for image types, and its contents will depend on what can be parsed from the image content.
     */
    DriveFile::ImageMediaMetadata imageMediaMetadata();
    
    /**
     * Sets metadata about image media.
     * This will only be present for image types, and its contents will depend on what can be parsed from the image content.
     */
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
