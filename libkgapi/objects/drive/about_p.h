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

#ifndef LIBKGAPI_OBJECTS_DRIVEABOUT_P_H
#define LIBKGAPI_OBJECTS_DRIVEABOUT_P_H

#include <QtCore/QString>

namespace KGAPI
{
    
namespace Objects
{

class DriveAboutPrivate_Format
{
public:
    QString source;
    QList<QString> targets;
};

class DriveAboutPrivate_AdditionalRoleInfo_RoleSet
{
public:
    QString primaryRole;
    QString additionalRoles;
};

class DriveAboutPrivate_AdditionalRoleInfo
{
public:
    typedef DriveAboutPrivate_AdditionalRoleInfo_RoleSet RoleSet;
    QString type;
    QList<DriveAbout::AdditionalRoleInfo::RoleSet> roleSets;
};

class DriveAboutPrivate_Feature
{
public:
    QString featureName;
    qreal featureRate;
};

class DriveAboutPrivate_MaxUploadSize
{
public:
    QString type;
    qulonglong size;
};
    
        
class DriveAboutPrivate
{
public:
    
    // need that because nested classes can't be forward-declared
    typedef DriveAboutPrivate_Format Format;
    typedef DriveAboutPrivate_AdditionalRoleInfo AdditionalRoleInfo;
    typedef DriveAboutPrivate_Feature Feature;
    typedef DriveAboutPrivate_MaxUploadSize MaxUploadSize;
    
    DriveAboutPrivate() { };
    DriveAboutPrivate(const DriveAboutPrivate& other);
    ~DriveAboutPrivate() { };

    QString selfLink;
    QString name;
    qulonglong quotaBytesTotal;
    qulonglong quotaBytesUsed;
    qulonglong quotaBytesUsedInTrash;
    qulonglong largestChangeId;
    qulonglong remainingChangeIds;
    QString rootFolderId;
    QString domainSharingPolicy;
    QList<DriveAbout::Format> importFormats;
    QList<DriveAbout::Format> exportFormats;
    QList<DriveAbout::AdditionalRoleInfo> additionalRoleInfo;
    QList<DriveAbout::Feature> features;
    QList<DriveAbout::MaxUploadSize> maxUploadSizes;
    QString permissionId;
    bool isCurrentAppInstalled;
};

} /* namespace Objects */
    
} /* namespace KGAPI */
    
#endif // LIBKGAPI_OBJECTS_DRIVEABOUT_P_H