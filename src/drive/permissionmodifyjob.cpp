/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "permissionmodifyjob.h"
#include "driveservice.h"
#include "permission.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

namespace
{
static constexpr bool removeExpirationDefault = false;
static constexpr bool transferOwnershipDefault = false;
static constexpr bool useDomainAdminAccessDefault = false;
}

class Q_DECL_HIDDEN PermissionModifyJob::Private
{
public:
    Private(PermissionModifyJob *parent);
    void processNext();

    QString fileId;
    PermissionsList permissions;
    bool supportsAllDrives;
    bool removeExpiration;
    bool transferOwnership;
    bool useDomainAdminAccess;

private:
    PermissionModifyJob *const q;
};

PermissionModifyJob::Private::Private(PermissionModifyJob *parent)
    : supportsAllDrives(true)
    , removeExpiration(removeExpirationDefault)
    , transferOwnership(transferOwnershipDefault)
    , useDomainAdminAccess(useDomainAdminAccessDefault)
    , q(parent)
{
}

void PermissionModifyJob::Private::processNext()
{
    if (permissions.isEmpty()) {
        q->emitFinished();
        return;
    }

    const PermissionPtr permission = permissions.takeFirst();
    QUrl url = DriveService::modifyPermissionUrl(fileId, permission->id());

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(supportsAllDrives));

    if (removeExpiration != removeExpirationDefault) {
        query.addQueryItem(QStringLiteral("removeExpiration"), Utils::bool2Str(removeExpiration));
    }

    if (transferOwnership != transferOwnershipDefault) {
        query.addQueryItem(QStringLiteral("transferOwnership"), Utils::bool2Str(transferOwnership));
    }

    if (useDomainAdminAccess != useDomainAdminAccessDefault) {
        query.addQueryItem(QStringLiteral("useDomainAdminAccess"), Utils::bool2Str(useDomainAdminAccess));
    }

    url.setQuery(query);
    QNetworkRequest request(url);

    const QByteArray rawData = Permission::toJSON(permission);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

PermissionModifyJob::PermissionModifyJob(const QString &fileId, const PermissionPtr &permission, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->fileId = fileId;
    d->permissions << permission;
}

PermissionModifyJob::PermissionModifyJob(const QString &fileId, const PermissionsList &permissions, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->fileId = fileId;
    d->permissions << permissions;
}

PermissionModifyJob::~PermissionModifyJob() = default;

bool PermissionModifyJob::removeExpiration() const
{
    return d->removeExpiration;
}

void PermissionModifyJob::setRemoveExpiration(bool removeExpiration)
{
    d->removeExpiration = removeExpiration;
}

bool PermissionModifyJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void PermissionModifyJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

bool PermissionModifyJob::transferOwnership() const
{
    return d->transferOwnership;
}

void PermissionModifyJob::setTransferOwnership(bool transferOwnership)
{
    d->transferOwnership = transferOwnership;
}

bool PermissionModifyJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess;
}

void PermissionModifyJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    d->useDomainAdminAccess = useDomainAdminAccess;
}

void PermissionModifyJob::start()
{
    d->processNext();
}

ObjectsList PermissionModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << Permission::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}
