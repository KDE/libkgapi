/*
    Copyright 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

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

#ifndef LIBKGAPI2_DRIVE_ABOUT_H
#define LIBKGAPI2_DRIVE_ABOUT_H

#include "types.h"
#include "object.h"
#include "kgapidrive_export.h"

#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QUrl>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief DriveAbout contains information about the current user along with
 *        Google Drive API settings.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/about">About</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT About: public KGAPI2::Object
{

public:
    /**
     * @brief DriveAbout::Format holds the structure used for importFormats[]
     *        and exportFormats[] properties.
     */
    class Format
    {
      public:
        explicit Format(const Format &other);
        virtual ~Format();

        /**
         * @brief Returns the content type to convert from.
         */
        QString source() const;

        /**
         * @brief Returns the possible content types to convert to.
         */
        QStringList targets() const;

      private:
        explicit Format();

        class Private;
        Private *const d;
        friend class Private;
        friend class About;
    };

    typedef QSharedPointer<Format> FormatPtr;
    typedef QList<FormatPtr> FormatsList;

    /**
     * @brief DriveAbout::AdditionalRoleInfo holds the structure used for
     *        additionalRoleInfo[] property.
     */
    class AdditionalRoleInfo
    {
      public:

        /**
         * @brief DriveAbout::AdditionalRoleInfo::Role holds the structure used for
         *        additionalRoleInfo[].roleSets[] property.
         */
        class RoleSet
        {
          public:
            explicit RoleSet(const RoleSet &other);
            virtual ~RoleSet();

            /**
             * @brief Returns the primary permission role.
             */
            QString primaryRole() const;

            /**
             * @brief Returns the supported additional roles with the primary role.
             */
            QStringList additionalRoles() const;

          private:
            explicit RoleSet();

            class Private;
            Private *const d;
            friend class Private;
            friend class About;
        };

        typedef QSharedPointer<RoleSet> RoleSetPtr;
        typedef QList<RoleSetPtr> RoleSetsList;

        explicit AdditionalRoleInfo(const AdditionalRoleInfo &other);
        virtual ~AdditionalRoleInfo();

        /**
         * @brief Returns the content type that this additional role info applies to.
         */
        QString type() const;

        /**
         * @brief Returns the supported additional roles per primary role.
         */
        RoleSetsList roleSets() const;

      private:
        explicit AdditionalRoleInfo();

        class Private;
        Private *const d;
        friend class Private;
        friend class About;
    };

    typedef QSharedPointer<AdditionalRoleInfo> AdditionalRoleInfoPtr;
    typedef QList<AdditionalRoleInfoPtr> AdditionalRoleInfosList;

    /**
     * @brief DriveAbout::Feature holds the structure used for features[] property.
     */
    class Feature
    {
      public:
        explicit Feature(const Feature &other);
        virtual ~Feature();

        /**
         * @brief Returns the name of the feature.
         */
        QString featureName() const;

        /**
         * @brief Returns the request limit rate for this feature, in queries per second.
         */
        qreal featureRate() const;

      private:
        explicit Feature();

        class Private;
        Private *const d;
        friend class Private;
        friend class About;
    };

    typedef QSharedPointer<Feature> FeaturePtr;
    typedef QList<FeaturePtr> FeaturesList;

    /**
     * @brief DriveAbout::MaxUploadSize holds the structure used for maxUploadSizes[] property.
     */
    class MaxUploadSize
    {
      public:
        explicit MaxUploadSize(const MaxUploadSize &other);
        virtual ~MaxUploadSize();

        /**
         * @brief Returns the file type.
         */
        QString type() const;

        /**
         * @brief Returns the max upload size for this type.
         */
        qlonglong size() const;


      private:
        explicit MaxUploadSize();

        class Private;
        Private *const d;
        friend class Private;
        friend class About;
    };

    typedef QSharedPointer<MaxUploadSize> MaxUploadSizePtr;
    typedef QList<MaxUploadSizePtr> MaxUploadSizesList;

    explicit About(const About &other);
    virtual ~About();

    /**
     * @brief Returns the link back to this item.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns the name of the current user.
     */
    QString name() const;

    /**
     * @brief Returns the total number of quota bytes.
     */
    qlonglong quotaBytesTotal() const;

    /**
     * @brief Returns the total number of quota bytes used.
     */
    qlonglong quotaBytesUsed() const;

    /**
     * @brief Returns the total number of quota bytes used by trashed items.
     */
    qlonglong quotaBytesUsedInTrash() const;

    /**
     * @brief returns the total number of quota bytes used by all Google apps
     *        (Drive, Picasa, etc.).
     * @since 5.3.2
     */
    qlonglong quotaBytesUsedAggregate() const;

    /**
     * @brief returns the total number of quota bytes used by all Google apps
     *        (Drive, Picasa, etc.).
     * @deprecated since 5.3.2, use quotaBytesUsedAggregate() instead.
     */
#ifndef KGAPIDRIVE_NO_DEPRECATED
    KGAPIDRIVE_DEPRECATED_EXPORT qlonglong quotaBytesUserAggregate() const;
#endif

    /**
     * @brief Returns the largest change id.
     */
    qlonglong largestChangeId() const;

    /**
     * @brief Returns the number of remaining change ids.
     */
    qlonglong remainingChangeIds() const;

    /**
     * @brief Returns the id of the root folder.
     */
    QString rootFolderId() const;

    /**
     * @brief Returns the domain sharing policy for the current user.
     */
    QString domainSharingPolicy() const;

    /**
     * @brief Returns the allowable import formats.
     */
    FormatsList importFormats() const;

    /**
     * @brief Returns the allowable export formats.
     */
    FormatsList exportFormats() const;

    /**
     * @brief Returns information about supported additional roles per file type.
     *
     * The most specific type takes precedence.
     */
    AdditionalRoleInfosList additionalRoleInfo() const;

    /**
     * @brief Returns the list of additional features enabled on this account.
     */
    FeaturesList features() const;

    /**
     * @brief Returns the list of max upload sizes for each file type.
     *
     * The most specific type takes precedence.
     */
    MaxUploadSizesList maxUploadSizes() const;

    /**
     * @brief Returns the current user's ID as visible in the permissions collection.
     */
    QString permissionId() const;

    /**
     * @brief Returns whether the authenticated app is installed by the authenticated user.
     */
    bool isCurrentAppInstalled() const;

    /**
     * @brief Returns the authenticated user.
     */
    UserPtr user() const;

    /**
     * @brief Constructs a new DriveAbout object from given JSON data
     *
     * @param jsonData
     */
    static AboutPtr fromJSON(const QByteArray &jsonData);

  private:
    explicit About();

    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_ABOUT_H
