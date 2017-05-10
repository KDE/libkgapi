/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

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

#include "about.h"
#include "user.h"

#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

///// DriveAbout::Format

class Q_DECL_HIDDEN About::Format::Private
{
  public:
    Private();
    Private(const Private &other);

    QString source;
    QStringList targets;
};

About::Format::Private::Private()
{
}

About::Format::Private::Private(const Private &other):
    source(other.source),
    targets(other.targets)
{
}

About::Format::Format():
    d(new Private)
{
}

About::Format::Format(const About::Format &other):
    d(new Private(*(other.d)))
{
}

About::Format::~Format()
{
    delete d;
}

QString About::Format::source() const
{
    return d->source;
}

QStringList About::Format::targets() const
{
    return d->targets;
}

///// DriveAbout::AdditionalRoleInfo::RoleSet

class Q_DECL_HIDDEN About::AdditionalRoleInfo::RoleSet::Private
{
  public:
    Private();
    Private(const Private &other);

    QString primaryRole;
    QStringList additionalRoles;
};

About::AdditionalRoleInfo::RoleSet::Private::Private()
{
}

About::AdditionalRoleInfo::RoleSet::Private::Private(const Private &other):
    primaryRole(other.primaryRole),
    additionalRoles(other.additionalRoles)
{
}

About::AdditionalRoleInfo::RoleSet::RoleSet():
    d(new Private)
{
}

About::AdditionalRoleInfo::RoleSet::RoleSet(const About::AdditionalRoleInfo::RoleSet &other):
    d(new Private(*(other.d)))
{
}

About::AdditionalRoleInfo::RoleSet::~RoleSet()
{
    delete d;
}

QString About::AdditionalRoleInfo::RoleSet::primaryRole() const
{
    return d->primaryRole;
}

QStringList About::AdditionalRoleInfo::RoleSet::additionalRoles() const
{
    return d->additionalRoles;
}

///// DriveAbout::AdditionalRoleInfo

class Q_DECL_HIDDEN About::AdditionalRoleInfo::Private
{
  public:
    Private();
    Private(const Private &other);

    QString type;
    RoleSetsList roleSets;
};

About::AdditionalRoleInfo::Private::Private()
{
}

About::AdditionalRoleInfo::Private::Private(const Private &other):
    type(other.type),
    roleSets(other.roleSets)
{
}

About::AdditionalRoleInfo::AdditionalRoleInfo():
    d(new Private)
{
}

About::AdditionalRoleInfo::AdditionalRoleInfo(const About::AdditionalRoleInfo &other):
    d(new Private(*(other.d)))
{
}

About::AdditionalRoleInfo::~AdditionalRoleInfo()
{
    delete d;
}

QString About::AdditionalRoleInfo::type() const
{
    return d->type;
}

About::AdditionalRoleInfo::RoleSetsList About::AdditionalRoleInfo::roleSets() const
{
    return d->roleSets;
}

///// DriveAbout::Feature

class Q_DECL_HIDDEN About::Feature::Private
{
  public:
    Private();
    Private(const Private &other);

    QString featureName;
    qreal featureRate;
};

About::Feature::Private::Private():
    featureRate(-1)
{
}

About::Feature::Private::Private(const Private &other):
    featureName(other.featureName),
    featureRate(other.featureRate)
{
}

About::Feature::Feature():
    d(new Private)
{
}

About::Feature::Feature(const About::Feature &other):
    d(new Private(*(other.d)))
{
}

About::Feature::~Feature()
{
    delete d;
}

QString About::Feature::featureName() const
{
    return d->featureName;
}

qreal About::Feature::featureRate() const
{
    return d->featureRate;
}

///// DriveAbout::MaxUploadSize

class Q_DECL_HIDDEN About::MaxUploadSize::Private
{
  public:
    Private();
    Private(const Private &other);

    QString type;
    qlonglong size;
};

About::MaxUploadSize::Private::Private():
    size(-1)
{
}

About::MaxUploadSize::Private::Private(const Private &other):
    type(other.type),
    size(other.size)
{
}

About::MaxUploadSize::MaxUploadSize():
    d(new Private)
{
}

About::MaxUploadSize::MaxUploadSize(const About::MaxUploadSize &other):
    d(new Private(*(other.d)))
{ }

About::MaxUploadSize::~MaxUploadSize()
{
    delete d;
}

QString About::MaxUploadSize::type() const
{
    return d->type;
}

qlonglong About::MaxUploadSize::size() const
{
    return d->size;
}

///// DriveAbout

class Q_DECL_HIDDEN About::Private
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
    UserPtr user;
};

About::Private::Private():
    quotaBytesTotal(-1),
    quotaBytesUsed(-1),
    quotaBytesUsedInTrash(-1),
    quotaBytesUsedAggregate(-1),
    largestChangeId(-1),
    remainingChangeIds(-1),
    isCurrentAppInstalled(false)
{
}

About::Private::Private(const About::Private &other):
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

About::About():
    KGAPI2::Object(),
    d(new Private)
{
}

