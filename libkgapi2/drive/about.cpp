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
#include "user.h"

#include <qjson/parser.h>

using namespace KGAPI2;

///// DriveAbout::Format

class DriveAbout::Format::Private
{
  public:
    Private();
    Private(const Private &other);

    QString source;
    QStringList targets;
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

QStringList DriveAbout::Format::targets() const
{
    return d->targets;
}

///// DriveAbout::AdditionalRoleInfo::RoleSet

class DriveAbout::AdditionalRoleInfo::RoleSet::Private
{
  public:
    Private();
    Private(const Private &other);

    QString primaryRole;
    QStringList additionalRoles;
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

QStringList DriveAbout::AdditionalRoleInfo::RoleSet::additionalRoles() const
{
    return d->additionalRoles;
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

DriveAbout::AdditionalRoleInfo::RoleSetsList DriveAbout::AdditionalRoleInfo::roleSets() const
{
    return d->roleSets;
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

qreal DriveAbout::Feature::featureRate() const
{
    return d->featureRate;
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

qlonglong DriveAbout::MaxUploadSize::size() const
{
    return d->size;
}

///// DriveAbout

class DriveAbout::Private
{
  public:
    Private();
    Private(const Private &other);

    QUrl selfLink;
    QString name;
    qlonglong quotaBytesTotal;
    qlonglong quotaBytesUsed;
    qlonglong quotaBytesUsedInTrash;
    qlonglong quotaBytesUsedAggregate;
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
    DriveUserPtr user;
};

DriveAbout::Private::Private():
    quotaBytesTotal(-1),
    quotaBytesUsed(-1),
    quotaBytesUsedInTrash(-1),
    quotaBytesUsedAggregate(-1),
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
    quotaBytesUsedAggregate(other.quotaBytesUsedAggregate),
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
    isCurrentAppInstalled(other.isCurrentAppInstalled),
    user(other.user)
{
}

DriveAbout::DriveAbout():
    KGAPI2::Object(),
    d(new Private)
{
}

DriveAbout::DriveAbout(const DriveAbout &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

DriveAbout::~DriveAbout()
{
    delete d;
}

QUrl DriveAbout::selfLink() const
{
    return d->selfLink;
}

QString DriveAbout::name() const
{
    return d->name;
}

qlonglong DriveAbout::quotaBytesTotal() const
{
    return d->quotaBytesTotal;
}

qlonglong DriveAbout::quotaBytesUsed() const
{
    return d->quotaBytesUsed;
}

qlonglong DriveAbout::quotaBytesUsedInTrash() const
{
    return d->quotaBytesUsedInTrash;
}

qlonglong DriveAbout::quotaBytesUserAggregate() const
{
    return d->quotaBytesUsedAggregate;
}

qlonglong DriveAbout::largestChangeId() const
{
    return d->largestChangeId;
}
qlonglong DriveAbout::remainingChangeIds() const
{
    return d->remainingChangeIds;
}

QString DriveAbout::rootFolderId() const
{
    return d->rootFolderId;
}

QString DriveAbout::domainSharingPolicy() const
{
    return d->domainSharingPolicy;
}

DriveAbout::FormatsList DriveAbout::importFormats() const
{
    return d->importFormats;
}

DriveAbout::FormatsList DriveAbout::exportFormats() const
{
    return d->exportFormats;
}

DriveAbout::AdditionalRoleInfosList DriveAbout::additionalRoleInfo() const
{
    return d->additionalRoleInfo;
}

DriveAbout::FeaturesList DriveAbout::features() const
{
    return d->features;
}

DriveAbout::MaxUploadSizesList DriveAbout::maxUploadSizes() const
{
    return d->maxUploadSizes;
}

QString DriveAbout::permissionId() const
{
    return d->permissionId;
}

bool DriveAbout::isCurrentAppInstalled() const
{
    return d->isCurrentAppInstalled;
}

DriveUserPtr DriveAbout::user() const
{
    return d->user;
}

DriveAboutPtr DriveAbout::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant json = parser.parse(jsonData, &ok);
    if (!ok) {
        return DriveAboutPtr();
    }
    const QVariantMap map = json.toMap();

    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#about")) {

        return DriveAboutPtr();
    }

    DriveAboutPtr about(new DriveAbout());
    about->setEtag(map.value(QLatin1String("etag")).toString());
    about->d->selfLink = map.value(QLatin1String("selfLink")).toUrl();
    about->d->name = map.value(QLatin1String("name")).toString();
    about->d->quotaBytesTotal = map.value(QLatin1String("quotaBytesTotal")).toLongLong();
    about->d->quotaBytesUsed = map.value(QLatin1String("quotaBytesUsed")).toLongLong();
    about->d->quotaBytesUsedInTrash = map.value(QLatin1String("quotaBytesUsedInTrash")).toLongLong();
    about->d->quotaBytesUsedAggregate = map.value(QLatin1String("quotaBytesUsedAggregate")).toLongLong();
    about->d->largestChangeId = map.value(QLatin1String("largestChangeId")).toLongLong();
    about->d->remainingChangeIds = map.value(QLatin1String("remainingChangeIds")).toLongLong();
    about->d->rootFolderId = map.value(QLatin1String("rootFolderId")).toString();
    about->d->domainSharingPolicy = map.value(QLatin1String("domainSharingPolicy")).toString();
    about->d->permissionId = map.value(QLatin1String("permissionId")).toString();
    about->d->isCurrentAppInstalled = map.value(QLatin1String("isCurrentAppInstalled")).toBool();

    const QVariantList importFormats = map.value(QLatin1String("importFormats")).toList();
    Q_FOREACH (const QVariant &v, importFormats) {
        const QVariantMap importFormat = v.toMap();
        FormatPtr format(new Format());
        format->d->source = importFormat.value(QLatin1String("source")).toString();
        format->d->targets = importFormat.value(QLatin1String("targets")).toStringList();

        about->d->importFormats << format;
    }

    const QVariantList exportFormats = map.value(QLatin1String("exportFormats")).toList();
    Q_FOREACH (const QVariant &v, exportFormats) {
        const QVariantMap exportFormat = v.toMap();
        FormatPtr format(new Format());
        format->d->source = exportFormat.value(QLatin1String("source")).toString();
        format->d->targets = exportFormat.value(QLatin1String("targets")).toStringList();

        about->d->exportFormats << format;
    }

    const QVariantList additionalRoleInfos = map.value(QLatin1String("additionalRoleInfo")).toList();
    Q_FOREACH (const QVariant &v, additionalRoleInfos) {
        const QVariantMap additionalRoleInfo = v.toMap();
        AdditionalRoleInfoPtr info(new AdditionalRoleInfo());
        info->d->type = additionalRoleInfo.value(QLatin1String("type")).toString();

        const QVariantList roleSets = additionalRoleInfo.value(QLatin1String("roleSets")).toList();
        Q_FOREACH (const QVariant &vv, roleSets) {
            const QVariantMap roleSetData = vv.toMap();
            AdditionalRoleInfo::RoleSetPtr roleSet(new AdditionalRoleInfo::RoleSet());
            roleSet->d->primaryRole = roleSetData.value(QLatin1String("primaryRole")).toString();
            roleSet->d->additionalRoles = roleSetData.value(QLatin1String("additionalRoles")).toStringList();

            info->d->roleSets << roleSet;
        }

        about->d->additionalRoleInfo << info;
    }

    const QVariantList features = map.value(QLatin1String("features")).toList();
    Q_FOREACH (const QVariant &v, features) {
        const QVariantMap featureData = v.toMap();
        FeaturePtr feature(new Feature());
        feature->d->featureName = featureData.value(QLatin1String("featureName")).toString();
        feature->d->featureRate = featureData.value(QLatin1String("featureRate")).toReal();

        about->d->features << feature;
    }

    const QVariantList maxUploadSizes = map.value(QLatin1String("maxUploadSizes")).toList();
    Q_FOREACH (const QVariant &v, maxUploadSizes) {
        const QVariantMap maxUploadSizeData = v.toMap();
        MaxUploadSizePtr maxUploadSize(new MaxUploadSize());
        maxUploadSize->d->type = maxUploadSizeData.value(QLatin1String("type")).toString();
        maxUploadSize->d->size = maxUploadSizeData.value(QLatin1String("size")).toLongLong();

        about->d->maxUploadSizes << maxUploadSize;
    }

    about->d->user = DriveUser::fromJSON(map.value(QLatin1String("user")).toMap());

    return about;
}
