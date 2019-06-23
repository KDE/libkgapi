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
#include "drives.h"
#include "utils_p.h"

#include <QVariant>
#include <QUrlQuery>
#include <QJsonDocument>

namespace {
    static const QString ApiKind = QStringLiteral("drive#drive");
    static const QString ApiKindList = QStringLiteral("drive#driveList");
}

using namespace KGAPI2;
using namespace KGAPI2::Drive;

///// Drives::Restrictions

class Q_DECL_HIDDEN Drives::Restrictions::Private
{
  public:
    Private() = default;
    Private(const Private &other) = default;

    bool adminManagedRestrictions = false;
    bool copyRequiresWriterPermission = false;
    bool domainUsersOnly = false;
    bool driveMembersOnly = false;
};

Drives::Restrictions::Restrictions():
    d(new Private)
{
}

Drives::Restrictions::Restrictions(const Drives::Restrictions &other):
    d(new Private(*(other.d)))
{
}

Drives::Restrictions::~Restrictions() = default;

bool Drives::Restrictions::operator==(const Drives::Restrictions &other) const
{
    GAPI_COMPARE(adminManagedRestrictions);
    GAPI_COMPARE(copyRequiresWriterPermission);
    GAPI_COMPARE(domainUsersOnly);
    GAPI_COMPARE(driveMembersOnly);
    return true;
}

bool Drives::Restrictions::adminManagedRestrictions() const
{
    return d->adminManagedRestrictions;
}

void Drives::Restrictions::setAdminManagedRestrictions(bool adminManagedRestrictions) const
{
    d->adminManagedRestrictions = adminManagedRestrictions;
}

bool Drives::Restrictions::copyRequiresWriterPermission() const
{
    return d->copyRequiresWriterPermission;
}

void Drives::Restrictions::setCopyRequiresWriterPermission(bool copyRequiresWriterPermission) const
{
    d->copyRequiresWriterPermission = copyRequiresWriterPermission;
}

bool Drives::Restrictions::domainUsersOnly() const
{
    return d->domainUsersOnly;
}

void Drives::Restrictions::setDomainUsersOnly(bool domainUsersOnly) const
{
    d->domainUsersOnly = domainUsersOnly;
}

bool Drives::Restrictions::driveMembersOnly() const
{
    return d->driveMembersOnly;
}

void Drives::Restrictions::setDriveMembersOnly(bool driveMembersOnly) const
{
    d->driveMembersOnly = driveMembersOnly;
}

///// Drives::Capabilities

class Q_DECL_HIDDEN Drives::Capabilities::Private
{
  public:
    Private() = default;
    Private(const Private &other) = default;

    bool canAddChildren = false;
    bool canChangeCopyRequiresWriterPermissionRestriction = false;
    bool canChangeDomainUsersOnlyRestriction = false;
    bool canChangeDriveBackground = false;
    bool canChangeDriveMembersOnlyRestriction = false;
    bool canComment = false;
    bool canCopy = false;
    bool canDeleteChildren = false;
    bool canDeleteDrive = false;
    bool canDownload = false;
    bool canEdit = false;
    bool canListChildren = false;
    bool canManageMembers = false;
    bool canReadRevisions = false;
    bool canRename = false;
    bool canRenameDrive = false;
    bool canShare = false;
    bool canTrashChildren = false;
};

Drives::Capabilities::Capabilities():
    d(new Private)
{
}

Drives::Capabilities::Capabilities(const Drives::Capabilities &other):
    d(new Private(*(other.d)))
{
}

Drives::Capabilities::~Capabilities() = default;

bool Drives::Capabilities::operator==(const Drives::Capabilities &other) const
{
    GAPI_COMPARE(canAddChildren);
    GAPI_COMPARE(canChangeCopyRequiresWriterPermissionRestriction);
    GAPI_COMPARE(canChangeDomainUsersOnlyRestriction);
    GAPI_COMPARE(canChangeDriveBackground);
    GAPI_COMPARE(canChangeDriveMembersOnlyRestriction);
    GAPI_COMPARE(canComment);
    GAPI_COMPARE(canCopy);
    GAPI_COMPARE(canDeleteChildren);
    GAPI_COMPARE(canDeleteDrive);
    GAPI_COMPARE(canDownload);
    GAPI_COMPARE(canEdit);
    GAPI_COMPARE(canListChildren);
    GAPI_COMPARE(canManageMembers);
    GAPI_COMPARE(canReadRevisions);
    GAPI_COMPARE(canRename);
    GAPI_COMPARE(canRenameDrive);
    GAPI_COMPARE(canShare);
    GAPI_COMPARE(canTrashChildren);
    return true;
}

