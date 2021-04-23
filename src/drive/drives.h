/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "types.h"
#include "object.h"
#include "kgapidrive_export.h"

#include <QString>

#include <QDateTime>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief Drives contains a representation of a Drive.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/drives">Drives</a>
 *
 * @since 5.11.41
 * @author David Barchiesi <david@barchie.si>
 */
class KGAPIDRIVE_EXPORT Drives: public KGAPI2::Object
{

  public:

    /**
     * @brief Drives::Restrictions holds the structure used for
     * a set of restrictions that apply to this shared drive
     * or items inside this shared drive.
     */
    class Restrictions
    {

      public:
        struct Fields {
            static const QString AdminManagedRestrictions;
            static const QString CopyRequiresWriterPermission;
            static const QString DomainUsersOnly;
            static const QString DriveMembersOnly;
        };

        Restrictions();
        Restrictions(const Restrictions &other);
        ~Restrictions();
        bool operator==(const Restrictions &other) const;
        bool operator!=(const Restrictions &other) const { return !operator==(other); }

        /**
         * @brief Returns whether administrative privileges on this shared
         * drive are required to modify restrictions.
         */
        bool adminManagedRestrictions() const;

        /**
         * @brief Sets whether administrative privileges on this shared drive
         * are required to modify restrictions.
         *
         * @param adminManagedRestrictions
         */
        void setAdminManagedRestrictions(bool adminManagedRestrictions) const;

        /**
         * @brief Returns whether the options to copy, print, or download files
         * inside this shared drive, should be disabled for readers and commenters.
         * When this restriction is set to true, it will override the similarly
         * named field to true for any file inside this shared drive.
         */
        bool copyRequiresWriterPermission() const;

        /**
         * @brief Sets whether the options to copy, print, or download files
         * inside this shared drive, should be disabled for readers and commenters.
         * When this restriction is set to true, it will override the similarly
         * named field to true for any file inside this shared drive.
         *
         * @param copyRequiresWriterPermission
         */
        void setCopyRequiresWriterPermission(bool copyRequiresWriterPermission) const;

        /**
         * @brief Returns whether access to this shared drive and items inside
         * this shared drive is restricted to users of the domain to which this
         * shared drive belongs. This restriction may be overridden by other
         * sharing policies controlled outside of this shared drive.
         */
        bool domainUsersOnly() const;

        /**
         * @brief Sets whether access to this shared drive and items inside this
         * shared drive is restricted to users of the domain to which this
         * shared drive belongs. This restriction may be overridden by other
         * sharing policies controlled outside of this shared drive.
         *
         * @param domainUsersOnly
         */
        void setDomainUsersOnly(bool domainUsersOnly) const;

        /**
         * @brief Returns whether access to items inside this shared drive is
         * restricted to its members.
         */
        bool driveMembersOnly() const;

        /**
         * @brief Sets whether access to items inside this shared drive is
         * restricted to its members.
         *
         * @param driveMembersOnly
         */
        void setDriveMembersOnly(bool driveMembersOnly) const;

      private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Drives;
    };

    using RestrictionsPtr = QSharedPointer<Restrictions>;

    /**
     * @brief Drives::Capabilities holds the structure used for
     * capabilities the current user has on this shared drive.
     */
    class Capabilities
    {

      public:
        struct Fields {
            static const QString CanAddChildren;
            static const QString CanChangeCopyRequiresWriterPermissionRestriction;
            static const QString CanChangeDomainUsersOnlyRestriction;
            static const QString CanChangeDriveBackground;
            static const QString CanChangeDriveMembersOnlyRestriction;
            static const QString CanComment;
            static const QString CanCopy;
            static const QString CanDeleteChildren;
            static const QString CanDeleteDrive;
            static const QString CanDownload;
            static const QString CanEdit;
            static const QString CanListChildren;
            static const QString CanManageMembers;
            static const QString CanReadRevisions;
            static const QString CanRename;
            static const QString CanRenameDrive;
            static const QString CanShare;
            static const QString CanTrashChildren;
        };

        Capabilities();
        Capabilities(const Capabilities &other);
        ~Capabilities();
        bool operator==(const Capabilities &other) const;
        bool operator!=(const Capabilities &other) const { return !operator==(other); }

        /**
         * @brief Returns whether the current user can add children to folders
         * in this shared drive.
         */
        bool canAddChildren() const;