About::About(const About &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

About::~About()
{
    delete d;
}

QUrl About::selfLink() const
{
    return d->selfLink;
}

QString About::name() const
{
    return d->name;
}

qlonglong About::quotaBytesTotal() const
{
    return d->quotaBytesTotal;
}

qlonglong About::quotaBytesUsed() const
{
    return d->quotaBytesUsed;
}

qlonglong About::quotaBytesUsedInTrash() const
{
    return d->quotaBytesUsedInTrash;
}

qlonglong About::quotaBytesUsedAggregate() const
{
    return d->quotaBytesUsedAggregate;
}

#ifndef KGAPIDRIVE_NO_DEPRECATED
qlonglong About::quotaBytesUserAggregate() const
{
    return quotaBytesUsedAggregate();
}
#endif

qlonglong About::largestChangeId() const
{
    return d->largestChangeId;
}
qlonglong About::remainingChangeIds() const
{
    return d->remainingChangeIds;
}

QString About::rootFolderId() const
{
    return d->rootFolderId;
}

QString About::domainSharingPolicy() const
{
    return d->domainSharingPolicy;
}

About::FormatsList About::importFormats() const
{
    return d->importFormats;
}

About::FormatsList About::exportFormats() const
{
    return d->exportFormats;
}

About::AdditionalRoleInfosList About::additionalRoleInfo() const
{
    return d->additionalRoleInfo;
}

About::FeaturesList About::features() const
{
    return d->features;
}

About::MaxUploadSizesList About::maxUploadSizes() const
{
    return d->maxUploadSizes;
}

QString About::permissionId() const
{
    return d->permissionId;
}

bool About::isCurrentAppInstalled() const
{
    return d->isCurrentAppInstalled;
}

UserPtr About::user() const
{
    return d->user;
}

AboutPtr About::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return AboutPtr();
    }
    const QVariantMap map = document.toVariant().toMap();

    if (!map.contains(QStringLiteral("kind")) ||
        map[QStringLiteral("kind")].toString() != QLatin1String("drive#about")) {

        return AboutPtr();
    }

    AboutPtr about(new About());
    about->setEtag(map.value(QStringLiteral("etag")).toString());
    about->d->selfLink = map.value(QStringLiteral("selfLink")).toUrl();
    about->d->name = map.value(QStringLiteral("name")).toString();
    about->d->quotaBytesTotal = map.value(QStringLiteral("quotaBytesTotal")).toLongLong();
    about->d->quotaBytesUsed = map.value(QStringLiteral("quotaBytesUsed")).toLongLong();
    about->d->quotaBytesUsedInTrash = map.value(QStringLiteral("quotaBytesUsedInTrash")).toLongLong();
    about->d->quotaBytesUsedAggregate = map.value(QStringLiteral("quotaBytesUsedAggregate")).toLongLong();
    about->d->largestChangeId = map.value(QStringLiteral("largestChangeId")).toLongLong();
    about->d->remainingChangeIds = map.value(QStringLiteral("remainingChangeIds")).toLongLong();
    about->d->rootFolderId = map.value(QStringLiteral("rootFolderId")).toString();
    about->d->domainSharingPolicy = map.value(QStringLiteral("domainSharingPolicy")).toString();
    about->d->permissionId = map.value(QStringLiteral("permissionId")).toString();
    about->d->isCurrentAppInstalled = map.value(QStringLiteral("isCurrentAppInstalled")).toBool();

    const QVariantList importFormats = map.value(QStringLiteral("importFormats")).toList();
    for  (const QVariant &v : importFormats) {
        const QVariantMap importFormat = v.toMap();
        FormatPtr format(new Format());
        format->d->source = importFormat.value(QStringLiteral("source")).toString();
        format->d->targets = importFormat.value(QStringLiteral("targets")).toStringList();

        about->d->importFormats << format;
    }

    const QVariantList exportFormats = map.value(QStringLiteral("exportFormats")).toList();
    for (const QVariant &v : exportFormats) {
        const QVariantMap exportFormat = v.toMap();
        FormatPtr format(new Format());
        format->d->source = exportFormat.value(QStringLiteral("source")).toString();
        format->d->targets = exportFormat.value(QStringLiteral("targets")).toStringList();

        about->d->exportFormats << format;
    }

    const QVariantList additionalRoleInfos = map.value(QStringLiteral("additionalRoleInfo")).toList();
    for (const QVariant &v : additionalRoleInfos) {
        const QVariantMap additionalRoleInfo = v.toMap();
        AdditionalRoleInfoPtr info(new AdditionalRoleInfo());
        info->d->type = additionalRoleInfo.value(QStringLiteral("type")).toString();

        const QVariantList roleSets = additionalRoleInfo.value(QStringLiteral("roleSets")).toList();
        for (const QVariant &vv : roleSets) {
            const QVariantMap roleSetData = vv.toMap();
            AdditionalRoleInfo::RoleSetPtr roleSet(new AdditionalRoleInfo::RoleSet());
            roleSet->d->primaryRole = roleSetData.value(QStringLiteral("primaryRole")).toString();
            roleSet->d->additionalRoles = roleSetData.value(QStringLiteral("additionalRoles")).toStringList();

            info->d->roleSets << roleSet;
        }

        about->d->additionalRoleInfo << info;
    }

    const QVariantList features = map.value(QStringLiteral("features")).toList();
    for (const QVariant &v : features) {
        const QVariantMap featureData = v.toMap();
        FeaturePtr feature(new Feature());
        feature->d->featureName = featureData.value(QStringLiteral("featureName")).toString();
        feature->d->featureRate = featureData.value(QStringLiteral("featureRate")).toReal();

        about->d->features << feature;
    }

    const QVariantList maxUploadSizes = map.value(QStringLiteral("maxUploadSizes")).toList();
    for (const QVariant &v : maxUploadSizes) {
        const QVariantMap maxUploadSizeData = v.toMap();
        MaxUploadSizePtr maxUploadSize(new MaxUploadSize());
        maxUploadSize->d->type = maxUploadSizeData.value(QStringLiteral("type")).toString();
        maxUploadSize->d->size = maxUploadSizeData.value(QStringLiteral("size")).toLongLong();

        about->d->maxUploadSizes << maxUploadSize;
    }

    about->d->user = User::fromJSON(map.value(QStringLiteral("user")).toMap());

    return about;
}
