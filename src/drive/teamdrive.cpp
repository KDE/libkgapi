/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "teamdrive.h"
#include "driveservice.h"
#include "utils_p.h"

#include <QJsonDocument>
#include <QUrlQuery>
#include <QVariant>

namespace
{
static const QString ApiKind = QStringLiteral("drive#teamDrive");
static const QString ApiKindList = QStringLiteral("drive#teamDriveList");
}

using namespace KGAPI2;
using namespace KGAPI2::Drive;

///// DriveTeamdrive::Restrictions

class Q_DECL_HIDDEN Teamdrive::Restrictions::Private
{
public:
    Private() = default;
    Private(const Private &other) = default;

    bool adminManagedRestrictions = false;
    bool copyRequiresWriterPermission = false;
    bool domainUsersOnly = false;
    bool teamMembersOnly = false;
};

Teamdrive::Restrictions::Restrictions()
    : d(new Private)
{
}

Teamdrive::Restrictions::Restrictions(const Teamdrive::Restrictions &other)
    : d(new Private(*(other.d)))
{
}

Teamdrive::Restrictions::~Restrictions() = default;

bool Teamdrive::Restrictions::operator==(const Teamdrive::Restrictions &other) const
{
    GAPI_COMPARE(adminManagedRestrictions);
    GAPI_COMPARE(copyRequiresWriterPermission);
    GAPI_COMPARE(domainUsersOnly);
    GAPI_COMPARE(teamMembersOnly);
    return true;
}

bool Teamdrive::Restrictions::adminManagedRestrictions() const
{
    return d->adminManagedRestrictions;
}

void Teamdrive::Restrictions::setAdminManagedRestrictions(bool adminManagedRestrictions) const
{
    d->adminManagedRestrictions = adminManagedRestrictions;
}

bool Teamdrive::Restrictions::copyRequiresWriterPermission() const
{
    return d->copyRequiresWriterPermission;
}

void Teamdrive::Restrictions::setCopyRequiresWriterPermission(bool copyRequiresWriterPermission) const
{
    d->copyRequiresWriterPermission = copyRequiresWriterPermission;
}

bool Teamdrive::Restrictions::domainUsersOnly() const
{
    return d->domainUsersOnly;
}

void Teamdrive::Restrictions::setDomainUsersOnly(bool domainUsersOnly) const
{
    d->domainUsersOnly = domainUsersOnly;
}

bool Teamdrive::Restrictions::teamMembersOnly() const
{
    return d->teamMembersOnly;
}

void Teamdrive::Restrictions::setTeamMembersOnly(bool teamMembersOnly) const
{
    d->teamMembersOnly = teamMembersOnly;
}

///// DriveTeamdrive::Capabilities

class Q_DECL_HIDDEN Teamdrive::Capabilities::Private
{
public:
    Private() = default;
    Private(const Private &other) = default;

    bool canAddChildren = false;
    bool canChangeCopyRequiresWriterPermissionRestriction = false;
    bool canChangeDomainUsersOnlyRestriction = false;
    bool canChangeTeamDriveBackground = false;
    bool canChangeTeamMembersOnlyRestriction = false;
    bool canComment = false;
    bool canCopy = false;
    bool canDeleteChildren = false;
    bool canDeleteTeamDrive = false;
    bool canDownload = false;
    bool canEdit = false;
    bool canListChildren = false;
    bool canManageMembers = false;
    bool canReadRevisions = false;
    bool canRename = false;
    bool canRenameTeamDrive = false;
    bool canShare = false;
    bool canTrashChildren = false;
};

Teamdrive::Capabilities::Capabilities()
    : d(new Private)
{
}

Teamdrive::Capabilities::Capabilities(const Teamdrive::Capabilities &other)
    : d(new Private(*(other.d)))
{
}

Teamdrive::Capabilities::~Capabilities() = default;

