/*
    Copyright (C) 2019  David Barchiesi <david@barchie.si>

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
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI2_DRIVETEAMDRIVE_H
#define LIBKGAPI2_DRIVETEAMDRIVE_H

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
 * @brief Teamdrive contains a representation of a Team Drive.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/teamdrives">Teamdrives</a>
 *
 * @since x.0
 * @author David Barchiesi <david@barchie.si>
 */
class KGAPIDRIVE_EXPORT Teamdrive: public KGAPI2::Object
{

  public:

    /**
     * @brief DriveTeamdrive::Restrictions holds the structure used for
     * restrictions property.
     */
    class Restrictions
    {

      public:
        Restrictions();
        Restrictions(const Restrictions &other);
        ~Restrictions();
        bool operator==(const Restrictions &other) const;
        bool operator!=(const Restrictions &other) const { return !operator==(other); }

        /**
         * @brief Returns whether administrative privileges on this Team Drive
         * are required to modify restrictions.
         */
        bool adminManagedRestrictions() const;

        /**
         * @brief Returns whether the options to copy, print, or download files
         * inside this Team Drive, should be disabled for readers and commenters.
         * When this restriction is set to true, it will override the similarly
         * named field to true for any file inside this Team Drive.
         */
        bool copyRequiresWriterPermission() const;

        /**
         * @brief Returns whether access to this Team Drive and items inside this
         * Team Drive is restricted to users of the domain to which this Team
         * Drive belongs. This restriction may be overridden by other sharing
         * policies controlled outside of this Team Drive.
         */
        bool domainUsersOnly() const;

        /**
         * @brief Returns whether access to items inside this Team Drive is
         * restricted to members of this Team Drive.
         */
        bool teamMembersOnly() const;

      private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Teamdrive;
    };

    typedef QSharedPointer<Restrictions> RestrictionsPtr;

    /**
     * @brief DriveTeamdrive::Capabilities holds the structure used for capabilities property.
     */
    class Capabilities
    {

      public:
        Capabilities();
        Capabilities(const Capabilities &other);
        ~Capabilities();
        bool operator==(const Capabilities &other) const;
        bool operator!=(const Capabilities &other) const { return !operator==(other); }

        /**
         * @brief Returns whether the current user can add children to folders
         * in this Team Drive.
         */
        bool canAddChildren() const;

        /**
         * @brief Returns whether the current user can change the
         * copyRequiresWriterPermission restriction of this Team Drive.
         */
        bool canChangeCopyRequiresWriterPermissionRestriction() const;

        /**
         * @brief Returns whether the current user can change the domainUsersOnly
         * restriction of this Team Drive.
         */
        bool canChangeDomainUsersOnlyRestriction() const;

        /**
         * @brief Returns whether the current user can change the background of
         * this Team Drive.
         */
        bool canChangeTeamDriveBackground() const;

        /**
         * @brief Returns whether the current user can change the teamMembersOnly
         * restriction of this Team Drive.
         */
        bool canChangeTeamMembersOnlyRestriction() const;

        /**
         * @brief Returns Whether the current user can comment on files in
         * this Team Drive.
         */
        bool canComment() const;

        /**
         * @brief Returns Whether the current user can copy files in this Team Drive.
         */
        bool canCopy() const;

        /**
         * @brief Returns Whether the current user can delete children from
         * folders in this Team Drive.
         */
        bool canDeleteChildren() const;

        /**
         * @brief Returns Whether the current user can delete this Team Drive.
         *
         * Attempting to delete the Team Drive may still fail if there are
         * untrashed items inside the Team Drive.
         */
        bool canDeleteTeamDrive() const;

        /**
         * @brief Returns Whether the current user can download files in this
         * Team Drive.
         */
        bool canDownload() const;

        /**
         * @brief Returns Whether the current user can edit files in this
         * Team Drive
         */
        bool canEdit() const;

        /**
         * @brief Returns Whether the current user can list the children of
         * folders in this Team Drive.
         */
        bool canListChildren() const;

        /**
         * @brief Returns Whether the current user can add members to this Team Drive
         * or remove them or change their role.
         */
        bool canManageMembers() const;

        /**
         * @brief Returns Whether the current user can read the revisions
         * resource of files in this Team Drive.
         */
        bool canReadRevisions() const;

        /**
         * @brief Returns Whether the current user can rename files or folders
         * in this Team Drive.
         */
        bool canRename() const;

        /**
         * @brief Returns Whether the current user can rename this Team Drive.
         */
        bool canRenameTeamDrive() const;

        /**
         * @brief Returns Whether the current user can share files or folders
         * in this Team Drive.
         */
        bool canShare() const;

        /**
         * @brief Returns Whether the current user can trash children from
         * folders in this Team Drive.
         */
        bool canTrashChildren() const;

      private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Teamdrive;
    };

    typedef QSharedPointer<Capabilities> CapabilitiesPtr;

    /**
     * @brief DriveTeamdrive::BackgroundImageFile holds the structure used
     * for backgroundImageFile property.
     */
    class BackgroundImageFile
    {

      public:
        BackgroundImageFile();
        BackgroundImageFile(const BackgroundImageFile &other);
        ~BackgroundImageFile();
        bool operator==(const BackgroundImageFile &other) const;
        bool operator!=(const BackgroundImageFile &other) const { return !operator==(other); }

        /**
         * @brief Returns the id of the background image file.
         */
        QString id() const;

        /**
         * @brief Returns the x coordinate for this background image file.
         */
        float xCoordinate() const;

        /**
         * @brief Returns the y coordinate for this background image file.
         */
        float yCoordinate() const;

        /**
         * @brief Returns the width for this background image file.
         */
        float width() const;

      private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Teamdrive;
    };

    typedef QSharedPointer<BackgroundImageFile> BackgroundImageFilePtr;

    Teamdrive();
    Teamdrive(const Teamdrive &other);
    ~Teamdrive() override;
    bool operator==(const Teamdrive &other) const;
    bool operator!=(const Teamdrive &other) const { return !operator==(other); }

    /**
     * @brief Returns the id of the teamdrive.
     */
    QString id() const;

    /**
     * @brief Returns the name of the teamdrive.
     */
    QString name() const;

    /**
     * @brief Returns the themeId of the teamdrive.
     */
    QString themeId() const;

    /**
     * @brief Returns the colorRgb of the teamdrive.
     */
    QString colorRgb() const;

    /**
     * @brief Returns the image file and cropping parameters from which a background image for this Team Drive is set.
     */
    BackgroundImageFilePtr backgroundImageFile() const;

    /**
     * @brief Returns the backgroundImageLink of the teamdrive.
     */
    QString backgroundImageLink() const;

    /**
     * @brief Returns the capabilities the current user has on this Team Drive.
     */
    CapabilitiesPtr capabilities() const;

    /**
     * @brief Returns the time at which the Team Drive was created.
     */
    QDateTime createdDate() const;

    /**
     * @brief Returns the set of restrictions that apply to this Team Drive or
     * items inside this Team Drive.
     */
    RestrictionsPtr restrictions() const;

    static TeamdrivePtr fromJSON(const QByteArray &jsonData);
    static TeamdrivesList fromJSONFeed(const QByteArray &jsonData, FeedData &feedData);
    static QByteArray toJSON(const TeamdrivePtr &teamdrive);

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVETEAMDRIVE_H
