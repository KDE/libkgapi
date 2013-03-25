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

using namespace KGAPI2;

///// DriveAbout::Format

class DriveAbout::Format::Private
{
  public:
    Private();
    Private(const Private &other);

    QString source;
    QList<QString> targets;
};

DriveAbout::Format::Private::Private()
{
}

DriveAbout::Format::Private::Private(const Private &other):
    source(other.source),
    targets(other.targets)
{
}

DriveAbout::Format::Format():
    d(new Private)
{
}

DriveAbout::Format::Format(const DriveAbout::Format &other):
    d(new Private(*(other.d)))
{
}

DriveAbout::Format::~Format()
{
    delete d;
}

QString DriveAbout::Format::source() const
{
    return d->source;
}

void DriveAbout::Format::setSource(const QString &source)
{
    d->source = source;
}

QStringList DriveAbout::Format::targets() const
{
    return d->targets;
}

void DriveAbout::Format::setTargets(const QStringList &targets)
{
    d->targets = targets;
}

///// DriveAbout::AdditionalRoleInfo::RoleSet

class DriveAbout::AdditionalRoleInfo::RoleSet::Private
{
  public:
    Private();
    Private(const Private &other);

    QString primaryRole;
    QString additionalRoles;
};

DriveAbout::AdditionalRoleInfo::RoleSet::Private::Private()
{
}

DriveAbout::AdditionalRoleInfo::RoleSet::Private::Private(const Private &other):
    primaryRole(other.primaryRole),
    additionalRoles(other.additionalRoles)
{
}

DriveAbout::AdditionalRoleInfo::RoleSet::RoleSet():
    d(new Private)
{
}

DriveAbout::AdditionalRoleInfo::RoleSet::RoleSet(const DriveAbout::AdditionalRoleInfo::RoleSet &other):
    d(new Private(*(other.d)))
{
}

DriveAbout::AdditionalRoleInfo::RoleSet::~RoleSet()
{
    delete d;
}

QString DriveAbout::AdditionalRoleInfo::RoleSet::primaryRole() const
{
    return d->primaryRole;
}

void DriveAbout::AdditionalRoleInfo::RoleSet::setPrimaryRole(const QString &primaryRole)
{
    d->primaryRole = primaryRole;
}

QString DriveAbout::AdditionalRoleInfo::RoleSet::additionalRoles() const
{
    return d->additionalRoles;
}

void DriveAbout::AdditionalRoleInfo::RoleSet::setAdditionalRoles(const QString &additionalRoles)
{
    d->additionalRoles = additionalRoles;
}

///// DriveAbout::AdditionalRoleInfo

class DriveAbout::AdditionalRoleInfo::Private
{
  public:
    Private();
    Private(const Private &other);

    QString type;
    RoleSetsList roleSets;
};

DriveAbout::AdditionalRoleInfo::Private::Private()
{
}

DriveAbout::AdditionalRoleInfo::Private::Private(const Private &other):
    type(other.type),
    roleSets(other.roleSets)
{
}

DriveAbout::AdditionalRoleInfo::AdditionalRoleInfo():
    d(new Private)
{
}

DriveAbout::AdditionalRoleInfo::AdditionalRoleInfo(const DriveAbout::AdditionalRoleInfo &other):
    d(new Private(*(other.d)))
{
}

DriveAbout::AdditionalRoleInfo::~AdditionalRoleInfo()
{
    delete d;
}

QString DriveAbout::AdditionalRoleInfo::type() const
{
    return d->type;
}

void DriveAbout::AdditionalRoleInfo::setType(const QString &type)
{
    d->type = type;
}

DriveAbout::AdditionalRoleInfo::RoleSetsList DriveAbout::AdditionalRoleInfo::roleSets() const
{
    return d->roleSets;
}

void DriveAbout::AdditionalRoleInfo::setRoleSets(const RoleSetsList &roleSets)
{
    d->roleSets = roleSets;
}

///// DriveAbout::Feature

class DriveAbout::Feature::Private
{
  public:
    Private();
    Private(const Private &other);

    QString featureName;
    qreal featureRate;
};

DriveAbout::Feature::Private::Private():
    featureRate(-1)
{
}

DriveAbout::Feature::Private::Private(const Private &other):
    featureName(other.featureName),
    featureRate(other.featureRate)
{
}

DriveAbout::Feature::Feature():
    d(new Private)
{
}

DriveAbout::Feature::Feature(const DriveAbout::Feature &other):
    d(new Private(*(other.d)))
{
}

DriveAbout::Feature::~Feature()
{
    delete d;
}

QString DriveAbout::Feature::featureName() const
{
    return d->featureName;
}

void DriveAbout::Feature::setFeatureName(const QString &featureName)
{
    d->featureName = featureName;
}

qreal DriveAbout::Feature::featureRate() const
{
    return d->featureRate;
}

void DriveAbout::Feature::setFeatureRate(qreal featureRate)
{
    d->featureRate = featureRate;
}

///// DriveAbout::MaxUploadSize

class DriveAbout::MaxUploadSize::Private
{
  public:
    Private();
    Private(const Private &other);

    QString type;
    qlonglong size;
};

DriveAbout::MaxUploadSize::Private::Private():
    size(-1)
{
}

DriveAbout::MaxUploadSize::Private::Private(const Private &other):
    type(other.type),
    size(other.size)
{
}

DriveAbout::MaxUploadSize::MaxUploadSize():
    d(new Private)
{
}

DriveAbout::MaxUploadSize::MaxUploadSize(const DriveAbout::MaxUploadSize &other):
    d(new Private(*(other.d)))
{ }