bool Teamdrive::Capabilities::operator==(const Teamdrive::Capabilities &other) const
{
    GAPI_COMPARE(canAddChildren);
    GAPI_COMPARE(canChangeCopyRequiresWriterPermissionRestriction);
    GAPI_COMPARE(canChangeDomainUsersOnlyRestriction);
    GAPI_COMPARE(canChangeTeamDriveBackground);
    GAPI_COMPARE(canChangeTeamMembersOnlyRestriction);
    GAPI_COMPARE(canComment);
    GAPI_COMPARE(canCopy);
    GAPI_COMPARE(canDeleteChildren);
    GAPI_COMPARE(canDeleteTeamDrive);
    GAPI_COMPARE(canDownload);
    GAPI_COMPARE(canEdit);
    GAPI_COMPARE(canListChildren);
    GAPI_COMPARE(canManageMembers);
    GAPI_COMPARE(canReadRevisions);
    GAPI_COMPARE(canRename);
    GAPI_COMPARE(canRenameTeamDrive);
    GAPI_COMPARE(canShare);
    GAPI_COMPARE(canTrashChildren);
    return true;
}

bool Teamdrive::Capabilities::canAddChildren() const
{
    return d->canAddChildren;
}

bool Teamdrive::Capabilities::canChangeCopyRequiresWriterPermissionRestriction() const
{
    return d->canChangeCopyRequiresWriterPermissionRestriction;
}

bool Teamdrive::Capabilities::canChangeDomainUsersOnlyRestriction() const
{
    return d->canChangeDomainUsersOnlyRestriction;
}

bool Teamdrive::Capabilities::canChangeTeamDriveBackground() const
{
    return d->canChangeTeamDriveBackground;
}

bool Teamdrive::Capabilities::canChangeTeamMembersOnlyRestriction() const
{
    return d->canChangeTeamMembersOnlyRestriction;
}

bool Teamdrive::Capabilities::canComment() const
{
    return d->canComment;
}

bool Teamdrive::Capabilities::canCopy() const
{
    return d->canCopy;
}

bool Teamdrive::Capabilities::canDeleteChildren() const
{
    return d->canDeleteChildren;
}

bool Teamdrive::Capabilities::canDeleteTeamDrive() const
{
    return d->canDeleteTeamDrive;
}

bool Teamdrive::Capabilities::canDownload() const
{
    return d->canDownload;
}

bool Teamdrive::Capabilities::canEdit() const
{
    return d->canEdit;
}

bool Teamdrive::Capabilities::canListChildren() const
{
    return d->canListChildren;
}

bool Teamdrive::Capabilities::canManageMembers() const
{
    return d->canManageMembers;
}

bool Teamdrive::Capabilities::canReadRevisions() const
{
    return d->canReadRevisions;
}

bool Teamdrive::Capabilities::canRename() const
{
    return d->canRename;
}

bool Teamdrive::Capabilities::canRenameTeamDrive() const
{
    return d->canRenameTeamDrive;
}

bool Teamdrive::Capabilities::canShare() const
{
    return d->canShare;
}

bool Teamdrive::Capabilities::canTrashChildren() const
{
    return d->canTrashChildren;
}

///// DriveTeamdrive::BackgroundImageFile

class Q_DECL_HIDDEN Teamdrive::BackgroundImageFile::Private
{
public:
    Private() = default;
    Private(const Private &other) = default;

    QString id;
    float xCoordinate = 0.0f;
    float yCoordinate = 0.0f;
    float width = 0.0f;
};

Teamdrive::BackgroundImageFile::BackgroundImageFile()
    : d(new Private)
{
}

Teamdrive::BackgroundImageFile::BackgroundImageFile(const Teamdrive::BackgroundImageFile &other)
    : d(new Private(*(other.d)))
{
}

Teamdrive::BackgroundImageFile::~BackgroundImageFile() = default;

