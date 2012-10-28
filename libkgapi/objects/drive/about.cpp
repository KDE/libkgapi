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

#include "about.h"
#include "about_p.h"

using namespace KGAPI::Objects;

///// DriveAboutPrivate

DriveAboutPrivate::DriveAboutPrivate(const DriveAboutPrivate& other):
    selfLink(other.selfLink),
    name(other.name),
    quotaBytesTotal(other.quotaBytesTotal),
    quotaBytesUsed(other.quotaBytesUsed),
    quotaBytesUsedInTrash(other.quotaBytesUsedInTrash),
    largestChangeId(other.largestChangeId),
    remainingChangeIds(other.remainingChangeIds),
    rootFolderId(other.rootFolderId),
    domainSharingPolicy(other.domainSharingPolicy),
    importFormats(other.importFormats),
    exportFormats(other.exportFormats),
    additionalRoleInfo(other.additionalRoleInfo),
    features(other.features),
    maxUploadSizes(other.maxUploadSizes),
    permissionId(other.permissionId),
    isCurrentAppInstalled(other.isCurrentAppInstalled)
{ }

///// DriveAbout::Format

DriveAbout::Format::Format()
{ }

DriveAbout::Format::Format(const DriveAbout::Format& other):
    d(new DriveAboutPrivate::Format(*(other.d)))
{ }

DriveAbout::Format::~Format()
{
    delete d;
}

QString DriveAbout::Format::source()
{
    return d->source;
}

void DriveAbout::Format::setSource(const QString& source)
{
    d->source = source;
}

QStringList DriveAbout::Format::targets()
{
    return d->targets;
}

void DriveAbout::Format::setTargets(const QStringList& targets)
{
    d->targets = targets;
}

///// DriveAbout::AdditionalRoleInfo::RoleSet

DriveAbout::AdditionalRoleInfo::RoleSet::RoleSet()
{ }

DriveAbout::AdditionalRoleInfo::RoleSet::RoleSet(const DriveAbout::AdditionalRoleInfo::RoleSet& other):
    d(new DriveAboutPrivate::AdditionalRoleInfo::RoleSet(*(other.d)))
{ }

DriveAbout::AdditionalRoleInfo::RoleSet::~RoleSet()
{
    delete d;
}

QString DriveAbout::AdditionalRoleInfo::RoleSet::primaryRole()
{
    return d->primaryRole;
}

void DriveAbout::AdditionalRoleInfo::RoleSet::setPrimaryRole(const QString& primaryRole)
{
    d->primaryRole = primaryRole;
}

QString DriveAbout::AdditionalRoleInfo::RoleSet::additionalRoles()
{
    return d->additionalRoles;
}

void DriveAbout::AdditionalRoleInfo::RoleSet::setAdditionalRoles(const QString& additionalRoles)
{
    d->additionalRoles = additionalRoles;
}

///// DriveAbout::AdditionalRoleInfo

DriveAbout::AdditionalRoleInfo::AdditionalRoleInfo()
{ }

DriveAbout::AdditionalRoleInfo::AdditionalRoleInfo(const DriveAbout::AdditionalRoleInfo& other):
    d(new DriveAboutPrivate::AdditionalRoleInfo(*(other.d)))
{ }

DriveAbout::AdditionalRoleInfo::~AdditionalRoleInfo()
{
    delete d;
}

QString DriveAbout::AdditionalRoleInfo::type()
{
    return d->type;
}

void DriveAbout::AdditionalRoleInfo::setType(const QString& type)
{
    d->type = type;
}

QList<DriveAbout::AdditionalRoleInfo::RoleSet> DriveAbout::AdditionalRoleInfo::roleSets()
{
    return d->roleSets;
}

void DriveAbout::AdditionalRoleInfo::setRoleSets(const QList<DriveAbout::AdditionalRoleInfo::RoleSet>& roleSets)
{
    d->roleSets = roleSets;
}

///// DriveAbout::Feature

DriveAbout::Feature::Feature()
{ }

DriveAbout::Feature::Feature(const DriveAbout::Feature& other):
    d(new DriveAboutPrivate::Feature(*(other.d)))
{ }

DriveAbout::Feature::~Feature()
{
    delete d;
}

QString DriveAbout::Feature::featureName()
{
    return d->featureName;
}

void DriveAbout::Feature::setFeatureName(const QString& featureName)
{
    d->featureName = featureName;
}

qreal DriveAbout::Feature::featureRate()
{
    return d->featureRate;
}

void DriveAbout::Feature::setFeatureRate(const qreal& featureRate)
{
    d->featureRate = featureRate;
}

///// DriveAbout::MaxUploadSize

DriveAbout::MaxUploadSize::MaxUploadSize()
{ }

