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

#ifndef LIBKGAPI_OBJECTS_DRIVEABOUT_H
#define LIBKGAPI_OBJECTS_DRIVEABOUT_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DriveAboutPrivate;
class DriveAboutPrivate_Format;
class DriveAboutPrivate_AdditionalRoleInfo;
class DriveAboutPrivate_AdditionalRoleInfo_RoleSet;
class DriveAboutPrivate_Feature;
class DriveAboutPrivate_MaxUploadSize;


/**
 * DriveAbout contains information about the current user along with Google Drive API settings.
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/about">About</a>
 */
class LIBKGAPI_EXPORT DriveAbout: public KGAPI::Object
{
public:
    /**
     * DriveAbout::Format holds the structure used for importFormats[] and exportFormats[] properties.
     */
    class Format
    {
    public:
        Format();
        Format(const Format& other);
        virtual ~Format();
        
        /**
         * Returns the content type to convert from.
         */
        QString source();
        
        /**
         * Sets the content type to convert from.
         */
        void setSource(const QString& source);

        /**
         * Returns the possible content types to convert to.
         */
        QStringList targets();
        
        /** 
         * Sets the possible content types to convert to.
         */
        void setTargets(const QStringList& targets);
    private:
        DriveAboutPrivate_Format* d;
    };
    
    /**
     * DriveAbout::AdditionalRoleInfo holds the structure used for additionalRoleInfo[] property.
     */
    class AdditionalRoleInfo
    {
    public:
        /**
         * DriveAbout::AdditionalRoleInfo::Role holds the structure used for additionalRoleInfo[].roleSets[] property.
         */
        class RoleSet
        {
        public:
            RoleSet();
            RoleSet(const RoleSet& other);
            virtual ~RoleSet();
            
            /**
             * Returns the primary permission role.
             */
            QString primaryRole();
            
            /**
             * Sets the primary permission role.
             */
            void setPrimaryRole(const QString& primaryRole);
            
            /**
             * Returns the supported additional roles with the primary role.
             */
            QString additionalRoles();
            
            /**
             * Sets the supported additional roles with the primary role.
             */
            void setAdditionalRoles(const QString& additionalRoles);
            
        private:
            DriveAboutPrivate_AdditionalRoleInfo_RoleSet* d;
        };
        
        AdditionalRoleInfo();
        AdditionalRoleInfo(const AdditionalRoleInfo& other);
        virtual ~AdditionalRoleInfo();
        
        /**
         * Returns the content type that this additional role info applies to.
         */
        QString type();
        
        /**
         * Sets the content type that this additional role info applies to.
         */
        void setType(const QString& type);
        
        /**
         * Returns the supported additional roles per primary role.
         */
        QList<RoleSet> roleSets();
        
        /**
         * Sets the supported additional roles per primary role.
         */
        void setRoleSets(const QList<RoleSet>& roleSets);
        
    private:
        DriveAboutPrivate_AdditionalRoleInfo* d;
    };
    
    /**
     * DriveAbout::Feature holds the structure used for features[] property.
     */
    class Feature
    {
    public:
        Feature();
        Feature(const Feature& other);
        virtual ~Feature();
        
        /**
         * Returns the name of the feature.
         */
        QString featureName();
        
        /**
         * Sets the name of the feature.
         */
        void setFeatureName(const QString& featureName);
        
        /**
         * Returns the request limit rate for this feature, in queries per second.
         */
        qreal featureRate();
        
        /**
         * Sets the request limit rate for this feature, in queries per second.
         */
        void setFeatureRate(const qreal& featureRate);
    private:
        DriveAboutPrivate_Feature* d;
    };
    
    /**
     * DriveAbout::MaxUploadSize holds the structure used for maxUploadSizes[] property.
     */
    class MaxUploadSize
    {
    public:
        MaxUploadSize();
        MaxUploadSize(const MaxUploadSize& other);
        virtual ~MaxUploadSize();
        
        /**
         * Returns the file type.
         */
        QString type();
        
        /**
         * Sets the file type.
         */
        void setType(const QString& type);
        
        /**
         * Returns the max upload size for this type.
         */
        qulonglong size();
        
        /**
         * Sets the max upload size for this type.
         */
        void setSize(const qulonglong& size);
        
    private:
        DriveAboutPrivate_MaxUploadSize* d;
    };
    