bool Teamdrive::BackgroundImageFile::operator==(const Teamdrive::BackgroundImageFile &other) const
{
    GAPI_COMPARE(id);
    GAPI_COMPARE(xCoordinate);
    GAPI_COMPARE(yCoordinate);
    GAPI_COMPARE(width);
    return true;
}

QString Teamdrive::BackgroundImageFile::id() const
{
    return d->id;
}

void Teamdrive::BackgroundImageFile::setId(const QString &id) const
{
    d->id = id;
}

float Teamdrive::BackgroundImageFile::xCoordinate() const
{
    return d->xCoordinate;
}

void Teamdrive::BackgroundImageFile::setXCoordinate(const float xCoordinate) const
{
    d->xCoordinate = xCoordinate;
}

float Teamdrive::BackgroundImageFile::yCoordinate() const
{
    return d->yCoordinate;
}

void Teamdrive::BackgroundImageFile::setYCoordinate(const float yCoordinate) const
{
    d->yCoordinate = yCoordinate;
}

float Teamdrive::BackgroundImageFile::width() const
{
    return d->width;
}

void Teamdrive::BackgroundImageFile::setWidth(const float width) const
{
    d->width = width;
}

///// DriveTeamdrive

class Q_DECL_HIDDEN Teamdrive::Private
{
public:
    Private() = default;
    Private(const Private &other) = default;

    QString id;
    QString name;
    QString themeId;
    QString colorRgb;
    BackgroundImageFilePtr backgroundImageFile;
    QString backgroundImageLink;
    CapabilitiesPtr capabilities;
    QDateTime createdDate;
    RestrictionsPtr restrictions;

    static TeamdrivePtr fromJSON(const QVariantMap &map);
};