DriveAbout::MaxUploadSize::MaxUploadSize(const DriveAbout::MaxUploadSize& other):
    d(new DriveAboutPrivate::MaxUploadSize(*(other.d)))
{ }

DriveAbout::MaxUploadSize::~MaxUploadSize()
{
    delete d;
}

QString DriveAbout::MaxUploadSize::type()
{
    return d->type;
}

void DriveAbout::MaxUploadSize::setType(const QString& type)
{
    d->type = type;
}

qlonglong DriveAbout::MaxUploadSize::size()
{
    return d->size;
}

void DriveAbout::MaxUploadSize::setSize(const qlonglong& size)
{
    d->size = size;
}

///// DriveAbout

DriveAbout::DriveAbout()
{ }

DriveAbout::DriveAbout(const DriveAbout& other):
    d(new DriveAboutPrivate)
{ }

DriveAbout::~DriveAbout()
{
    delete d;
}

QString DriveAbout::selfLink()
{
    return d->selfLink;
}

void DriveAbout::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QString DriveAbout::name()
{
    return d->name;
}

void DriveAbout::setName(const QString& name)
{
    d->name = name;
}

qlonglong DriveAbout::quotaBytesTotal()
{
    return d->quotaBytesTotal;
}

void DriveAbout::setQuotaBytesTotal(const qlonglong& quotaBytesTotal)
{
    d->quotaBytesTotal = quotaBytesTotal;
}

qlonglong DriveAbout::quotaBytesUsed()
{
    return d->quotaBytesUsed;
}

void DriveAbout::setQuotaBytesUsed(const qlonglong& quotaBytesUsed)
{
    d->quotaBytesUsed = quotaBytesUsed;
}

qlonglong DriveAbout::quotaBytesUsedInTrash()
{
    return d->quotaBytesUsedInTrash;
}

void DriveAbout::setQuotaBytesUsedInTrash(const qlonglong& quotaBytesUsedInTrash)
{
    d->quotaBytesUsedInTrash = quotaBytesUsedInTrash;
}

qlonglong DriveAbout::largestChangeId()
{
    return d->largestChangeId;
}

void DriveAbout::setLargestChangeId(const qlonglong& largestChangeId)
{
    d->largestChangeId = largestChangeId;
}

qlonglong DriveAbout::remainingChangeIds()
{
    return d->remainingChangeIds;
}

void DriveAbout::setRemainingChangeIds(const qlonglong& remainingChangeIds)
{
    d->remainingChangeIds = remainingChangeIds;
}

QString DriveAbout::rootFolderId()
{
    return d->rootFolderId;
}

void DriveAbout::setRootFolderId(const QString& rootFolderId)
{
    d->rootFolderId = rootFolderId;
}

QString DriveAbout::domainSharingPolicy()
{
    return d->domainSharingPolicy;
}

void DriveAbout::setDomainSharingPolicy(const QString& domainSharingPolicy)
{
    d->domainSharingPolicy = domainSharingPolicy;
}

QList<DriveAbout::Format> DriveAbout::importFormats()
{
    return d->importFormats;
}

void DriveAbout::setImportFormats(const QList<DriveAbout::Format>& importFormats)
{
    d->importFormats = importFormats;
}

QList<DriveAbout::Format> DriveAbout::exportFormats()
{
    return d->exportFormats;
}

void DriveAbout::setExportFormats(const QList<DriveAbout::Format>& exportFormats)
{
    d->exportFormats = exportFormats;
}

QList<DriveAbout::AdditionalRoleInfo> DriveAbout::additionalRoleInfo()
{
    return d->additionalRoleInfo;
}

void DriveAbout::setAdditionalRoleInfo(const QList<DriveAbout::AdditionalRoleInfo>& additionalRoleInfo)
{
    d->additionalRoleInfo = additionalRoleInfo;
}

QList<DriveAbout::Feature> DriveAbout::features()
{
    return d->features;
}

void DriveAbout::setFeatures(const QList<DriveAbout::Feature>& features)
{
    d->features = features;
}

QList<DriveAbout::MaxUploadSize> DriveAbout::maxUploadSizes()
{
    return d->maxUploadSizes;
}

void DriveAbout::setMaxUploadSizes(const QList<DriveAbout::MaxUploadSize>& maxUploadSizes)
{
    d->maxUploadSizes = maxUploadSizes;
}

QString DriveAbout::permissionId()
{
    return d->permissionId;
}

void DriveAbout::setPermissionId(const QString& permissionId)
{
    d->permissionId = permissionId;
}

QVariant DriveAbout::isCurrentAppInstalled()
{
    return d->isCurrentAppInstalled;
}

void DriveAbout::setIsCurrentAppInstalled(const QVariant& isCurrentAppInstalled)
{
    d->isCurrentAppInstalled = isCurrentAppInstalled;
}
