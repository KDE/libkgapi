/*
    Copyright (C) 2019  David Barchiesi <david@barchie.si>

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
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "driveservice.h"
#include "teamdrive.h"
#include "utils_p.h"

#include <QVariant>
#include <QUrlQuery>
#include <QJsonDocument>

namespace {
    static const QString ApiKind = QStringLiteral("drive#teamDrive");
    static const QString ApiKindList = QStringLiteral("drive#teamDriveList");
    static const QString AdminManagedRestrictionsAttr = QStringLiteral("adminManagedRestrictions");
    static const QString BackgroundImageFileAttr = QStringLiteral("backgroundImageFile");
    static const QString BackgroundImageLinkAttr = QStringLiteral("backgroundImageLink");
    static const QString CanAddChildrenAttr = QStringLiteral("canAddChildren");
    static const QString CanChangeCopyRequiresWriterPermissionRestrictionAttr = QStringLiteral("canChangeCopyRequiresWriterPermissionRestriction");
    static const QString CanChangeDomainUsersOnlyRestrictionAttr = QStringLiteral("canChangeDomainUsersOnlyRestriction");
    static const QString CanChangeTeamDriveBackgroundAttr = QStringLiteral("canChangeTeamDriveBackground");
    static const QString CanChangeTeamMembersOnlyRestrictionAttr = QStringLiteral("canChangeTeamMembersOnlyRestriction");
    static const QString CanCommentAttr = QStringLiteral("canComment");
    static const QString CanCopyAttr = QStringLiteral("canCopy");
    static const QString CanDeleteChildrenAttr = QStringLiteral("canDeleteChildren");
    static const QString CanDeleteTeamDriveAttr = QStringLiteral("canDeleteTeamDrive");
    static const QString CanDownloadAttr = QStringLiteral("canDownload");
    static const QString CanEditAttr = QStringLiteral("canEdit");
    static const QString CanListChildrenAttr = QStringLiteral("canListChildren");
    static const QString CanManageMembersAttr = QStringLiteral("canManageMembers");
    static const QString CanReadRevisionsAttr = QStringLiteral("canReadRevisions");
    static const QString CanRenameAttr = QStringLiteral("canRename");
    static const QString CanRenameTeamDriveAttr = QStringLiteral("canRenameTeamDrive");
    static const QString CanShareAttr = QStringLiteral("canShare");
    static const QString CanTrashChildrenAttr = QStringLiteral("canTrashChildren");
    static const QString CapabilitiesAttr = QStringLiteral("capabilities");
    static const QString ColorRgbAttr = QStringLiteral("colorRgb");
    static const QString CopyRequiresWriterPermissionAttr = QStringLiteral("copyRequiresWriterPermission");
    static const QString CreatedDateAttr = QStringLiteral("createdDate");
    static const QString DomainUsersOnlyAttr = QStringLiteral("domainUsersOnly");
    static const QString IdAttr = QStringLiteral("id");
    static const QString ItemsAttr = QStringLiteral("items");
    static const QString KindAttr = QStringLiteral("kind");
    static const QString KindDriveAttr = QStringLiteral("kind");
    static const QString NameAttr = QStringLiteral("name");
    static const QString PageTokenAttr = QStringLiteral("pageToken");
    static const QString NextPageTokenAttr = QStringLiteral("nextPageToken");
    static const QString RestrictionsAttr = QStringLiteral("restrictions");
    static const QString TeamMembersOnlyAttr = QStringLiteral("teamMembersOnly");
    static const QString ThemeIdAttr = QStringLiteral("themeId");
    static const QString WidthAttr = QStringLiteral("width");
    static const QString XCoordinateAttr = QStringLiteral("xCoordinate");
    static const QString YCoordinateAttr = QStringLiteral("yCoordinate");
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

Teamdrive::Restrictions::Restrictions():
    d(new Private)
{
}

Teamdrive::Restrictions::Restrictions(const Teamdrive::Restrictions &other):
    d(new Private(*(other.d)))
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

Teamdrive::Capabilities::Capabilities():
    d(new Private)
{
}

Teamdrive::Capabilities::Capabilities(const Teamdrive::Capabilities &other):
    d(new Private(*(other.d)))
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

Teamdrive::BackgroundImageFile::BackgroundImageFile():
    d(new Private)
{
}

Teamdrive::BackgroundImageFile::BackgroundImageFile(const Teamdrive::BackgroundImageFile &other):
    d(new Private(*(other.d)))
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
    Private(const Private& other) = default;

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
    if (!map.contains(KindAttr) ||
        map[KindAttr].toString() != ApiKind)
    {
        return TeamdrivePtr();
    }

    auto teamdrive = TeamdrivePtr::create();
    if (map.contains(IdAttr)) {
        teamdrive->d->id = map[IdAttr].toString();
    }
    if (map.contains(NameAttr)) {
        teamdrive->d->name = map[NameAttr].toString();
    }
    if (map.contains(ThemeIdAttr)) {
        teamdrive->d->themeId = map[ThemeIdAttr].toString();
    }
    if (map.contains(ColorRgbAttr)) {
        teamdrive->d->colorRgb = map[ColorRgbAttr].toString();
    }
    if (map.contains(BackgroundImageLinkAttr)) {
        teamdrive->d->backgroundImageLink = map[BackgroundImageLinkAttr].toString();
    }
    if (map.contains(CreatedDateAttr)) {
        teamdrive->d->createdDate = QDateTime::fromString(map[CreatedDateAttr].toString(), Qt::ISODate);
    }

    if (map.contains(BackgroundImageFileAttr)) {
        const QVariantMap backgroundImageFileMap = map[BackgroundImageFileAttr].toMap();
        auto backgroundImageFile = BackgroundImageFilePtr::create();
        backgroundImageFile->d->id = backgroundImageFileMap[IdAttr].toString();
        backgroundImageFile->d->xCoordinate = backgroundImageFileMap[XCoordinateAttr].toReal();
        backgroundImageFile->d->yCoordinate = backgroundImageFileMap[YCoordinateAttr].toReal();
        backgroundImageFile->d->width = backgroundImageFileMap[WidthAttr].toReal();
        teamdrive->d->backgroundImageFile = backgroundImageFile;
    }

    if (map.contains(CapabilitiesAttr)) {
        const QVariantMap capabilitiesMap = map[CapabilitiesAttr].toMap();
        auto capabilities = CapabilitiesPtr::create();
        capabilities->d->canAddChildren = capabilitiesMap[CanAddChildrenAttr].toBool();
        capabilities->d->canChangeCopyRequiresWriterPermissionRestriction = capabilitiesMap[CanChangeCopyRequiresWriterPermissionRestrictionAttr].toBool();
        capabilities->d->canChangeDomainUsersOnlyRestriction = capabilitiesMap[CanChangeDomainUsersOnlyRestrictionAttr].toBool();
        capabilities->d->canChangeTeamDriveBackground = capabilitiesMap[CanChangeTeamDriveBackgroundAttr].toBool();
        capabilities->d->canChangeTeamMembersOnlyRestriction = capabilitiesMap[CanChangeTeamMembersOnlyRestrictionAttr].toBool();
        capabilities->d->canComment = capabilitiesMap[CanCommentAttr].toBool();
        capabilities->d->canCopy = capabilitiesMap[CanCopyAttr].toBool();
        capabilities->d->canDeleteChildren = capabilitiesMap[CanDeleteChildrenAttr].toBool();
        capabilities->d->canDeleteTeamDrive = capabilitiesMap[CanDeleteTeamDriveAttr].toBool();
        capabilities->d->canDownload = capabilitiesMap[CanDownloadAttr].toBool();
        capabilities->d->canEdit = capabilitiesMap[CanEditAttr].toBool();
        capabilities->d->canListChildren = capabilitiesMap[CanListChildrenAttr].toBool();
        capabilities->d->canManageMembers = capabilitiesMap[CanManageMembersAttr].toBool();
        capabilities->d->canReadRevisions = capabilitiesMap[CanReadRevisionsAttr].toBool();
        capabilities->d->canRename = capabilitiesMap[CanRenameAttr].toBool();
        capabilities->d->canRenameTeamDrive = capabilitiesMap[CanRenameTeamDriveAttr].toBool();
        capabilities->d->canShare = capabilitiesMap[CanShareAttr].toBool();
        capabilities->d->canTrashChildren = capabilitiesMap[CanTrashChildrenAttr].toBool();
        teamdrive->d->capabilities = capabilities;
    }

    if (map.contains(RestrictionsAttr)) {
        const QVariantMap restrictionsMap = map[RestrictionsAttr].toMap();
        auto restrictions = RestrictionsPtr::create();
        restrictions->d->adminManagedRestrictions = restrictionsMap[AdminManagedRestrictionsAttr].toBool();
        restrictions->d->copyRequiresWriterPermission = restrictionsMap[CopyRequiresWriterPermissionAttr].toBool();
        restrictions->d->domainUsersOnly = restrictionsMap[DomainUsersOnlyAttr].toBool();
        restrictions->d->teamMembersOnly = restrictionsMap[TeamMembersOnlyAttr].toBool();
        teamdrive->d->restrictions = restrictions;
    }

    return teamdrive;
}

Teamdrive::Teamdrive():
    KGAPI2::Object(),
    d(new Private)
{
}

Teamdrive::Teamdrive(const Teamdrive& other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
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
    if (!map.contains(KindAttr) ||
            map[KindAttr].toString() != ApiKindList) {
        return TeamdrivesList();
    }

    if (map.contains(NextPageTokenAttr)) {
        feedData.nextPageUrl = DriveService::fetchTeamdrivesUrl();
        QUrlQuery query(feedData.nextPageUrl);
        query.addQueryItem(PageTokenAttr, map.value(NextPageTokenAttr).toString());
        feedData.nextPageUrl.setQuery(query);
    }

    TeamdrivesList list;
    const QVariantList items = map[ItemsAttr].toList();
    for (const QVariant & item : items) {
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
    teamDriveMap[KindAttr] = ApiKind;
    if (!teamdrive->id().isEmpty()) {
        teamDriveMap[IdAttr] = teamdrive->id();
    }
    if (!teamdrive->name().isEmpty()) {
        teamDriveMap[NameAttr] = teamdrive->name();
    }
    if (!teamdrive->themeId().isEmpty()) {
        teamDriveMap[ThemeIdAttr] = teamdrive->themeId();
    }
    if (!teamdrive->colorRgb().isEmpty()) {
        teamDriveMap[ColorRgbAttr] = teamdrive->colorRgb();
    }
    if (!teamdrive->backgroundImageLink().isEmpty()) {
        teamDriveMap[BackgroundImageLinkAttr] = teamdrive->backgroundImageLink();
    }
    if (teamdrive->createdDate().isValid()) {
        teamDriveMap[CreatedDateAttr] = teamdrive->createdDate();
    }

    if (teamdrive->restrictions()) {
        QVariantMap restrictionsMap;
        restrictionsMap[AdminManagedRestrictionsAttr] = teamdrive->restrictions()->adminManagedRestrictions();
        restrictionsMap[CopyRequiresWriterPermissionAttr] = teamdrive->restrictions()->copyRequiresWriterPermission();
        restrictionsMap[DomainUsersOnlyAttr] = teamdrive->restrictions()->domainUsersOnly();
        restrictionsMap[TeamMembersOnlyAttr] = teamdrive->restrictions()->teamMembersOnly();
        teamDriveMap[RestrictionsAttr] = restrictionsMap;
    }

    if (teamdrive->backgroundImageFile()) {
        QVariantMap backgroundImageFileMap;
        backgroundImageFileMap[IdAttr] = teamdrive->backgroundImageFile()->id();
        backgroundImageFileMap[XCoordinateAttr] = teamdrive->backgroundImageFile()->xCoordinate();
        backgroundImageFileMap[YCoordinateAttr] = teamdrive->backgroundImageFile()->yCoordinate();
        backgroundImageFileMap[WidthAttr] = teamdrive->backgroundImageFile()->width();
        teamDriveMap[BackgroundImageFileAttr] = backgroundImageFileMap;
    }

    if (teamdrive->capabilities()) {
        QVariantMap capabilitiesMap;
        capabilitiesMap[CanAddChildrenAttr] = teamdrive->capabilities()->canAddChildren();
        capabilitiesMap[CanChangeCopyRequiresWriterPermissionRestrictionAttr] = teamdrive->capabilities()->canChangeCopyRequiresWriterPermissionRestriction();
        capabilitiesMap[CanChangeDomainUsersOnlyRestrictionAttr] = teamdrive->capabilities()->canChangeDomainUsersOnlyRestriction();
        capabilitiesMap[CanChangeTeamDriveBackgroundAttr] = teamdrive->capabilities()->canChangeTeamDriveBackground();
        capabilitiesMap[CanChangeTeamMembersOnlyRestrictionAttr] = teamdrive->capabilities()->canChangeTeamMembersOnlyRestriction();
        capabilitiesMap[CanCommentAttr] = teamdrive->capabilities()->canComment();
        capabilitiesMap[CanCopyAttr] = teamdrive->capabilities()->canCopy();
        capabilitiesMap[CanDeleteChildrenAttr] = teamdrive->capabilities()->canDeleteChildren();
        capabilitiesMap[CanDeleteTeamDriveAttr] = teamdrive->capabilities()->canDeleteTeamDrive();
        capabilitiesMap[CanDownloadAttr] = teamdrive->capabilities()->canDownload();
        capabilitiesMap[CanEditAttr] = teamdrive->capabilities()->canEdit();
        capabilitiesMap[CanListChildrenAttr] = teamdrive->capabilities()->canListChildren();
        capabilitiesMap[CanManageMembersAttr] = teamdrive->capabilities()->canManageMembers();
        capabilitiesMap[CanReadRevisionsAttr] = teamdrive->capabilities()->canReadRevisions();
        capabilitiesMap[CanRenameAttr] = teamdrive->capabilities()->canRename();
        capabilitiesMap[CanRenameTeamDriveAttr] = teamdrive->capabilities()->canRenameTeamDrive();
        capabilitiesMap[CanShareAttr] = teamdrive->capabilities()->canShare();
        capabilitiesMap[CanTrashChildrenAttr] = teamdrive->capabilities()->canTrashChildren();
        teamDriveMap[CapabilitiesAttr] = capabilitiesMap;
    }

    QJsonDocument document = QJsonDocument::fromVariant(teamDriveMap);
    return document.toJson(QJsonDocument::Compact);
}