TeamdrivePtr Teamdrive::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(Teamdrive::Fields::Kind) || map[Teamdrive::Fields::Kind].toString() != ApiKind) {
        return TeamdrivePtr();
    }

    auto teamdrive = TeamdrivePtr::create();
    if (map.contains(Teamdrive::Fields::Id)) {
        teamdrive->d->id = map[Teamdrive::Fields::Id].toString();
    }
    if (map.contains(Teamdrive::Fields::Name)) {
        teamdrive->d->name = map[Teamdrive::Fields::Name].toString();
    }
    if (map.contains(Teamdrive::Fields::ThemeId)) {
        teamdrive->d->themeId = map[Teamdrive::Fields::ThemeId].toString();
    }
    if (map.contains(Teamdrive::Fields::ColorRgb)) {
        teamdrive->d->colorRgb = map[Teamdrive::Fields::ColorRgb].toString();
    }
    if (map.contains(Teamdrive::Fields::BackgroundImageLink)) {
        teamdrive->d->backgroundImageLink = map[Teamdrive::Fields::BackgroundImageLink].toString();
    }
    if (map.contains(Teamdrive::Fields::CreatedDate)) {
        teamdrive->d->createdDate = QDateTime::fromString(map[Teamdrive::Fields::CreatedDate].toString(), Qt::ISODate);
    }

    if (map.contains(Teamdrive::Fields::BackgroundImageFile)) {
        const QVariantMap backgroundImageFileMap = map[Teamdrive::Fields::BackgroundImageFile].toMap();
        auto backgroundImageFile = BackgroundImageFilePtr::create();
        backgroundImageFile->d->id = backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::Id].toString();
        backgroundImageFile->d->xCoordinate = backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::XCoordinate].toReal();
        backgroundImageFile->d->yCoordinate = backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::YCoordinate].toReal();
        backgroundImageFile->d->width = backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::Width].toReal();
        teamdrive->d->backgroundImageFile = backgroundImageFile;
    }

    if (map.contains(Teamdrive::Fields::Capabilities)) {
        const QVariantMap capabilitiesMap = map[Teamdrive::Fields::Capabilities].toMap();
        auto capabilities = CapabilitiesPtr::create();
        capabilities->d->canAddChildren = capabilitiesMap[Teamdrive::Capabilities::Fields::CanAddChildren].toBool();
        capabilities->d->canChangeCopyRequiresWriterPermissionRestriction =
            capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeCopyRequiresWriterPermissionRestriction].toBool();
        capabilities->d->canChangeDomainUsersOnlyRestriction = capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeDomainUsersOnlyRestriction].toBool();
        capabilities->d->canChangeTeamDriveBackground = capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeTeamDriveBackground].toBool();
        capabilities->d->canChangeTeamMembersOnlyRestriction = capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeTeamMembersOnlyRestriction].toBool();
        capabilities->d->canComment = capabilitiesMap[Teamdrive::Capabilities::Fields::CanComment].toBool();
        capabilities->d->canCopy = capabilitiesMap[Teamdrive::Capabilities::Fields::CanCopy].toBool();
        capabilities->d->canDeleteChildren = capabilitiesMap[Teamdrive::Capabilities::Fields::CanDeleteChildren].toBool();
        capabilities->d->canDeleteTeamDrive = capabilitiesMap[Teamdrive::Capabilities::Fields::CanDeleteTeamDrive].toBool();
        capabilities->d->canDownload = capabilitiesMap[Teamdrive::Capabilities::Fields::CanDownload].toBool();
        capabilities->d->canEdit = capabilitiesMap[Teamdrive::Capabilities::Fields::CanEdit].toBool();
        capabilities->d->canListChildren = capabilitiesMap[Teamdrive::Capabilities::Fields::CanListChildren].toBool();
        capabilities->d->canManageMembers = capabilitiesMap[Teamdrive::Capabilities::Fields::CanManageMembers].toBool();
        capabilities->d->canReadRevisions = capabilitiesMap[Teamdrive::Capabilities::Fields::CanReadRevisions].toBool();
        capabilities->d->canRename = capabilitiesMap[Teamdrive::Capabilities::Fields::CanRename].toBool();
        capabilities->d->canRenameTeamDrive = capabilitiesMap[Teamdrive::Capabilities::Fields::CanRenameTeamDrive].toBool();
        capabilities->d->canShare = capabilitiesMap[Teamdrive::Capabilities::Fields::CanShare].toBool();
        capabilities->d->canTrashChildren = capabilitiesMap[Teamdrive::Capabilities::Fields::CanTrashChildren].toBool();
        teamdrive->d->capabilities = capabilities;
    }

    if (map.contains(Teamdrive::Fields::Restrictions)) {
        const QVariantMap restrictionsMap = map[Teamdrive::Fields::Restrictions].toMap();
        auto restrictions = RestrictionsPtr::create();
        restrictions->d->adminManagedRestrictions = restrictionsMap[Teamdrive::Restrictions::Fields::AdminManagedRestrictions].toBool();
        restrictions->d->copyRequiresWriterPermission = restrictionsMap[Teamdrive::Restrictions::Fields::CopyRequiresWriterPermission].toBool();
        restrictions->d->domainUsersOnly = restrictionsMap[Teamdrive::Restrictions::Fields::DomainUsersOnly].toBool();
        restrictions->d->teamMembersOnly = restrictionsMap[Teamdrive::Restrictions::Fields::TeamMembersOnly].toBool();
        teamdrive->d->restrictions = restrictions;
    }

    return teamdrive;
}

const QString Teamdrive::Restrictions::Fields::AdminManagedRestrictions = QStringLiteral("adminManagedRestrictions");
const QString Teamdrive::Restrictions::Fields::CopyRequiresWriterPermission = QStringLiteral("copyRequiresWriterPermission");
const QString Teamdrive::Restrictions::Fields::DomainUsersOnly = QStringLiteral("domainUsersOnly");
const QString Teamdrive::Restrictions::Fields::TeamMembersOnly = QStringLiteral("teamMembersOnly");