        /**
         * @brief Returns whether the current user can change the
         * copyRequiresWriterPermission restriction of this shared drive.
         */
        bool canChangeCopyRequiresWriterPermissionRestriction() const;

        /**
         * @brief Returns whether the current user can change the domainUsersOnly
         * restriction of this shared drive.
         */
        bool canChangeDomainUsersOnlyRestriction() const;

        /**
         * @brief Returns whether the current user can change the background of
         * this shared drive.
         */
        bool canChangeDriveBackground() const;

        /**
         * @brief Returns whether the current user can change the driveMembersOnly
         * restriction of this shared drive.
         */
        bool canChangeDriveMembersOnlyRestriction() const;

        /**
         * @brief Returns Whether the current user can comment on files in
         * this shared drive.
         */
        bool canComment() const;

        /**
         * @brief Returns Whether the current user can copy files in this shared drive.
         */
        bool canCopy() const;

        /**
         * @brief Returns Whether the current user can delete children from
         * folders in this shared drive.
         */
        bool canDeleteChildren() const;

        /**
         * @brief Returns Whether the current user can delete this shared drive.
         *
         * Attempting to delete the shared drive may still fail if there are
         * untrashed items inside the shared drive.
         */
        bool canDeleteDrive() const;

        /**
         * @brief Returns Whether the current user can download files in this
         * shared drive.
         */
        bool canDownload() const;

        /**
         * @brief Returns Whether the current user can edit files in this
         * shared drive
         */
        bool canEdit() const;

        /**
         * @brief Returns Whether the current user can list the children of
         * folders in this shared drive.
         */
        bool canListChildren() const;

        /**
         * @brief Returns Whether the current user can add members to this shared drive
         * or remove them or change their role.
         */
        bool canManageMembers() const;

        /**
         * @brief Returns Whether the current user can read the revisions
         * resource of files in this shared drive.
         */
        bool canReadRevisions() const;

        /**
         * @brief Returns Whether the current user can rename files or folders
         * in this shared drive.
         */
        bool canRename() const;

        /**
         * @brief Returns Whether the current user can rename this shared drive.
         */
        bool canRenameDrive() const;

        /**
         * @brief Returns Whether the current user can share files or folders
         * in this shared drive.
         */
        bool canShare() const;

        /**
         * @brief Returns Whether the current user can trash children from
         * folders in this shared drive.
         */
        bool canTrashChildren() const;

      private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Drives;
    };

    using CapabilitiesPtr = QSharedPointer<Capabilities>;

    /**
     * @brief Drives::BackgroundImageFile holds the structure used
     * for backgroundImageFile property.
     */
    class BackgroundImageFile
    {

      public:
        struct Fields {
            static const QString Id;
            static const QString XCoordinate;
            static const QString YCoordinate;
            static const QString Width;
        };

        BackgroundImageFile();
        BackgroundImageFile(const BackgroundImageFile &other);
        ~BackgroundImageFile();
        bool operator==(const BackgroundImageFile &other) const;
        bool operator!=(const BackgroundImageFile &other) const { return !operator==(other); }

        /**
         * @brief Returns the ID of an image file in Google Drive
         * to use for the background image.
         */
        QString id() const;

        /**
         * @brief Sets the ID of an image file in Google Drive to
         * use for the background image.
         *
         * @param id
         */
        void setId(const QString &id) const;

        /**
         * @brief Returns the X coordinate of the upper left corner of
         * the cropping area in the background image. This is a value
         * in the closed range of 0 to 1. This value represents the
         * horizontal distance from the left side of the entire image
         * to the left side of the cropping area divided by the width
         * of the entire image.
         */
        float xCoordinate() const;

        /**
         * @brief Returns the X coordinate of the upper left corner of
         * the cropping area in the background image. This is a value
         * in the closed range of 0 to 1. This value represents the
         * horizontal distance from the left side of the entire image
         * to the left side of the cropping area divided by the width
         * of the entire image.
         *
         * @param xCoordinate
         */
        void setXCoordinate(float xCoordinate) const;

        /**
         * @brief Returns the Y coordinate of the upper left corner of the
         * cropping area in the background image. This is a value in the
         * closed range of 0 to 1. This value represents the vertical
         * distance from the top side of the entire image to the top side
         * of the cropping area divided by the height of the entire image.
         */
        float yCoordinate() const;