bool Drives::Capabilities::canAddChildren() const
{
    return d->canAddChildren;
}

bool Drives::Capabilities::canChangeCopyRequiresWriterPermissionRestriction() const
{
    return d->canChangeCopyRequiresWriterPermissionRestriction;
}

bool Drives::Capabilities::canChangeDomainUsersOnlyRestriction() const
{
    return d->canChangeDomainUsersOnlyRestriction;
}

bool Drives::Capabilities::canChangeDriveBackground() const
{
    return d->canChangeDriveBackground;
}

bool Drives::Capabilities::canChangeDriveMembersOnlyRestriction() const
{
    return d->canChangeDriveMembersOnlyRestriction;
}

bool Drives::Capabilities::canComment() const
{
    return d->canComment;
}

bool Drives::Capabilities::canCopy() const
{
    return d->canCopy;
}

bool Drives::Capabilities::canDeleteChildren() const
{
    return d->canDeleteChildren;
}

bool Drives::Capabilities::canDeleteDrive() const
{
    return d->canDeleteDrive;
}

bool Drives::Capabilities::canDownload() const
{
    return d->canDownload;
}

bool Drives::Capabilities::canEdit() const
{
    return d->canEdit;
}

bool Drives::Capabilities::canListChildren() const
{
    return d->canListChildren;
}

bool Drives::Capabilities::canManageMembers() const
{
    return d->canManageMembers;
}

bool Drives::Capabilities::canReadRevisions() const
{
    return d->canReadRevisions;
}

bool Drives::Capabilities::canRename() const
{
    return d->canRename;
}

bool Drives::Capabilities::canRenameDrive() const
{
    return d->canRenameDrive;
}

bool Drives::Capabilities::canShare() const
{
    return d->canShare;
}

bool Drives::Capabilities::canTrashChildren() const
{
    return d->canTrashChildren;
}

///// Drives::BackgroundImageFile

class Q_DECL_HIDDEN Drives::BackgroundImageFile::Private
{
  public:
    Private() = default;
    Private(const Private &other) = default;

    QString id;
    float xCoordinate = 0.0f;
    float yCoordinate = 0.0f;
    float width = 0.0f;
};

Drives::BackgroundImageFile::BackgroundImageFile():
    d(new Private)
{
}

Drives::BackgroundImageFile::BackgroundImageFile(const Drives::BackgroundImageFile &other):
    d(new Private(*(other.d)))
{
}

Drives::BackgroundImageFile::~BackgroundImageFile() = default;

bool Drives::BackgroundImageFile::operator==(const Drives::BackgroundImageFile &other) const
{
    GAPI_COMPARE(id);
    GAPI_COMPARE(xCoordinate);
    GAPI_COMPARE(yCoordinate);
    GAPI_COMPARE(width);
    return true;
}

QString Drives::BackgroundImageFile::id() const
{
    return d->id;
}

void Drives::BackgroundImageFile::setId(const QString &id) const
{
    d->id = id;
}

float Drives::BackgroundImageFile::xCoordinate() const
{
    return d->xCoordinate;
}

void Drives::BackgroundImageFile::setXCoordinate(const float xCoordinate) const
{
    d->xCoordinate = xCoordinate;
}

float Drives::BackgroundImageFile::yCoordinate() const
{
    return d->yCoordinate;
}

void Drives::BackgroundImageFile::setYCoordinate(const float yCoordinate) const
{
    d->yCoordinate = yCoordinate;
}

float Drives::BackgroundImageFile::width() const
{
    return d->width;
}

void Drives::BackgroundImageFile::setWidth(const float width) const
{
    d->width = width;
}

///// Drives

class Q_DECL_HIDDEN Drives::Private
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
    bool hidden = false;
    RestrictionsPtr restrictions;

    static DrivesPtr fromJSON(const QVariantMap &map);
};