const QString Teamdrive::Capabilities::Fields::CanAddChildren = QStringLiteral("canAddChildren");
const QString Teamdrive::Capabilities::Fields::CanChangeCopyRequiresWriterPermissionRestriction =
    QStringLiteral("canChangeCopyRequiresWriterPermissionRestriction");
const QString Teamdrive::Capabilities::Fields::CanChangeDomainUsersOnlyRestriction = QStringLiteral("canChangeDomainUsersOnlyRestriction");
const QString Teamdrive::Capabilities::Fields::CanChangeTeamDriveBackground = QStringLiteral("canChangeTeamDriveBackground");
const QString Teamdrive::Capabilities::Fields::CanChangeTeamMembersOnlyRestriction = QStringLiteral("canChangeTeamMembersOnlyRestriction");
const QString Teamdrive::Capabilities::Fields::CanComment = QStringLiteral("canComment");
const QString Teamdrive::Capabilities::Fields::CanCopy = QStringLiteral("canCopy");
const QString Teamdrive::Capabilities::Fields::CanDeleteChildren = QStringLiteral("canDeleteChildren");
const QString Teamdrive::Capabilities::Fields::CanDeleteTeamDrive = QStringLiteral("canDeleteTeamDrive");
const QString Teamdrive::Capabilities::Fields::CanDownload = QStringLiteral("canDownload");
const QString Teamdrive::Capabilities::Fields::CanEdit = QStringLiteral("canEdit");
const QString Teamdrive::Capabilities::Fields::CanListChildren = QStringLiteral("canListChildren");
const QString Teamdrive::Capabilities::Fields::CanManageMembers = QStringLiteral("canManageMembers");
const QString Teamdrive::Capabilities::Fields::CanReadRevisions = QStringLiteral("canReadRevisions");
const QString Teamdrive::Capabilities::Fields::CanRename = QStringLiteral("canRename");
const QString Teamdrive::Capabilities::Fields::CanRenameTeamDrive = QStringLiteral("canRenameTeamDrive");
const QString Teamdrive::Capabilities::Fields::CanShare = QStringLiteral("canShare");
const QString Teamdrive::Capabilities::Fields::CanTrashChildren = QStringLiteral("canTrashChildren");

const QString Teamdrive::BackgroundImageFile::Fields::Id = QStringLiteral("id");
const QString Teamdrive::BackgroundImageFile::Fields::XCoordinate = QStringLiteral("xCoordinate");
const QString Teamdrive::BackgroundImageFile::Fields::YCoordinate = QStringLiteral("yCoordinate");
const QString Teamdrive::BackgroundImageFile::Fields::Width = QStringLiteral("width");

const QString Teamdrive::Fields::Items = QStringLiteral("items");
const QString Teamdrive::Fields::KindDrive = QStringLiteral("kind");
const QString Teamdrive::Fields::PageToken = QStringLiteral("pageToken");
const QString Teamdrive::Fields::NextPageToken = QStringLiteral("nextPageToken");
const QString Teamdrive::Fields::Id = QStringLiteral("id");
const QString Teamdrive::Fields::Kind = QStringLiteral("kind");
const QString Teamdrive::Fields::Name = QStringLiteral("name");
const QString Teamdrive::Fields::ThemeId = QStringLiteral("themeId");
const QString Teamdrive::Fields::ColorRgb = QStringLiteral("colorRgb");
const QString Teamdrive::Fields::BackgroundImageFile = QStringLiteral("backgroundImageFile");
const QString Teamdrive::Fields::BackgroundImageLink = QStringLiteral("backgroundImageLink");
const QString Teamdrive::Fields::Capabilities = QStringLiteral("capabilities");
const QString Teamdrive::Fields::CreatedDate = QStringLiteral("createdDate");
const QString Teamdrive::Fields::Restrictions = QStringLiteral("restrictions");

Teamdrive::Teamdrive()
    : KGAPI2::Object()
    , d(new Private)
{
}

