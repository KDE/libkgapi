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

#ifndef LIBKGAPI2_DRIVEFILE_H
#define LIBKGAPI2_DRIVEFILE_H

#include <libkgapi2/types.h>

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <KDE/KDateTime>

namespace KGAPI2
{

/**
 * @brief DriveFile contains metadata for a file.
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/files">Files</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT DriveFile
{

  public:
    /**
     * @brief DriveFile::Labels holds the structure used for labels property.
     */
    class Labels
    {
      public:
        explicit Labels();
        explicit Labels(const Labels &other);
        virtual ~Labels();

        /**
         * @brief Returns whether this file is starred by the user.
         */
        bool starred();

        /**
         * @brief Sets whether this file is starred by the user.
         *
         * @param starred
         */
        void setStarred(bool starred);

        /**
         * @brief Returns whether this file is hidden from the user.
         */
        bool hidden();

        /**
         * @brief Sets whether this file is hidden from the user
         *
         * @param hidden
         */
        void setHidden(bool hidden);

        /**
         * @brief Returns whether this file has been trashed.
         */
        bool trashed();

        /**
         * @brief Sets whether this file has been trashed.
         *
         * @param trashed
         */
        void setTrashed(bool trashed);

        /**
         * @brief Returns whether viewers are prevented from downloading this file.
         */
        bool restricted();

        /**
         * @brief Sets whether viewers are prevented from downloading this file.
         *
         * @param restricted
         */
        void setRestricted(bool restricted);

        /**
         * @brief Returns whether this file has been viewed by this user.
         */
        bool viewed();

        /**
         * @brief Sets whether this file has been viewed by this user.
         *
         * @param viewed
         */
        void setViewed(bool viewed);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<Labels> LabelsPtr;
    typedef QList<LabelsPtr> LabelsList;

    /**
     * @brief DriveFile::IndexableText holds the structure used for indexableText property.
     */
    class IndexableText
    {
      public:
        explicit IndexableText();
        explicit IndexableText(const IndexableText &other);
        virtual ~IndexableText();

        /**
         * @brief Returns the text to be indexed for this file.
         */
        QString text();

        /**
         * @brief Sets the text to be indexed for this file.
         *
         * @param text
         */
        void setText(const QString &text);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<IndexableText> IndexableTextPtr;

    /**
     * @brief DriveFile::ImageMediaMetadata holds the structure used for
     *        imageMediaMetadata property.
     */
    class ImageMediaMetadata
    {
      public:
        /**
         * @brief DriveFile::ImageMediaMetadata::Location holds the structure used
         *        for imageMediaMetadata.location property.
         */
        class Location
        {
          public:
            explicit Location();
            explicit Location(const Location &other);
            virtual ~Location();

            /**
             * @brief Returns the latitude stored in the image.
             */
            qreal latitude();

            /**
             * @brief Sets the latitude stored in the image.
             *
             * @param latitude
             */
            void setLatitude(qreal latitude);

            /**
             * @brief Returns the longitude stored in the image.
             */
            qreal longitude();

            /**
             * @brief Sets the longitude stored in the image.
             *
             * @param longitude
             */
            void setLongitude(qreal longitude);

            /**
             * @brief Returns the altitude stored in the image.
             */
            qreal altitude();

            /**
             * @brief Sets the altitude stored in the image.
             *
             * @param altitude
             */
            void setAltitude(qreal altitude);

          private:
            class Private;
            Private *const d;
            friend class Private;
        };

        typedef QSharedPointer<Location> LocationPtr;

        explicit ImageMediaMetadata();
        explicit ImageMediaMetadata(const ImageMediaMetadata &other);
        virtual ~ImageMediaMetadata();

        /**
         * @brief Returns the width of the image in pixels.
         */
        int width();

        /**
         * @brief Sets the width of the image in pixels.
         *
         * @param width
         */
        void setWidth(int width);

        /**
         * @brief Returns the height of the image in pixels.
         */
        int height();

        /**
         * @brief Sets the height of the image in pixels.
         *
         * @param height
         */
        void setHeight(int height);

        /**
         * @brief Returns the rotation in clockwise degrees from the image's original orientation.
         */
        int rotation();

        /**
         * @brief Sets the rotation in clockwise degrees from the image's original orientation.
         */
        void setRotation(int rotation);

        /**
         * @brief Returns the geographic location information stored in the image.
         */
        LocationPtr location();

        /**
         * @brief Sets the geographic location information stored in the image.
         *
         * @param location
         */
        void setLocation(const LocationPtr &location);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<ImageMediaMetadata> ImageMediaMetadataPtr;

    explicit DriveFile();
    explicit DriveFile(const DriveFile &other);
    virtual ~DriveFile();

    /**
     * @brief Returns the id of the file.
     */
    QString id();

    /**
     * @brief Sets the id of the file.
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns a link back to this file.
     */
    QString selfLink();

    /**
     * @brief Sets the link back to this file.
     *
     * @param selfLink
     */
    void setSelfLink(const QString &selfLink);