DrivesPtr Drives::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(Drives::Fields::Kind) ||
        map[Drives::Fields::Kind].toString() != ApiKind)
    {
        return DrivesPtr();
    }

    auto drives = DrivesPtr::create();
    if (map.contains(Drives::Fields::Id)) {
        drives->d->id = map[Drives::Fields::Id].toString();
    }
    if (map.contains(Drives::Fields::Name)) {
        drives->d->name = map[Drives::Fields::Name].toString();
    }
    if (map.contains(Drives::Fields::ThemeId)) {
        drives->d->themeId = map[Drives::Fields::ThemeId].toString();
    }
    if (map.contains(Drives::Fields::ColorRgb)) {
        drives->d->colorRgb = map[Drives::Fields::ColorRgb].toString();
    }
    if (map.contains(Drives::Fields::BackgroundImageLink)) {
        drives->d->backgroundImageLink = map[Drives::Fields::BackgroundImageLink].toString();
    }
    if (map.contains(Drives::Fields::CreatedDate)) {
        drives->d->createdDate = QDateTime::fromString(map[Drives::Fields::CreatedDate].toString(), Qt::ISODate);
    }
    if (map.contains(Drives::Fields::Hidden)) {
        drives->d->hidden = map[Drives::Fields::Hidden].toBool();
    }

    if (map.contains(Drives::Fields::BackgroundImageFile)) {
        const QVariantMap backgroundImageFileMap = map[Drives::Fields::BackgroundImageFile].toMap();
        auto backgroundImageFile = BackgroundImageFilePtr::create();
        backgroundImageFile->d->id = backgroundImageFileMap[Drives::BackgroundImageFile::Fields::Id].toString();
        backgroundImageFile->d->xCoordinate = backgroundImageFileMap[Drives::BackgroundImageFile::Fields::XCoordinate].toReal();
        backgroundImageFile->d->yCoordinate = backgroundImageFileMap[Drives::BackgroundImageFile::Fields::YCoordinate].toReal();
        backgroundImageFile->d->width = backgroundImageFileMap[Drives::BackgroundImageFile::Fields::Width].toReal();
        drives->d->backgroundImageFile = backgroundImageFile;
    }

    if (map.contains(Drives::Fields::Capabilities)) {
        const QVariantMap capabilitiesMap = map[Drives::Fields::Capabilities].toMap();
        auto capabilities = CapabilitiesPtr::create();
        capabilities->d->canAddChildren = capabilitiesMap[Drives::Capabilities::Fields::CanAddChildren].toBool();
        capabilities->d->canChangeCopyRequiresWriterPermissionRestriction = capabilitiesMap[Drives::Capabilities::Fields::CanChangeCopyRequiresWriterPermissionRestriction].toBool();
        capabilities->d->canChangeDomainUsersOnlyRestriction = capabilitiesMap[Drives::Capabilities::Fields::CanChangeDomainUsersOnlyRestriction].toBool();
        capabilities->d->canChangeDriveBackground = capabilitiesMap[Drives::Capabilities::Fields::CanChangeDriveBackground].toBool();
        capabilities->d->canChangeDriveMembersOnlyRestriction = capabilitiesMap[Drives::Capabilities::Fields::CanChangeDriveMembersOnlyRestriction].toBool();
        capabilities->d->canComment = capabilitiesMap[Drives::Capabilities::Fields::CanComment].toBool();
        capabilities->d->canCopy = capabilitiesMap[Drives::Capabilities::Fields::CanCopy].toBool();
        capabilities->d->canDeleteChildren = capabilitiesMap[Drives::Capabilities::Fields::CanDeleteChildren].toBool();
        capabilities->d->canDeleteDrive = capabilitiesMap[Drives::Capabilities::Fields::CanDeleteDrive].toBool();
        capabilities->d->canDownload = capabilitiesMap[Drives::Capabilities::Fields::CanDownload].toBool();
        capabilities->d->canEdit = capabilitiesMap[Drives::Capabilities::Fields::CanEdit].toBool();
        capabilities->d->canListChildren = capabilitiesMap[Drives::Capabilities::Fields::CanListChildren].toBool();
        capabilities->d->canManageMembers = capabilitiesMap[Drives::Capabilities::Fields::CanManageMembers].toBool();
        capabilities->d->canReadRevisions = capabilitiesMap[Drives::Capabilities::Fields::CanReadRevisions].toBool();
        capabilities->d->canRename = capabilitiesMap[Drives::Capabilities::Fields::CanRename].toBool();
        capabilities->d->canRenameDrive = capabilitiesMap[Drives::Capabilities::Fields::CanRenameDrive].toBool();
        capabilities->d->canShare = capabilitiesMap[Drives::Capabilities::Fields::CanShare].toBool();
        capabilities->d->canTrashChildren = capabilitiesMap[Drives::Capabilities::Fields::CanTrashChildren].toBool();
        drives->d->capabilities = capabilities;
    }

    if (map.contains(Drives::Fields::Restrictions)) {
        const QVariantMap restrictionsMap = map[Drives::Fields::Restrictions].toMap();
        auto restrictions = RestrictionsPtr::create();
        restrictions->d->adminManagedRestrictions = restrictionsMap[Drives::Restrictions::Fields::AdminManagedRestrictions].toBool();
        restrictions->d->copyRequiresWriterPermission = restrictionsMap[Drives::Restrictions::Fields::CopyRequiresWriterPermission].toBool();
        restrictions->d->domainUsersOnly = restrictionsMap[Drives::Restrictions::Fields::DomainUsersOnly].toBool();
        restrictions->d->driveMembersOnly = restrictionsMap[Drives::Restrictions::Fields::DriveMembersOnly].toBool();
        drives->d->restrictions = restrictions;
    }

    return drives;
}