Teamdrive::Teamdrive(const Teamdrive &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

Teamdrive::~Teamdrive() = default;

bool Teamdrive::operator==(const Teamdrive &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }
    GAPI_COMPARE(id);
    GAPI_COMPARE(name);
    GAPI_COMPARE(themeId);
    GAPI_COMPARE(colorRgb);
    GAPI_COMPARE_SHAREDPTRS(backgroundImageFile);
    GAPI_COMPARE(backgroundImageLink);
    GAPI_COMPARE_SHAREDPTRS(capabilities);
    GAPI_COMPARE(createdDate);
    GAPI_COMPARE_SHAREDPTRS(restrictions);
    return true;
}

QString Teamdrive::id() const
{
    return d->id;
}

void Teamdrive::setId(const QString &id) const
{
    d->id = id;
}

QString Teamdrive::name() const
{
    return d->name;
}

void Teamdrive::setName(const QString &name) const
{
    d->name = name;
}

QString Teamdrive::themeId() const
{
    return d->themeId;
}

void Teamdrive::setThemeId(const QString &themeId) const
{
    d->themeId = themeId;
}

QString Teamdrive::colorRgb() const
{
    return d->colorRgb;
}

void Teamdrive::setColorRgb(const QString &colorRgb) const
{
    d->colorRgb = colorRgb;
}

Teamdrive::BackgroundImageFilePtr Teamdrive::backgroundImageFile() const
{
    return d->backgroundImageFile;
}

void Teamdrive::setBackgroundImageFile(const Teamdrive::BackgroundImageFilePtr &backgroundImageFile) const
{
    d->backgroundImageFile = backgroundImageFile;
}

QString Teamdrive::backgroundImageLink() const
{
    return d->backgroundImageLink;
}

Teamdrive::CapabilitiesPtr Teamdrive::capabilities() const
{
    return d->capabilities;
}

QDateTime Teamdrive::createdDate() const
{
    return d->createdDate;
}

void Teamdrive::setRestrictions(const Teamdrive::RestrictionsPtr &restrictions) const
{
    d->restrictions = restrictions;
}

Teamdrive::RestrictionsPtr Teamdrive::restrictions() const
{
    return d->restrictions;
}

TeamdrivePtr Teamdrive::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return TeamdrivePtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

TeamdrivesList Teamdrive::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return TeamdrivesList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(Teamdrive::Fields::Kind) || map[Teamdrive::Fields::Kind].toString() != ApiKindList) {
        return TeamdrivesList();
    }

    if (map.contains(Teamdrive::Fields::NextPageToken)) {
        feedData.nextPageUrl = DriveService::fetchTeamdrivesUrl();
        QUrlQuery query(feedData.nextPageUrl);
        query.addQueryItem(Teamdrive::Fields::PageToken, map.value(Teamdrive::Fields::NextPageToken).toString());
        feedData.nextPageUrl.setQuery(query);
    }

    TeamdrivesList list;
    const QVariantList items = map[Teamdrive::Fields::Items].toList();
    for (const QVariant &item : items) {
        const TeamdrivePtr teamdrive = Private::fromJSON(item.toMap());

        if (!teamdrive.isNull()) {
            list << teamdrive;
        }
    }

    return list;
}

