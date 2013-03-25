/*
    Copyright 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

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

#ifndef LIBKGAPI2_DRIVE_ABOUT_H
#define LIBKGAPI2_DRIVE_ABOUT_H

#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>

namespace KGAPI2
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
class LIBKGAPI2_EXPORT DriveAbout
{

public:
    /**
     * @brief DriveAbout::Format holds the structure used for importFormats[]
     *        and exportFormats[] properties.
     */
    class Format
    {
      public:
        explicit Format();
        explicit Format(const Format &other);
        virtual ~Format();

        /**
         * @brief Returns the content type to convert from.
         */
        QString source() const;

        /**
         * @brief Sets the content type to convert from.
         *
         * @param source
         */
        void setSource(const QString &source);

        /**
         * @brief Returns the possible content types to convert to.
         */
        QStringList targets() const;

        /**
         * @brief Sets the possible content types to convert to.
         *
         * @param targets
         */
        void setTargets(const QStringList &targets);

      private:
        class Private;
        Private *const d;
        friend class Private;
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
            explicit RoleSet();
            explicit RoleSet(const RoleSet &other);
            virtual ~RoleSet();

            /**
             * @brief Returns the primary permission role.
             */
            QString primaryRole() const;

            /**
             * @brief Sets the primary permission role.
             *
             * @param primaryRole
             */
            void setPrimaryRole(const QString &primaryRole);

            /**
             * @brief Returns the supported additional roles with the primary role.
             */
            QString additionalRoles() const;

            /**
             * @brief Sets the supported additional roles with the primary role.
             *
             * @param additionalRoles
             */
            void setAdditionalRoles(const QString &additionalRoles);

          private:
            class Private;
            Private *const d;
            friend class Private;
        };

        typedef QSharedPointer<RoleSet> RoleSetPtr;
        typedef QList<RoleSetPtr> RoleSetsList;

        explicit AdditionalRoleInfo();
        explicit AdditionalRoleInfo(const AdditionalRoleInfo &other);
        virtual ~AdditionalRoleInfo();

        /**
         * @brief Returns the content type that this additional role info applies to.
         */
        QString type() const;

        /**
         * @brief Sets the content type that this additional role info applies to.
         *
         * @param type
         */
        void setType(const QString &type);

        /**
         * @brief Returns the supported additional roles per primary role.
         */
        RoleSetsList roleSets() const;

        /**
         * @brief Sets the supported additional roles per primary role.
         *
         * @param roleSets
         */
        void setRoleSets(const RoleSetsList &roleSets);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<AdditionalRoleInfo> AdditionalRoleInfoPtr;
    typedef QList<AdditionalRoleInfoPtr> AdditionalRoleInfosList;

    /**
     * @brief DriveAbout::Feature holds the structure used for features[] property.
     */
    class Feature
    {
      public:
        explicit Feature();
        explicit Feature(const Feature &other);
        virtual ~Feature();

        /**
         * @brief Returns the name of the feature.
         */
        QString featureName() const;

        /**
         * @brief Sets the name of the feature.
         *
         * @param featureName
         */
        void setFeatureName(const QString &featureName);

        /**
         * @brief Returns the request limit rate for this feature, in queries per second.
         */
        qreal featureRate() const;

        /**
         * @brief Sets the request limit rate for this feature, in queries per second.
         *
         * @param featureRate
         */
        void setFeatureRate(qreal featureRate);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<Feature> FeaturePtr;
    typedef QList<FeaturePtr> FeaturesList;

    /**
     * @brief DriveAbout::MaxUploadSize holds the structure used for maxUploadSizes[] property.
     */
    class MaxUploadSize
    {
      public:
        explicit MaxUploadSize();
        explicit MaxUploadSize(const MaxUploadSize &other);
        virtual ~MaxUploadSize();

        /**
         * @brief Returns the file type.
         */
        QString type() const;

        /**
         * @brief Sets the file type.
         *
         * @param type
         */
        void setType(const QString &type);

        /**
         * @brief Returns the max upload size for this type.
         */
        qlonglong size() const;

        /**
         * @brief Sets the max upload size for this type.
         *
         * @param size
         */
        void setSize(qlonglong size);

      private:
        class Private;
        Private *const d;
        friend class Private;
    };

    typedef QSharedPointer<MaxUploadSize> MaxUploadSizePtr;
    typedef QList<MaxUploadSizePtr> MaxUploadSizesList;

    explicit DriveAbout();
    explicit DriveAbout(const DriveAbout &other);
    virtual ~DriveAbout();

    /**
     * @brief Returns the link back to this item.
     */
    QUrl selfLink() const;

    /**
     * @brief Sets the link back to this item.
     *
     * @param selfLink
     */
    void setSelfLink(const QUrl &selfLink);

    /**
     * @brief Returns the name of the current user.
     */
    QString name() const;

    /**
     * @brief Sets the name of the current user.
     *
     * @param name
     */
    void setName(const QString &name);

    /**
     * @brief Returns the total number of quota bytes.
     */
    qlonglong quotaBytesTotal() const;

    /**
     * @brief Sets the total number of quota bytes.
     *
     * @param quotaBytesTotal
     */
    void setQuotaBytesTotal(qlonglong quotaBytesTotal);

    /**
     * @brief Returns the total number of quota bytes used.
     */
    qlonglong quotaBytesUsed() const;

    /**
     * @brief Sets the total number of quota bytes used.
     *
     * @param quotaBytesUsed
     */
    void setQuotaBytesUsed(qlonglong quotaBytesUsed);

    /**
     * @brief Returns the total number of quota bytes used by trashed items.
     */
    qlonglong quotaBytesUsedInTrash() const;

    /**
     * @brief Sets the total number of quota bytes used by trashed items.
     *
     * @param quotaBytesUsedInTrash
     */
    void setQuotaBytesUsedInTrash(qlonglong quotaBytesUsedInTrash);

    /**
     * @brief Returns the largest change id.
     */
    qlonglong largestChangeId() const;

    /**
     * @brief Sets the largest change id.
     *
     * @param largestChangeId
     */
    void setLargestChangeId(qlonglong largestChangeId);

    /**
     * @brief Returns the number of remaining change ids.
     */
    qlonglong remainingChangeIds() const;

    /**
     * @brief Sets the number of remaining change ids.
     *
     * @param remainingChangeIds
     */
    void setRemainingChangeIds(qlonglong remainingChangeIds);

    /**
     * @brief Returns the id of the root folder.
     */
    QString rootFolderId() const;

    /**
     * @brief Sets the id of the root folder.
     *
     * @param rootFolderId
     */
    void setRootFolderId(const QString &rootFolderId);

    /**
     * @brief Returns the domain sharing policy for the current user.
     */
    QString domainSharingPolicy() const;

    /**
     * @brief Sets the domain sharing policy for the current user.
     *
     * @param domainSharingPolicy
     */
    void setDomainSharingPolicy(const QString &domainSharingPolicy);

    /**
     * @brief Returns the allowable import formats.
     */
    FormatsList importFormats() const;

    /**
     * @brief Sets the allowable import formats.
     *
     * @para importFormats
     */
    void setImportFormats(const FormatsList &importFormats);

    /**
     * @brief Returns the allowable export formats.
     */
    FormatsList exportFormats() const;

    /**
     * @brief Sets the allowable export formats.
     *
     * @param exportFormats
     */
    void setExportFormats(const FormatsList &exportFormats);

    /**
     * @brief Returns information about supported additional roles per file type.
     *
     * The most specific type takes precedence.
     */
    AdditionalRoleInfosList additionalRoleInfo() const;

    /**
     * @brief Sets information about supported additional roles per file type.
     *
     * The most specific type takes precedence.
     *
     * @param additionalRoleInfo
     */
    void setAdditionalRoleInfo(const AdditionalRoleInfosList &additionalRoleInfo);

    /**
     * @brief Returns the list of additional features enabled on this account.
     */
    FeaturesList features() const;

    /**
     * @brief Sets the list of additional features enabled on this account.
     *
     * @param features
     */
    void setFeatures(const FeaturesList &features);

    /**
     * @brief Returns the list of max upload sizes for each file type.
     *
     * The most specific type takes precedence.
     */
    MaxUploadSizesList maxUploadSizes() const;

    /**
     * @brief Sets the list of max upload sizes for each file type.
     *
     * The most specific type takes precedence.
     *
     * @param maxUploadSizes
     */
    void setMaxUploadSizes(const MaxUploadSizesList &maxUploadSizes);

    /**
     * @brief Returns the current user's ID as visible in the permissions collection.
     */
    QString permissionId() const;

    /**
     * @brief Sets the current user's ID as visible in the permissions collection.
     *
     * @param permissionId
     */
    void setPermissionId(const QString &permissionId);

    /**
     * @brief Returns whether the authenticated app is installed by the authenticated user.
     */
    bool isCurrentAppInstalled() const;

    /**
     * @brief Sets whether the authenticated app is installed by the authenticated user.
     *
     * @param isCurrentAppInstalled
     */
    void setIsCurrentAppInstalled(bool isCurrentAppInstalled);

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVE_ABOUT_H
