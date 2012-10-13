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

class LIBKGAPI_EXPORT DriveAbout: public KGAPI::Object
{
public:
    class Format
    {
    public:
        Format();
        Format(const Format& other);
        virtual ~Format();
        
        QString source();
        void setSource(const QString& source);

        QStringList targets();
        void setTargets(const QStringList& targets);
    private:
        DriveAboutPrivate_Format* d;
    };
    
    class AdditionalRoleInfo
    {
    public:
        class RoleSet
        {
        public:
            RoleSet();
            RoleSet(const RoleSet& other);
            virtual ~RoleSet();
            
            QString primaryRole();
            void setPrimaryRole(const QString& primaryRole);
            
            QString additionalRoles();
            void setAdditionalRoles(const QString& additionalRoles);
        private:
            DriveAboutPrivate_AdditionalRoleInfo_RoleSet* d;
        };
        
        AdditionalRoleInfo();
        AdditionalRoleInfo(const AdditionalRoleInfo& other);
        virtual ~AdditionalRoleInfo();
        
        QString type();
        void setType(const QString& type);
        
        QList<RoleSet> roleSets();
        void setRoleSets(const QList<RoleSet>& roleSets);
    private:
        DriveAboutPrivate_AdditionalRoleInfo* d;
    };
    
    class Feature
    {
    public:
        Feature();
        Feature(const Feature& other);
        virtual ~Feature();
        
        QString featureName();
        void setFeatureName(const QString& featureName);
        
        qreal featureRate();
        void setFeatureRate(const qreal& featureRate);
    private:
        DriveAboutPrivate_Feature* d;
    };
    
    class MaxUploadSize
    {
    public:
        MaxUploadSize();
        MaxUploadSize(const MaxUploadSize& other);
        virtual ~MaxUploadSize();
        
        QString type();
        void setType(const QString& type);
        
        qulonglong size();
        void setSize(const qulonglong& size);
    private:
        DriveAboutPrivate_MaxUploadSize* d;
    };
    
    typedef QSharedPointer< DriveAbout > Ptr;
    typedef QList< DriveAbout > List;
    
    DriveAbout();
    DriveAbout(const DriveAbout& other);
    virtual ~DriveAbout();
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    QString name();
    void setName(const QString& name);
    
    qulonglong quotaBytesTotal();
    void setQuotaBytesTotal(const qulonglong& quotaBytesTotal);
    
    qulonglong quotaBytesUsed();
    void setQuotaBytesUsed(const qulonglong& quotaBytesUsed);
    
    qulonglong quotaBytesUsedInTrash();
    void setQuotaBytesUsedInTrash(const qulonglong& quotaBytesUsedInTrash);
    
    qulonglong largestChangeId();
    void setLargestChangeId(const qulonglong& largestChangeId);
    
    qulonglong remainingChangeIds();
    void setRemainingChangeIds(const qulonglong& remainingChangeIds);
    
    QString rootFolderId();
    void setRootFolderId(const QString& rootFolderId);
    
    QString domainSharingPolicy();
    void setDomainSharingPolicy(const QString& domainSharingPolicy);
    
    QList<Format> importFormats();
    void setImportFormats(const QList<Format>& importFormats);
    
    QList<Format> exportFormats();
    void setExportFormats(const QList<Format>& exportFormats);
    
    QList<AdditionalRoleInfo> additionalRoleInfo();
    void setAdditionalRoleInfo(const QList<AdditionalRoleInfo>& additionalRoleInfo);
    
    QList<Feature> features();
    void setFeatures(const QList<Feature>& features);
    
    QList<MaxUploadSize> maxUploadSizes();
    void setMaxUploadSizes(const QList<MaxUploadSize>& maxUploadSizes);
    
    QString permissionId();
    void setPermissionId(const QString& permissionId);
    
    bool isCurrentAppInstalled();
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