DriveAbout::MaxUploadSize::~MaxUploadSize()
{
    delete d;
}

QString DriveAbout::MaxUploadSize::type() const
{
    return d->type;
}

void DriveAbout::MaxUploadSize::setType(const QString &type)
{
    d->type = type;
}

qlonglong DriveAbout::MaxUploadSize::size() const
{
    return d->size;
}

void DriveAbout::MaxUploadSize::setSize(qlonglong size)
{
    d->size = size;
}

///// DriveAbout

class DriveAbout::Private
{
  public:
    Private();
    Private(const Private &other);

    QString selfLink;
    QString name;
    qlonglong quotaBytesTotal;
    qlonglong quotaBytesUsed;
    qlonglong quotaBytesUsedInTrash;
    qlonglong largestChangeId;
    qlonglong remainingChangeIds;
    QString rootFolderId;
    QString domainSharingPolicy;
    FormatsList importFormats;
    FormatsList exportFormats;
    AdditionalRoleInfosList additionalRoleInfo;
    FeaturesList features;
    MaxUploadSizesList maxUploadSizes;
    QString permissionId;
    bool isCurrentAppInstalled;
};

DriveAbout::Private::Private():
    quotaBytesTotal(-1),
    quotaBytesUsed(-1),
    quotaBytesUsedInTrash(-1),
    largestChangeId(-1),
    remainingChangeIds(-1),
    isCurrentAppInstalled(false)
{
}

DriveAbout::Private::Private(const DriveAbout::Private &other):
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
{
}

DriveAbout::DriveAbout():
    d(new Private)
{
}

DriveAbout::DriveAbout(const DriveAbout &other):
    d(new Private(*(other.d)))
{
}

DriveAbout::~DriveAbout()
{
    delete d;
}

QString DriveAbout::selfLink() const
{
    return d->selfLink;
}

void DriveAbout::setSelfLink(const QString &selfLink)
{
    d->selfLink = selfLink;
}

QString DriveAbout::name() const
{
    return d->name;
}

void DriveAbout::setName(const QString &name)
{
    d->name = name;
}

qlonglong DriveAbout::quotaBytesTotal() const
{
    return d->quotaBytesTotal;
}

void DriveAbout::setQuotaBytesTotal(qlonglong quotaBytesTotal)
{
    d->quotaBytesTotal = quotaBytesTotal;
}

qlonglong DriveAbout::quotaBytesUsed() const
{
    return d->quotaBytesUsed;
}

void DriveAbout::setQuotaBytesUsed(qlonglong quotaBytesUsed)
{
    d->quotaBytesUsed = quotaBytesUsed;
}

qlonglong DriveAbout::quotaBytesUsedInTrash() const
{
    return d->quotaBytesUsedInTrash;
}

void DriveAbout::setQuotaBytesUsedInTrash(qlonglong quotaBytesUsedInTrash)
{
    d->quotaBytesUsedInTrash = quotaBytesUsedInTrash;
}

qlonglong DriveAbout::largestChangeId() const
{
    return d->largestChangeId;
}

void DriveAbout::setLargestChangeId(qlonglong largestChangeId)
{
    d->largestChangeId = largestChangeId;
}

qlonglong DriveAbout::remainingChangeIds() const
{
    return d->remainingChangeIds;
}

void DriveAbout::setRemainingChangeIds(qlonglong remainingChangeIds)
{
    d->remainingChangeIds = remainingChangeIds;
}

QString DriveAbout::rootFolderId() const
{
    return d->rootFolderId;
}

void DriveAbout::setRootFolderId(const QString &rootFolderId)
{
    d->rootFolderId = rootFolderId;
}

QString DriveAbout::domainSharingPolicy() const
{
    return d->domainSharingPolicy;
}

void DriveAbout::setDomainSharingPolicy(const QString &domainSharingPolicy)
{
    d->domainSharingPolicy = domainSharingPolicy;
}

DriveAbout::FormatsList DriveAbout::importFormats() const
{
    return d->importFormats;
}

void DriveAbout::setImportFormats(const FormatsList &importFormats)
{
    d->importFormats = importFormats;
}

DriveAbout::FormatsList DriveAbout::exportFormats() const
{
    return d->exportFormats;
}

void DriveAbout::setExportFormats(const FormatsList &exportFormats)
{
    d->exportFormats = exportFormats;
}

DriveAbout::AdditionalRoleInfosList DriveAbout::additionalRoleInfo() const
{
    return d->additionalRoleInfo;
}

void DriveAbout::setAdditionalRoleInfo(const AdditionalRoleInfosList &additionalRoleInfo)
{
    d->additionalRoleInfo = additionalRoleInfo;
}

DriveAbout::FeaturesList DriveAbout::features() const
{
    return d->features;
}

void DriveAbout::setFeatures(const FeaturesList &features)
{
    d->features = features;
}

DriveAbout::MaxUploadSizesList DriveAbout::maxUploadSizes() const
{
    return d->maxUploadSizes;
}

void DriveAbout::setMaxUploadSizes(const MaxUploadSizesList &maxUploadSizes)
{
    d->maxUploadSizes = maxUploadSizes;
}

QString DriveAbout::permissionId() const
{
    return d->permissionId;
}

void DriveAbout::setPermissionId(const QString &permissionId)
{
    d->permissionId = permissionId;
}

bool DriveAbout::isCurrentAppInstalled() const
{
    return d->isCurrentAppInstalled;
}

void DriveAbout::setIsCurrentAppInstalled(bool isCurrentAppInstalled)
{
    d->isCurrentAppInstalled = isCurrentAppInstalled;
}