const QString Drives::Restrictions::Fields::AdminManagedRestrictions = QStringLiteral("adminManagedRestrictions");
const QString Drives::Restrictions::Fields::CopyRequiresWriterPermission = QStringLiteral("copyRequiresWriterPermission");
const QString Drives::Restrictions::Fields::DomainUsersOnly = QStringLiteral("domainUsersOnly");
const QString Drives::Restrictions::Fields::DriveMembersOnly = QStringLiteral("driveMembersOnly");

const QString Drives::Capabilities::Fields::CanAddChildren = QStringLiteral("canAddChildren");
const QString Drives::Capabilities::Fields::CanChangeCopyRequiresWriterPermissionRestriction = QStringLiteral("canChangeCopyRequiresWriterPermissionRestriction");
const QString Drives::Capabilities::Fields::CanChangeDomainUsersOnlyRestriction = QStringLiteral("canChangeDomainUsersOnlyRestriction");
const QString Drives::Capabilities::Fields::CanChangeDriveBackground = QStringLiteral("canChangeDriveBackground");
const QString Drives::Capabilities::Fields::CanChangeDriveMembersOnlyRestriction = QStringLiteral("canChangeDriveMembersOnlyRestriction");
const QString Drives::Capabilities::Fields::CanComment = QStringLiteral("canComment");
const QString Drives::Capabilities::Fields::CanCopy = QStringLiteral("canCopy");
const QString Drives::Capabilities::Fields::CanDeleteChildren = QStringLiteral("canDeleteChildren");
const QString Drives::Capabilities::Fields::CanDeleteDrive = QStringLiteral("canDeleteDrive");
const QString Drives::Capabilities::Fields::CanDownload = QStringLiteral("canDownload");
const QString Drives::Capabilities::Fields::CanEdit = QStringLiteral("canEdit");
const QString Drives::Capabilities::Fields::CanListChildren = QStringLiteral("canListChildren");
const QString Drives::Capabilities::Fields::CanManageMembers = QStringLiteral("canManageMembers");
const QString Drives::Capabilities::Fields::CanReadRevisions = QStringLiteral("canReadRevisions");
const QString Drives::Capabilities::Fields::CanRename = QStringLiteral("canRename");
const QString Drives::Capabilities::Fields::CanRenameDrive = QStringLiteral("canRenameDrive");
const QString Drives::Capabilities::Fields::CanShare = QStringLiteral("canShare");
const QString Drives::Capabilities::Fields::CanTrashChildren = QStringLiteral("canTrashChildren");

const QString Drives::BackgroundImageFile::Fields::Id = QStringLiteral("id");
const QString Drives::BackgroundImageFile::Fields::XCoordinate = QStringLiteral("xCoordinate");
const QString Drives::BackgroundImageFile::Fields::YCoordinate = QStringLiteral("yCoordinate");
const QString Drives::BackgroundImageFile::Fields::Width = QStringLiteral("width");

