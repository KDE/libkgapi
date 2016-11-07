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

#ifndef LIBKGAPI2_DRIVEFILE_H
#define LIBKGAPI2_DRIVEFILE_H

#include "object.h"
#include "types.h"
#include "change.h"

#include <QString>
#include <QStringList>
#include <QUrl>
#include <QVariantMap>
#include <QImage>

#include <QDateTime>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief File contains metadata for a file.
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/files">Files</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT File: public KGAPI2::Object
{
  private:
    class Private;

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
        bool starred() const;

        /**
         * @brief Sets whether this file is starred by the user.
         *
         * @param starred
         */
        void setStarred(bool starred);

        /**
         * @brief Returns whether this file has the 'hidden' label set.
         * @deprecated The 'hidden' label has been deprecated in the v2 api and removed in the v3 one.
         *             You can just ignore it.
         */
#ifndef KGAPIDRIVE_NO_DEPRECATED
        KGAPIDRIVE_DEPRECATED bool hidden() const;
#endif

        /**
         * @brief Sets whether this file has the 'hidden' label set.
         * @deprecated The 'hidden' label has been deprecated in the v2 api and removed in the v3 one.
         */
#ifndef KGAPIDRIVE_NO_DEPRECATED
        KGAPIDRIVE_DEPRECATED void setHidden(bool hidden);
#endif

        /**
         * @brief Returns whether this file has been trashed.
         */
        bool trashed() const;

        /**
         * @brief Sets whether this file has been trashed.
         *
         * @param trashed
         */
        void setTrashed(bool trashed);

        /**
         * @brief Returns whether viewers are prevented from downloading this file.
         */
        bool restricted() const;

        /**
         * @brief Sets whether viewers are prevented from downloading this file.
         *
         * @param restricted
         */
        void setRestricted(bool restricted);

        /**
         * @brief Returns whether this file has been viewed by this user.
         */
        bool viewed() const;

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
        friend class File::Private;
    };

    typedef QSharedPointer<Labels> LabelsPtr;
    typedef QList<LabelsPtr> LabelsList;

    /**
     * @brief DriveFile::IndexableText holds the structure used for indexableText property.
     */
    class IndexableText
    {
      public:
        explicit IndexableText(const IndexableText &other);
        virtual ~IndexableText();

        /**
         * @brief Returns the text to be indexed for this file.
         */
        QString text() const;

        /**
         * @brief Sets the text to be indexed for this file.
         *
         * @param text
         */
        void setText(const QString &text);

      private:
        explicit IndexableText();

        class Private;
        Private *const d;
        friend class Private;
        friend class File::Private;
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
            explicit Location(const Location &other);
            virtual ~Location();

            /**
             * @brief Returns the latitude stored in the image.
             */
            qreal latitude() const;

            /**
             * @brief Returns the longitude stored in the image.
             */
            qreal longitude() const;

            /**
             * @brief Returns the altitude stored in the image.
             */
            qreal altitude() const;

          private:
            explicit Location();

            class Private;
            Private *const d;
            friend class Private;
            friend class ImageMediaMetadata;
        };

        typedef QSharedPointer<Location> LocationPtr;

        explicit ImageMediaMetadata(const ImageMediaMetadata &other);
        virtual ~ImageMediaMetadata();

        /**
         * @brief Returns the width of the image in pixels.
         */
        int width() const;

        /**
         * @brief Returns the height of the image in pixels.
         */
        int height() const;

        /**
         * @brief Returns the rotation in clockwise degrees from the image's original orientation.
         */
        int rotation() const;

        /**
         * @brief Returns the geographic location information stored in the image.
         */
        LocationPtr location() const;

        QString date() const;

        QString cameraMake() const;

        QString cameraModel() const;

        float exposureTime() const;

        float aperture()  const;

        bool flashUsed() const;

        float focalLength() const;

        int isoSpeed() const;

        QString meteringMode() const;

        QString sensor() const;

        QString exposureMode() const;

        QString colorSpace() const;

        QString whiteBalance() const;

        float exposureBias() const;

        float maxApertureValue() const;

        int subjectDistance() const;

        QString lens() const;

      private:
        explicit ImageMediaMetadata(const QVariantMap &jsonMap);

        class Private;
        Private *const d;
        friend class Private;
        friend class File::Private;
    };

    typedef QSharedPointer<ImageMediaMetadata> ImageMediaMetadataPtr;

    class Thumbnail
    {
      public:
        explicit Thumbnail(const Thumbnail &other);
        virtual ~Thumbnail();

        QImage image() const;

        QString mimeType() const;

      private:
        explicit Thumbnail(const QVariantMap &jsonMap);

        class Private;
        Private * const d;
        friend class Private;
        friend class File::Private;
    };

    typedef QSharedPointer<Thumbnail> ThumbnailPtr;

    /**
     * @brief JSON serialization options.
     * @since 5.3.1
     */
    enum SerializationOption {
        NoOptions = 0,             ///< No option set.
        ExcludeCreationDate = 1    ///< Exclude 'createdDate' entry. This is necessary when renaming URLs.
    };
    Q_DECLARE_FLAGS(SerializationOptions, SerializationOption)

    explicit File();
    explicit File(const File &other);
    virtual ~File();

    /**
     * @brief Returns mimetype of folders
     */
    static QString folderMimeType();

    /**
     * @brief Returns the id of the file.
     */
    QString id() const;

    /**
     * @brief Returns a link back to this file.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns the title of this file.
     *
     * Used to identify file or folder name.
     */
    QString title() const;

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
    QString mimeType() const;

    /**
     * @brief Sets the MIME type of the file.
     *
     * @param mimeType
     */
    void setMimeType(const QString &mimeType);

    /**
     * @brief Returns a short description of the file.
     */
    QString description() const;

    /**
     * @brief Sets a short description of the file.
     *
     * @param description
     */
    void setDescription(const QString &description);

    /**
     * @brief Returns a group of labels for the file.
     */
    File::LabelsPtr labels() const;

    /**
     * @brief Sets a group of labels for the file.
     *
     * @param labels
     */
    void setLabels(const LabelsPtr &labels);

    /**
     * @brief Returns the create time for this file.
     */
    QDateTime createdDate() const;

    /**
     * @brief Returns the last time this file was modified by anyone.
     *
     * This is only mutable on update when the setModifiedDate parameter is set.
     */
    QDateTime modifiedDate() const;

    /**
     * @brief Sets the last time this file was modified by anyone.
     *
     * This is only mutable on update when the setModifiedDate parameter is set.
     *
     * @param modifiedDate
     */
    void setModifiedDate(const QDateTime &modifiedDate);

    /**
     * @brief Returns the last time this file was modified by the currently
     *        authenticated user.
     */
    QDateTime modifiedByMeDate() const;

    /**
     * @brief Returns a short lived download URL for the file.
     *
     * This is only populated for files with content stored in Drive.
     */
    QUrl downloadUrl() const;

    /**
     * @brief Returns the indexable text attributes for the file.
     *
     * This property can only be written, and is not returned by files.get
     */
    File::IndexableTextPtr& indexableText();

    /**
     * @brief Returns the permissions for the authenticated user on this file.
     */
    PermissionPtr userPermission() const;

    /**
     * @brief Returns the file extension used when downloading this file.
     *
     * This field is read only. To set the extension, include it on title when creating the file.
     * This is populated only for files with content stored in Drive.
     */
    QString fileExtension() const;

    /**
     * @brief Returns an MD5 checksum for the content of this file.
     *
     * This is populated only for files with content stored in Drive.
     */
    QString md5Checksum() const;

    /**
     * @brief Returns the size of the file in bytes.
     *
     * This is populated only for files with content stored in Drive.
     */
    qlonglong fileSize() const;

    /**
     * @brief Returns a link for opening the file in using a relevant
     *        Google editor or viewer.
     */
    QUrl alternateLink() const;

    /**
     * @brief Returns a link for embedding the file.
     */
    QUrl embedLink() const;

    /**
     * @brief Returns the time at which this file was shared with the user.
     */
    QDateTime sharedWithMeDate() const;

    /**
     * @brief Returns the collection of parent folders which contain this file.
     *
     * Setting this field will put the file in all of the provided folders.
     * On insert, if no folders are provided, the file will be placed in the
     * default root folder.
     */
    ParentReferencesList parents() const;

    /**
     * @brief Sets the collection of parent folders which contain this file.
     *
     * Setting this field will put the file in all of the provided folders.
     * On insert, if no folders are provided, the file will be placed in the
     * default root folder.
     *
     * @param parents
     */
    void setParents(const ParentReferencesList &parents);

    /**
     * @brief Returns the links for exporting Google Docs to specific formats.
     *
     * This is a map from the export format to URL.
     */
    QMap < QString /* format */, QUrl /* url */ > exportLinks() const;

    /**
     * @brief Returns the original filename if the file was uploaded manually,
     *        or the original title if the file was inserted through the API.
     *
     * Note that renames of the title will not change the original filename.
     * This will only be populated on files with content stored in Drive.
     */
    QString originalFileName() const;

    /**
     * @bried Returns the number of quota bytes used by this file.
     */
    qlonglong quotaBytesUsed() const;

    /**
     * @brief Return the name(s) of the owner(s) of this file.
     */
    QStringList ownerNames() const;

    /**
     * @brief Returns the name of the last user to modify this file.
     *
     * This will only be populated if a user has edited this file.
     */
    QString lastModifyingUserName() const;

    /**
     * @brief Returns whether the file can be edited by the current user.
     */
    bool editable() const;

    /**
     * @brief Returns whether writers can share the document with other users.
     */
    bool writersCanShare() const;

    /**
     * @brief Returns a link to the file's thumbnail.
     */
    QUrl thumbnailLink() const;

    /**
     * @brief Returns the last time this file was viewed by the user.
     */
    QDateTime lastViewedByMeDate() const;

    /**
     * @brief Sets the last time this file was viewed by the user.
     *
     * @param lastViewedByMeDate
     */
    void setLastViewedByMeDate(const QDateTime &lastViewedByMeDate);

    /**
     * @brief Returns a link for downloading the content of the file in a browser
     *        using cookie based authentication.
     *
     * In cases where the content is shared publicly, the content can be
     * downloaded without any credentials.
     */
    QUrl webContentLink() const;

    /**
     * @brief Returns whether this file has been explicitly trashed, as opposed
     *        to recursively trashed.
     *
     * This will only be populated if the file is trashed.
     */
    bool explicitlyTrashed() const;

    /**
     * @brief Returns metadata about image media.
     *
     * This will only be present for image types, and its contents will depend
     * on what can be parsed from the image content.
     */
    File::ImageMediaMetadataPtr imageMediaMetadata() const;

    /**
     * @brief Retrusn thumbnail for the file.
     */
    ThumbnailPtr thumbnail() const;

    QUrl webViewLink() const;

    QUrl iconLink() const;

    bool shared() const;

    UsersList owners() const;

    UserPtr lastModifyingUser() const;

    bool isFolder() const;

    static FilePtr fromJSON(const QByteArray &jsonData);
    static FilesList fromJSONFeed(const  QByteArray &jsonData, FeedData &feedData);
    static QByteArray toJSON(const FilePtr &file, SerializationOptions options = NoOptions);

    static FilePtr fromJSON(const QVariantMap &jsonData);

private:
    Private * const d;
    friend class Private;
    friend class Change::Private;
    friend class ParentReference;
    friend class Permission;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

Q_DECLARE_OPERATORS_FOR_FLAGS(KGAPI2::Drive::File::SerializationOptions)

#endif // LIBKGAPI2_DRIVEFILE_H