    /**
     * @brief Returns the title of this file.
     *
     * Used to identify file or folder name.
     */
    QString title();

    /**
     * @brief Sets the title of this file.
     *
     * Used to identify file or folder name.
     *
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief Returns the MIME type of the file.
     */
    QString mimeType();

    /**
     * @brief Sets the MIME type of the file.
     *
     * @param mimeType
     */
    void setMimeType(const QString &mimeType);

    /**
     * @brief Returns a short description of the file.
     */
    QString description();

    /**
     * @brief Sets a short description of the file.
     *
     * @param description
     */
    void setDescription(const QString &description);

    /**
     * @brief Returns a group of labels for the file.
     */
    DriveFile::LabelsPtr labels();

    /**
     * @brief Sets a group of labels for the file.
     *
     * @param labels
     */
    void setLabels(const LabelsPtr &labels);

    /**
     * @brief Returns the create time for this file.
     */
    KDateTime createdDate();

    /**
     * @brief Sets the create time for this file.
     *
     * @param createdDate
     */
    void setCreatedDate(const KDateTime &createdDate);

    /**
     * @brief Returns the last time this file was modified by anyone.
     *
     * This is only mutable on update when the setModifiedDate parameter is set.
     */
    KDateTime modifiedDate();

    /**
     * @brief Sets the last time this file was modified by anyone.
     *
     * This is only mutable on update when the setModifiedDate parameter is set.
     *
     * @param modifiedDate
     */
    void setModifiedDate(const KDateTime &modifiedDate);

    /**
     * @brief Returns the last time this file was modified by the currently
     *        authenticated user.
     */
    KDateTime modifiedByMeDate();

    /**
     * @brief Sets the last time this file was modified by the currently
     *        authenticated user.
     *
     * @param modifiedByMeDate
     */
    void setModifiedByMeDate(const KDateTime &modifiedByMeDate);

    /**
     * @brief Returns a short lived download URL for the file.
     *
     * This is only populated for files with content stored in Drive.
     */
    QString downloadUrl();

    /**
     * @brief Sets the short lived download URL for the file.
     *
     * This is only populated for files with content stored in Drive.
     *
     * @param downloadUrl
     */
    void setDownloadUrl(const QString &downloadUrl);

    /**
     * @brief Returns the indexable text attributes for the file.
     *
     * This property can only be written, and is not returned by files.get
     */
    DriveFile::IndexableTextPtr indexableText();

    /**
     * @brief Sets the indexable text attributes for the file.
     *
     * This property can only be written, and is not returned by files.get
     *
     * @param indexableText
     */
    void setIndexableText(const IndexableTextPtr &indexableText);

    /**
     * @brief Returns the permissions for the authenticated user on this file.
     */
    DrivePermissionPtr userPermission();

    /**
     * @brief Sets the permissions for the authenticated user on this file.
     *
     * @param userPermission
     */
    void setUserPermission(const DrivePermissionPtr &userPermission);

    /**
     * @brief Returns the file extension used when downloading this file.
     *
     * This field is read only. To set the extension, include it on title when creating the file.
     * This is populated only for files with content stored in Drive.
     */
    QString fileExtension();

    /**
     * @brief Sets the file extension used when downloading this file.
     *
     * This field is read only. To set the extension, include it on title when creating the file.
     * This is populated only for files with content stored in Drive.
     *
     * @param fileExtension
     */
    void setFileExtension(const QString &fileExtension);

    /**
     * @brief Returns an MD5 checksum for the content of this file.
     *
     * This is populated only for files with content stored in Drive.
     */
    QString md5Checksum();

    /**
     * @brief Sets an MD5 checksum for the content of this file.
     * This is populated only for files with content stored in Drive
     *
     * @param md5Checksum
     */
    void setMd5Checksum(const QString &md5Checksum);

    /**
     * @brief Returns the size of the file in bytes.
     *
     * This is populated only for files with content stored in Drive.
     */
    qlonglong fileSize();

    /**
     * @brief Sets the size of the file in bytes.
     *
     * This is populated only for files with content stored in Drive.
     *
     * @param fileSize
     */
    void setFileSize(qlonglong fileSize);

    /**
     * @brief Returns a link for opening the file in using a relevant
     *        Google editor or viewer.
     */
    QString alternateLink();

    /**
     * @brief Sets the link for opening the file in using a relevant Google
     *        editor or viewer.
     *
     * @param alternateLink
     */
    void setAlternateLink(const QString &alternateLink);

    /**
     * @brief Returns a link for embedding the file.
     */
    QString embedLink();

    /**
     * @brief Sets the link for embedding the file.
     *
     * @param embedLink
     */
    void setEmbedLink(const QString &embedLink);

    /**
     * @brief Returns the time at which this file was shared with the user.
     */
    KDateTime sharedWithMeDate();

    /**
     * @brief Sets the time at which this file was shared with the user.
     *
     * @param sharedWithMeDate
     */
    void setSharedWithMeDate(const KDateTime &sharedWithMeDate);