QByteArray Teamdrive::toJSON(const TeamdrivePtr &teamdrive)
{
    QVariantMap teamDriveMap;
    teamDriveMap[Teamdrive::Fields::Kind] = ApiKind;
    if (!teamdrive->id().isEmpty()) {
        teamDriveMap[Teamdrive::Fields::Id] = teamdrive->id();
    }
    if (!teamdrive->name().isEmpty()) {
        teamDriveMap[Teamdrive::Fields::Name] = teamdrive->name();
    }
    if (!teamdrive->themeId().isEmpty()) {
        teamDriveMap[Teamdrive::Fields::ThemeId] = teamdrive->themeId();
    }
    if (!teamdrive->colorRgb().isEmpty()) {
        teamDriveMap[Teamdrive::Fields::ColorRgb] = teamdrive->colorRgb();
    }
    if (!teamdrive->backgroundImageLink().isEmpty()) {
        teamDriveMap[Teamdrive::Fields::BackgroundImageLink] = teamdrive->backgroundImageLink();
    }
    if (teamdrive->createdDate().isValid()) {
        teamDriveMap[Teamdrive::Fields::CreatedDate] = teamdrive->createdDate();
    }

    if (teamdrive->restrictions()) {
        QVariantMap restrictionsMap;
        restrictionsMap[Teamdrive::Restrictions::Fields::AdminManagedRestrictions] = teamdrive->restrictions()->adminManagedRestrictions();
        restrictionsMap[Teamdrive::Restrictions::Fields::CopyRequiresWriterPermission] = teamdrive->restrictions()->copyRequiresWriterPermission();
        restrictionsMap[Teamdrive::Restrictions::Fields::DomainUsersOnly] = teamdrive->restrictions()->domainUsersOnly();
        restrictionsMap[Teamdrive::Restrictions::Fields::TeamMembersOnly] = teamdrive->restrictions()->teamMembersOnly();
        teamDriveMap[Teamdrive::Fields::Restrictions] = restrictionsMap;
    }

    if (teamdrive->backgroundImageFile()) {
        QVariantMap backgroundImageFileMap;
        backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::Id] = teamdrive->backgroundImageFile()->id();
        backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::XCoordinate] = teamdrive->backgroundImageFile()->xCoordinate();
        backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::YCoordinate] = teamdrive->backgroundImageFile()->yCoordinate();
        backgroundImageFileMap[Teamdrive::BackgroundImageFile::Fields::Width] = teamdrive->backgroundImageFile()->width();
        teamDriveMap[Teamdrive::Fields::BackgroundImageFile] = backgroundImageFileMap;
    }

    if (teamdrive->capabilities()) {
        QVariantMap capabilitiesMap;
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanAddChildren] = teamdrive->capabilities()->canAddChildren();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeCopyRequiresWriterPermissionRestriction] =
            teamdrive->capabilities()->canChangeCopyRequiresWriterPermissionRestriction();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeDomainUsersOnlyRestriction] =
            teamdrive->capabilities()->canChangeDomainUsersOnlyRestriction();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeTeamDriveBackground] = teamdrive->capabilities()->canChangeTeamDriveBackground();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanChangeTeamMembersOnlyRestriction] =
            teamdrive->capabilities()->canChangeTeamMembersOnlyRestriction();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanComment] = teamdrive->capabilities()->canComment();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanCopy] = teamdrive->capabilities()->canCopy();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanDeleteChildren] = teamdrive->capabilities()->canDeleteChildren();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanDeleteTeamDrive] = teamdrive->capabilities()->canDeleteTeamDrive();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanDownload] = teamdrive->capabilities()->canDownload();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanEdit] = teamdrive->capabilities()->canEdit();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanListChildren] = teamdrive->capabilities()->canListChildren();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanManageMembers] = teamdrive->capabilities()->canManageMembers();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanReadRevisions] = teamdrive->capabilities()->canReadRevisions();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanRename] = teamdrive->capabilities()->canRename();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanRenameTeamDrive] = teamdrive->capabilities()->canRenameTeamDrive();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanShare] = teamdrive->capabilities()->canShare();
        capabilitiesMap[Teamdrive::Capabilities::Fields::CanTrashChildren] = teamdrive->capabilities()->canTrashChildren();
        teamDriveMap[Teamdrive::Fields::Capabilities] = capabilitiesMap;
    }

    QJsonDocument document = QJsonDocument::fromVariant(teamDriveMap);
    return document.toJson(QJsonDocument::Compact);
}