    typedef QSharedPointer< DriveAbout > Ptr;
    typedef QList< DriveAbout > List;
    
    DriveAbout();
    DriveAbout(const DriveAbout& other);
    virtual ~DriveAbout();
    
    /**
     * Returns the link back to this item.
     */
    QString selfLink();
    
    /**
     * Sets the link back to this item.
     */
    void setSelfLink(const QString& selfLink);
    
    /**
     * Returns the name of the current user.
     */
    QString name();
    
    /**
     * Sets the name of the current user.
     */
    void setName(const QString& name);
    
    /**
     * Returns the total number of quota bytes.
     */
    qulonglong quotaBytesTotal();
    
    /**
     * Sets the total number of quota bytes.
     */
    void setQuotaBytesTotal(const qulonglong& quotaBytesTotal);
    
    /**
     * Returns the total number of quota bytes used.
     */
    qulonglong quotaBytesUsed();
    
    /**
     * Sets the total number of quota bytes used.
     */
    void setQuotaBytesUsed(const qulonglong& quotaBytesUsed);
    
    /**
     * Returns the total number of quota bytes used by trashed items.
     */
    qulonglong quotaBytesUsedInTrash();
    
    /**
     * Sets the total number of quota bytes used by trashed items.
     */
    void setQuotaBytesUsedInTrash(const qulonglong& quotaBytesUsedInTrash);
    
    /**
     * Returns the largest change id.
     */
    qulonglong largestChangeId();
    
    /**
     * Sets the largest change id.
     */
    void setLargestChangeId(const qulonglong& largestChangeId);
    
    /**
     * Returns the number of remaining change ids.
     */
    qulonglong remainingChangeIds();
    
    /**
     * Sets the number of remaining change ids.
     */
    void setRemainingChangeIds(const qulonglong& remainingChangeIds);
    
    /**
     * Returns the id of the root folder.
     */
    QString rootFolderId();
    
    /**
     * Sets the id of the root folder.
     */
    void setRootFolderId(const QString& rootFolderId);
    
    /**
     * Returns the domain sharing policy for the current user.
     */
    QString domainSharingPolicy();
    
    /**
     * Sets the domain sharing policy for the current user.
     */
    void setDomainSharingPolicy(const QString& domainSharingPolicy);
    
    /**
     * Returns the allowable import formats.
     */
    QList<Format> importFormats();
    
    /**
     * Sets the allowable import formats.
     */
    void setImportFormats(const QList<Format>& importFormats);
    
    /**
     * Returns the allowable export formats.
     */
    QList<Format> exportFormats();
    
    /**
     * Sets the allowable export formats.
     */
    void setExportFormats(const QList<Format>& exportFormats);
    
    /**
     * Returns information about supported additional roles per file type.
     * The most specific type takes precedence.
     */
    QList<AdditionalRoleInfo> additionalRoleInfo();
    
    /**
     * Sets information about supported additional roles per file type.
     * The most specific type takes precedence.
     */
    void setAdditionalRoleInfo(const QList<AdditionalRoleInfo>& additionalRoleInfo);
    
    /**
     * Returns the list of additional features enabled on this account.
     */
    QList<Feature> features();
    
    /**
     * Sets the list of additional features enabled on this account.
     */
    void setFeatures(const QList<Feature>& features);
    
    /**
     * Returns the list of max upload sizes for each file type.
     * The most specific type takes precedence.
     */
    QList<MaxUploadSize> maxUploadSizes();
    
    /**
     * Sets the list of max upload sizes for each file type.
     * The most specific type takes precedence.
     */
    void setMaxUploadSizes(const QList<MaxUploadSize>& maxUploadSizes);
    
    /**
     * Returns the current user's ID as visible in the permissions collection.
     */
    QString permissionId();
    
    /**
     * Sets the current user's ID as visible in the permissions collection.
     */
    void setPermissionId(const QString& permissionId);
    
    /**
     * Returns whether the authenticated app is installed by the authenticated user.
     */
    bool isCurrentAppInstalled();
    
    /**
     * Sets whether the authenticated app is installed by the authenticated user.
     */
    void setIsCurrentAppInstalled(const bool& isCurrentAppInstalled);
    
private:
    DriveAboutPrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveAbout::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveAbout*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveAbout::List)

#endif // LIBKGAPI_OBJECTS_DRIVEABOUT_H