    /**
     * @brief Returns the collection of parent folders which contain this file.
     *
     * Setting this field will put the file in all of the provided folders.
     * On insert, if no folders are provided, the file will be placed in the
     * default root folder.
     */
    DriveParentReferencesList parents();

    /**
     * @brief Sets the collection of parent folders which contain this file.
     *
     * Setting this field will put the file in all of the provided folders.
     * On insert, if no folders are provided, the file will be placed in the
     * default root folder.
     *
     * @param parents
     */
    void setParents(const DriveParentReferencesList &parents);

    /**
     * @brief Returns the links for exporting Google Docs to specific formats.
     *
     * This is a map from the export format to URL.
     */
    QMap < QString /* format */, QString /* url */ > exportLinks();

    /**
     * @brief Sets the links for exporting Google Docs to specific formats.
     *
     * This is a map from the export format to URL.
     *
     * @param exportLinks
     */
    void setExportLinks(const QMap < QString /* format */, QString /* url */ > &exportLinks);

    /**
     * @brief Returns the original filename if the file was uploaded manually,
     *        or the original title if the file was inserted through the API.
     *
     * Note that renames of the title will not change the original filename.
     * This will only be populated on files with content stored in Drive.
     */
    QString originalFileName();

    /**
     * @brief Returns the original filename if the file was uploaded manually,
     *        or the original title if the file was inserted through the API.
     *
     * Note that renames of the title will not change the original filename.
     * This will only be populated on files with content stored in Drive.
     *
     * @param originalFileName
     */
    void setOriginalFileName(const QString &originalFileName);

    /**
     * @bried Returns the number of quota bytes used by this file.
     */
    qlonglong quotaBytesUsed();

    /**
     * @brief Sets the number of quota bytes used by this file.
     */
    void setQuotaBytesUsed(qlonglong quotaBytesUsed);

    /**
     * @brief Return the name(s) of the owner(s) of this file.
     */
    QStringList ownerNames();

    /**
     * @brief Sets the name(s) of the owner(s) of this file.
     *
     * @param ownerNames
     */
    void setOwnerNames(const QStringList &ownerNames);

    /**
     * @brief Returns the name of the last user to modify this file.
     *
     * This will only be populated if a user has edited this file.
     */
    QString lastModifyingUserName();

    /**
     * @brief Sets the name of the last user to modify this file.
     *
     * This will only be populated if a user has edited this file.
     *
     * @param lastModifyingUserName
     */
    void setLastModifyingUserName(const QString &lastModifyingUserName);

    /**
     * @brief Returns whether the file can be edited by the current user.
     */
    bool editable();

    /**
     * @brief Sets whether the file can be edited by the current user.
     *
     * @param editable
     */
    void setEditable(bool editable);

    /**
     * @brief Returns whether writers can share the document with other users.
     */
    bool writersCanShare();

    /**
     * @brief Sets whether writers can share the document with other users.
     */
    void setWritersCanShare(bool writersCanShare);

    /**
     * @brief Returns a link to the file's thumbnail.
     */
    QString thumbnailLink();

    /**
     * @brief Sets the link to the file's thumbnail.
     *
     * @param thumbnailLink
     */
    void setThumbnailLink(const QString &thumbnailLink);

    /**
     * @brief Returns the last time this file was viewed by the user.
     */
    KDateTime lastViewedByMeDate();

    /**
     * @brief Sets the last time this file was viewed by the user.
     *
     * @param lastViewedByMeDate
     */
    void setLastViewedByMeDate(const KDateTime &lastViewedByMeDate);

    /**
     * @brief Returns a link for downloading the content of the file in a browser
     *        using cookie based authentication.
     *
     * In cases where the content is shared publicly, the content can be
     * downloaded without any credentials.
     */
    QString webContentLink();

    /**
     * @brief Sets a link for downloading the content of the file in a browser
     *        using cookie based authentication.
     *
     * In cases where the content is shared publicly, the content can be
     * downloaded without any credentials.
     *
     * @param webContentLink
     */
    void setWebContentLink(const QString &webContentLink);

    /**
     * @brief Returns whether this file has been explicitly trashed, as opposed
     *        to recursively trashed.
     *
     * This will only be populated if the file is trashed.
     */
    bool explicitlyTrashed();

    /**
     * @brief Sets whether this file has been explicitly trashed, as opposed
     *        to recursively trashed.
     *
     * This will only be populated if the file is trashed.
     *
     * @param explicitlyTrashed
     */
    void setExplicitlyTrashed(bool explicitlyTrashed);

    /**
     * @brief Returns metadata about image media.
     *
     * This will only be present for image types, and its contents will depend
     * on what can be parsed from the image content.
     */
    DriveFile::ImageMediaMetadataPtr imageMediaMetadata();

    /**
     * @brief Sets metadata about image media.
     *
     * This will only be present for image types, and its contents will depend
     * on what can be parsed from the image content.
     *
     * @param imageMediaMetadata
     */
    void setImageMediaMetadata(const ImageMediaMetadataPtr &imageMediaMetadata);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVEFILE_H
