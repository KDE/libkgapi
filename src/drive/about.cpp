/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "about.h"
#include "debug.h"
#include "user.h"
#include "utils.h"
#include "utils_p.h"

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

About::Format::Private::Private(const Private &other)
    : source(other.source)
    , targets(other.targets)
{
}

About::Format::Format()
    : d(new Private)
{
}

About::Format::Format(const About::Format &other)
    : d(new Private(*(other.d)))
{
}

About::Format::~Format()
{
    delete d;
}

bool About::Format::operator==(const About::Format &other) const
{
    GAPI_COMPARE(source)
    GAPI_COMPARE(targets)
    return true;
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

About::AdditionalRoleInfo::RoleSet::Private::Private(const Private &other)
    : primaryRole(other.primaryRole)
    , additionalRoles(other.additionalRoles)
{
}

About::AdditionalRoleInfo::RoleSet::RoleSet()
    : d(new Private)
{
}

About::AdditionalRoleInfo::RoleSet::RoleSet(const About::AdditionalRoleInfo::RoleSet &other)
    : d(new Private(*(other.d)))
{
}

About::AdditionalRoleInfo::RoleSet::~RoleSet()
{
    delete d;
}

bool About::AdditionalRoleInfo::RoleSet::operator==(const RoleSet &other) const
{
    GAPI_COMPARE(primaryRole)
    GAPI_COMPARE(additionalRoles)
    return true;
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

About::AdditionalRoleInfo::Private::Private(const Private &other)
    : type(other.type)
    , roleSets(other.roleSets)
{
}

About::AdditionalRoleInfo::AdditionalRoleInfo()
    : d(new Private)
{
}

About::AdditionalRoleInfo::AdditionalRoleInfo(const About::AdditionalRoleInfo &other)
    : d(new Private(*(other.d)))
{
}

About::AdditionalRoleInfo::~AdditionalRoleInfo()
{
    delete d;
}

bool About::AdditionalRoleInfo::operator==(const AdditionalRoleInfo &other) const
{
    GAPI_COMPARE(type)
    GAPI_COMPARE_CONTAINERS(roleSets)
    return true;
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

About::Feature::Private::Private()
    : featureRate(-1)
{
}

About::Feature::Private::Private(const Private &other)
    : featureName(other.featureName)
    , featureRate(other.featureRate)
{
}

About::Feature::Feature()
    : d(new Private)
{
}

About::Feature::Feature(const About::Feature &other)
    : d(new Private(*(other.d)))
{
}

About::Feature::~Feature()
{
    delete d;
}

bool About::Feature::operator==(const Feature &other) const
{
    GAPI_COMPARE(featureName)
    GAPI_COMPARE(featureRate)
    return true;
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

About::MaxUploadSize::Private::Private()
    : size(-1)
{
}

About::MaxUploadSize::Private::Private(const Private &other)
    : type(other.type)
    , size(other.size)
{
}

About::MaxUploadSize::MaxUploadSize()
    : d(new Private)
{
}

About::MaxUploadSize::MaxUploadSize(const About::MaxUploadSize &other)
    : d(new Private(*(other.d)))
{
}

About::MaxUploadSize::~MaxUploadSize()
{
    delete d;
}

bool About::MaxUploadSize::operator==(const MaxUploadSize &other) const
{
    GAPI_COMPARE(type)
    GAPI_COMPARE(size)
    return true;
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
    bool canCreateDrives;
    UserPtr user;
};

About::Private::Private()
    : quotaBytesTotal(-1)
    , quotaBytesUsed(-1)
    , quotaBytesUsedInTrash(-1)
    , quotaBytesUsedAggregate(-1)
    , largestChangeId(-1)
    , remainingChangeIds(-1)
    , isCurrentAppInstalled(false)
    , canCreateDrives(false)
{
}

About::Private::Private(const About::Private &other)
    : selfLink(other.selfLink)
    , name(other.name)
    , quotaBytesTotal(other.quotaBytesTotal)
    , quotaBytesUsed(other.quotaBytesUsed)
    , quotaBytesUsedInTrash(other.quotaBytesUsedInTrash)
    , quotaBytesUsedAggregate(other.quotaBytesUsedAggregate)
    , largestChangeId(other.largestChangeId)
    , remainingChangeIds(other.remainingChangeIds)
    , rootFolderId(other.rootFolderId)
    , domainSharingPolicy(other.domainSharingPolicy)
    , importFormats(other.importFormats)
    , exportFormats(other.exportFormats)
    , additionalRoleInfo(other.additionalRoleInfo)
    , features(other.features)
    , maxUploadSizes(other.maxUploadSizes)
    , permissionId(other.permissionId)
    , isCurrentAppInstalled(other.isCurrentAppInstalled)
    , canCreateDrives(other.canCreateDrives)
    , user(other.user)
{
}

const QString About::Fields::AdditionalRoleInfo = QStringLiteral("additionalRoleInfo");
const QString About::Fields::AdditionalRoles = QStringLiteral("additionalRoles");
const QString About::Fields::BackgroundImageLink = QStringLiteral("backgroundImageLink");
const QString About::Fields::BytesUsed = QStringLiteral("bytesUsed");
const QString About::Fields::CanCreateDrives = QStringLiteral("canCreateDrives");
const QString About::Fields::ColorRgb = QStringLiteral("colorRgb");
const QString About::Fields::DisplayName = QStringLiteral("displayName");
const QString About::Fields::DomainSharingPolicy = QStringLiteral("domainSharingPolicy");
const QString About::Fields::EmailAddress = QStringLiteral("emailAddress");
const QString About::Fields::Etag = QStringLiteral("etag");
const QString About::Fields::ExportFormats = QStringLiteral("exportFormats");
const QString About::Fields::FeatureName = QStringLiteral("featureName");
const QString About::Fields::FeatureRate = QStringLiteral("featureRate");
const QString About::Fields::Features = QStringLiteral("features");
const QString About::Fields::FolderColorPalette = QStringLiteral("folderColorPalette");
const QString About::Fields::Id = QStringLiteral("id");
const QString About::Fields::ImportFormats = QStringLiteral("importFormats");
const QString About::Fields::IsAuthenticatedUser = QStringLiteral("isAuthenticatedUser");
const QString About::Fields::IsCurrentAppInstalled = QStringLiteral("isCurrentAppInstalled");
const QString About::Fields::Kind = QStringLiteral("kind");
const QString About::Fields::LanguageCode = QStringLiteral("languageCode");
const QString About::Fields::LargestChangeId = QStringLiteral("largestChangeId");
const QString About::Fields::MaxUploadSizes = QStringLiteral("maxUploadSizes");
const QString About::Fields::Name = QStringLiteral("name");
const QString About::Fields::PermissionId = QStringLiteral("permissionId");
const QString About::Fields::Picture = QStringLiteral("picture");
const QString About::Fields::PrimaryRole = QStringLiteral("primaryRole");
const QString About::Fields::QuotaBytesByService = QStringLiteral("quotaBytesByService");
const QString About::Fields::QuotaBytesTotal = QStringLiteral("quotaBytesTotal");
const QString About::Fields::QuotaBytesUsedAggregate = QStringLiteral("quotaBytesUsedAggregate");
const QString About::Fields::QuotaBytesUsedInTrash = QStringLiteral("quotaBytesUsedInTrash");
const QString About::Fields::QuotaBytesUsed = QStringLiteral("quotaBytesUsed");
const QString About::Fields::QuotaType = QStringLiteral("quotaType");
const QString About::Fields::RemainingChangeIds = QStringLiteral("remainingChangeIds");
const QString About::Fields::RoleSets = QStringLiteral("roleSets");
const QString About::Fields::RootFolderId = QStringLiteral("rootFolderId");
const QString About::Fields::SelfLink = QStringLiteral("selfLink");
const QString About::Fields::ServiceName = QStringLiteral("serviceName");
const QString About::Fields::Size = QStringLiteral("size");
const QString About::Fields::Source = QStringLiteral("source");
const QString About::Fields::Targets = QStringLiteral("targets");
const QString About::Fields::TeamDriveThemes = QStringLiteral("teamDriveThemes");
const QString About::Fields::Type = QStringLiteral("type");
const QString About::Fields::Url = QStringLiteral("url");
const QString About::Fields::User = QStringLiteral("user");

About::About()
    : KGAPI2::Object()
    , d(new Private)
{
}

About::About(const About &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

About::~About() = default;

bool About::operator==(const About &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }

    GAPI_COMPARE(selfLink)
    GAPI_COMPARE(name)
    GAPI_COMPARE(quotaBytesTotal)
    GAPI_COMPARE(quotaBytesUsed)
    GAPI_COMPARE(quotaBytesUsedInTrash)
    GAPI_COMPARE(quotaBytesUsedInTrash)
    GAPI_COMPARE(largestChangeId)
    GAPI_COMPARE(remainingChangeIds)
    GAPI_COMPARE(rootFolderId)
    GAPI_COMPARE(domainSharingPolicy)
    GAPI_COMPARE_CONTAINERS(importFormats)
    GAPI_COMPARE_CONTAINERS(exportFormats)
    GAPI_COMPARE_CONTAINERS(additionalRoleInfo)
    GAPI_COMPARE_CONTAINERS(features)
    GAPI_COMPARE_CONTAINERS(maxUploadSizes)
    GAPI_COMPARE(permissionId)
    GAPI_COMPARE(isCurrentAppInstalled)
    GAPI_COMPARE(canCreateDrives)
    GAPI_COMPARE_SHAREDPTRS(user)
    return true;
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

bool About::canCreateDrives() const
{
    return d->canCreateDrives;
}

AboutPtr About::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return AboutPtr();
    }
    const QVariantMap map = document.toVariant().toMap();

    if (!map.contains(QLatin1String("kind")) || map[QStringLiteral("kind")].toString() != QLatin1String("drive#about")) {
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
    about->d->canCreateDrives = map.value(QStringLiteral("canCreateDrives")).toBool();

    const QVariantList importFormats = map.value(QStringLiteral("importFormats")).toList();
    for (const QVariant &v : importFormats) {
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