const QString Drives::Fields::Items = QStringLiteral("items");
const QString Drives::Fields::KindDrive = QStringLiteral("kind");
const QString Drives::Fields::PageToken = QStringLiteral("pageToken");
const QString Drives::Fields::NextPageToken = QStringLiteral("nextPageToken");
const QString Drives::Fields::Id = QStringLiteral("id");
const QString Drives::Fields::Kind = QStringLiteral("kind");
const QString Drives::Fields::Name = QStringLiteral("name");
const QString Drives::Fields::ThemeId = QStringLiteral("themeId");
const QString Drives::Fields::ColorRgb = QStringLiteral("colorRgb");
const QString Drives::Fields::BackgroundImageFile = QStringLiteral("backgroundImageFile");
const QString Drives::Fields::BackgroundImageLink = QStringLiteral("backgroundImageLink");
const QString Drives::Fields::Capabilities = QStringLiteral("capabilities");
const QString Drives::Fields::CreatedDate = QStringLiteral("createdDate");
const QString Drives::Fields::Hidden = QStringLiteral("hidden");
const QString Drives::Fields::Restrictions = QStringLiteral("restrictions");

Drives::Drives():
    KGAPI2::Object(),
    d(new Private)
{
}

Drives::Drives(const Drives& other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

Drives::~Drives() = default;

bool Drives::operator==(const Drives &other) const
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
    GAPI_COMPARE(hidden);
    GAPI_COMPARE_SHAREDPTRS(restrictions);
    return true;
}

QString Drives::id() const
{
    return d->id;
}

void Drives::setId(const QString &id) const
{
    d->id = id;
}

QString Drives::name() const
{
    return d->name;
}

void Drives::setName(const QString &name) const
{
    d->name = name;
}

QString Drives::themeId() const
{
    return d->themeId;
}

void Drives::setThemeId(const QString &themeId) const
{
    d->themeId = themeId;
}

QString Drives::colorRgb() const
{
    return d->colorRgb;
}

void Drives::setColorRgb(const QString &colorRgb) const
{
    d->colorRgb = colorRgb;
}

Drives::BackgroundImageFilePtr Drives::backgroundImageFile() const
{
    return d->backgroundImageFile;
}

void Drives::setBackgroundImageFile(const Drives::BackgroundImageFilePtr &backgroundImageFile) const
{
    d->backgroundImageFile = backgroundImageFile;
}

QString Drives::backgroundImageLink() const
{
    return d->backgroundImageLink;
}

Drives::CapabilitiesPtr Drives::capabilities() const
{
    return d->capabilities;
}

QDateTime Drives::createdDate() const
{
    return d->createdDate;
}

bool Drives::hidden() const
{
    return d->hidden;
}

void Drives::setRestrictions(const Drives::RestrictionsPtr &restrictions) const
{
    d->restrictions = restrictions;
}

Drives::RestrictionsPtr Drives::restrictions() const
{
    return d->restrictions;
}

DrivesPtr Drives::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return DrivesPtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

DrivesList Drives::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return DrivesList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(Drives::Fields::Kind) ||
            map[Drives::Fields::Kind].toString() != ApiKindList) {
        return DrivesList();
    }

    if (map.contains(Drives::Fields::NextPageToken)) {
        feedData.nextPageUrl = DriveService::fetchDrivesUrl();
        QUrlQuery query(feedData.nextPageUrl);
        query.addQueryItem(Drives::Fields::PageToken, map.value(Drives::Fields::NextPageToken).toString());
        feedData.nextPageUrl.setQuery(query);
    }

    DrivesList list;
    const QVariantList items = map[Drives::Fields::Items].toList();
    for (const QVariant & item : items) {
        const DrivesPtr drives = Private::fromJSON(item.toMap());

        if (!drives.isNull()) {
            list << drives;
        }
    }

    return list;
}