        /**
         * @brief Sets the Y coordinate of the upper left corner of the
         * cropping area in the background image. This is a value in the
         * closed range of 0 to 1. This value represents the vertical
         * distance from the top side of the entire image to the top side
         * of the cropping area divided by the height of the entire image.
         *
         * @param yCoordinate
         */
        void setYCoordinate(float yCoordinate) const;

        /**
         * @brief Returns the width of the cropped image in the closed
         * range of 0 to 1. This value represents the width of the cropped
         * image divided by the width of the entire image. The height is
         * computed by applying a width to height aspect ratio of 80 to 9.
         * The resulting image must be at least 1280 pixels wide and
         * 144 pixels high.
         */
        float width() const;

        /**
         * @brief Sets the width of the cropped image in the closed
         * range of 0 to 1. This value represents the width of the cropped
         * image divided by the width of the entire image. The height is
         * computed by applying a width to height aspect ratio of 80 to 9.
         * The resulting image must be at least 1280 pixels wide and
         * 144 pixels high.
         *
         * @param width
         */
        void setWidth(float width) const;

      private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Drives;
    };

    using BackgroundImageFilePtr = QSharedPointer<BackgroundImageFile>;

    struct Fields {
        static const QString Kind;
        static const QString Items;
        static const QString KindDrive;
        static const QString PageToken;
        static const QString NextPageToken;
        static const QString Id;
        static const QString Name;
        static const QString ThemeId;
        static const QString ColorRgb;
        static const QString BackgroundImageFile;
        static const QString BackgroundImageLink;
        static const QString Capabilities;
        static const QString CreatedDate;
        static const QString Hidden;
        static const QString Restrictions;
    };

    Drives();
    Drives(const Drives &other);
    ~Drives() override;
    bool operator==(const Drives &other) const;
    bool operator!=(const Drives &other) const { return !operator==(other); }

    /**
     * @brief Returns the ID of this shared drive which
     * is also the ID of the top level folder of this
     * shared drive.
     */
    QString id() const;

    /**
     * @brief Sets the ID of this shared drive which
     * is also the ID of the top level folder of this
     * shared drive.
     *
     * @param id
     */
    void setId(const QString &id) const;

    /**
     * @brief Returns the name of the drive.
     */
    QString name() const;

    /**
     * @brief Sets the name of the drive.
     *
     * @param name
     */
    void setName(const QString &name) const;

    /**
     * @brief Returns the themeId of the drive.
     */
    QString themeId() const;

    /**
     * @brief Sets the themeId of the shared drive.
     *
     * @param themeId
     */
    void setThemeId(const QString &themeId) const;

    /**
     * @brief Returns the colorRgb of the shared drive.
     */
    QString colorRgb() const;

    /**
     * @brief Sets the colorRgb of the shared drive.
     *
     * @param colorRgb
     */
    void setColorRgb(const QString &colorRgb) const;

    /**
     * @brief Returns the image file and cropping parameters from which a background image for this shared drive is set.
     */
    BackgroundImageFilePtr backgroundImageFile() const;

    /**
     * @brief Sets the backgroundImageFile of the shared drive.
     *
     * @param backgroundImageFile
     */
    void setBackgroundImageFile(const BackgroundImageFilePtr &backgroundImageFile) const;

    /**
     * @brief Returns the backgroundImageLink of the shared drive.
     */
    QString backgroundImageLink() const;

    /**
     * @brief Returns the capabilities the current user has on this shared drive.
     */
    CapabilitiesPtr capabilities() const;

    /**
     * @brief Returns the time at which the shared drive was created.
     */
    QDateTime createdDate() const;

    /**
     * @brief Returns whether the shared drive is hidden from default view.
     */
    bool hidden() const;

    /**
     * @brief Returns the set of restrictions that apply to this shared drive or
     * items inside this shared drive.
     */
    RestrictionsPtr restrictions() const;

    /**
     * @brief Sets the restrictions of the shared drive.
     *
     * @param restrictions
     */
    void setRestrictions(const RestrictionsPtr &restrictions) const;

    static DrivesPtr fromJSON(const QByteArray &jsonData);
    static DrivesList fromJSONFeed(const QByteArray &jsonData, FeedData &feedData);
    static QByteArray toJSON(const DrivesPtr &drives);

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