QByteArray Drives::toJSON(const DrivesPtr &drives)
{
    QVariantMap drivesMap;
    drivesMap[Drives::Fields::Kind] = ApiKind;
    if (!drives->id().isEmpty()) {
        drivesMap[Drives::Fields::Id] = drives->id();
    }
    if (!drives->name().isEmpty()) {
        drivesMap[Drives::Fields::Name] = drives->name();
    }
    if (!drives->themeId().isEmpty()) {
        drivesMap[Drives::Fields::ThemeId] = drives->themeId();
    }
    if (!drives->colorRgb().isEmpty()) {
        drivesMap[Drives::Fields::ColorRgb] = drives->colorRgb();
    }
    if (!drives->backgroundImageLink().isEmpty()) {
        drivesMap[Drives::Fields::BackgroundImageLink] = drives->backgroundImageLink();
    }
    if (drives->createdDate().isValid()) {
        drivesMap[Drives::Fields::CreatedDate] = drives->createdDate();
    }
    drivesMap[Drives::Fields::Hidden] = drives->hidden();

    if (drives->restrictions()) {
        QVariantMap restrictionsMap;
        restrictionsMap[Drives::Restrictions::Fields::AdminManagedRestrictions] = drives->restrictions()->adminManagedRestrictions();
        restrictionsMap[Drives::Restrictions::Fields::CopyRequiresWriterPermission] = drives->restrictions()->copyRequiresWriterPermission();
        restrictionsMap[Drives::Restrictions::Fields::DomainUsersOnly] = drives->restrictions()->domainUsersOnly();
        restrictionsMap[Drives::Restrictions::Fields::DriveMembersOnly] = drives->restrictions()->driveMembersOnly();
        drivesMap[Drives::Fields::Restrictions] = restrictionsMap;
    }

    if (drives->backgroundImageFile()) {
        QVariantMap backgroundImageFileMap;
        backgroundImageFileMap[Drives::BackgroundImageFile::Fields::Id] = drives->backgroundImageFile()->id();
        backgroundImageFileMap[Drives::BackgroundImageFile::Fields::XCoordinate] = drives->backgroundImageFile()->xCoordinate();
        backgroundImageFileMap[Drives::BackgroundImageFile::Fields::YCoordinate] = drives->backgroundImageFile()->yCoordinate();
        backgroundImageFileMap[Drives::BackgroundImageFile::Fields::Width] = drives->backgroundImageFile()->width();
        drivesMap[Drives::Fields::BackgroundImageFile] = backgroundImageFileMap;
    }

    if (drives->capabilities()) {
        QVariantMap capabilitiesMap;
        capabilitiesMap[Drives::Capabilities::Fields::CanAddChildren] = drives->capabilities()->canAddChildren();
        capabilitiesMap[Drives::Capabilities::Fields::CanChangeCopyRequiresWriterPermissionRestriction] = drives->capabilities()->canChangeCopyRequiresWriterPermissionRestriction();
        capabilitiesMap[Drives::Capabilities::Fields::CanChangeDomainUsersOnlyRestriction] = drives->capabilities()->canChangeDomainUsersOnlyRestriction();
        capabilitiesMap[Drives::Capabilities::Fields::CanChangeDriveBackground] = drives->capabilities()->canChangeDriveBackground();
        capabilitiesMap[Drives::Capabilities::Fields::CanChangeDriveMembersOnlyRestriction] = drives->capabilities()->canChangeDriveMembersOnlyRestriction();
        capabilitiesMap[Drives::Capabilities::Fields::CanComment] = drives->capabilities()->canComment();
        capabilitiesMap[Drives::Capabilities::Fields::CanCopy] = drives->capabilities()->canCopy();
        capabilitiesMap[Drives::Capabilities::Fields::CanDeleteChildren] = drives->capabilities()->canDeleteChildren();
        capabilitiesMap[Drives::Capabilities::Fields::CanDeleteDrive] = drives->capabilities()->canDeleteDrive();
        capabilitiesMap[Drives::Capabilities::Fields::CanDownload] = drives->capabilities()->canDownload();
        capabilitiesMap[Drives::Capabilities::Fields::CanEdit] = drives->capabilities()->canEdit();
        capabilitiesMap[Drives::Capabilities::Fields::CanListChildren] = drives->capabilities()->canListChildren();
        capabilitiesMap[Drives::Capabilities::Fields::CanManageMembers] = drives->capabilities()->canManageMembers();
        capabilitiesMap[Drives::Capabilities::Fields::CanReadRevisions] = drives->capabilities()->canReadRevisions();
        capabilitiesMap[Drives::Capabilities::Fields::CanRename] = drives->capabilities()->canRename();
        capabilitiesMap[Drives::Capabilities::Fields::CanRenameDrive] = drives->capabilities()->canRenameDrive();
        capabilitiesMap[Drives::Capabilities::Fields::CanShare] = drives->capabilities()->canShare();
        capabilitiesMap[Drives::Capabilities::Fields::CanTrashChildren] = drives->capabilities()->canTrashChildren();
        drivesMap[Drives::Fields::Capabilities] = capabilitiesMap;
    }

    QJsonDocument document = QJsonDocument::fromVariant(drivesMap);
    return document.toJson(QJsonDocument::Compact);
}
