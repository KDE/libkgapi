/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "permissioncreatejob.h"
#include "account.h"
#include "driveservice.h"
#include "permission.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

namespace {
    static constexpr bool sendNotificationEmailsDefault = true;
    static constexpr bool useDomainAdminAccessDefault = false;
}

class Q_DECL_HIDDEN PermissionCreateJob::Private
{
  public:
    Private(PermissionCreateJob *parent);
    void processNext();

    PermissionsList permissions;
    QString fileId;
    QString emailMessage;
    bool sendNotificationEmails;
    bool supportsAllDrives;
    bool useDomainAdminAccess;

  private:
    PermissionCreateJob *const q;
};

PermissionCreateJob::Private::Private(PermissionCreateJob *parent):
    sendNotificationEmails(sendNotificationEmailsDefault),
    supportsAllDrives(true),
    useDomainAdminAccess(useDomainAdminAccessDefault),
    q(parent)
{
}

void PermissionCreateJob::Private::processNext()
{
    if (permissions.isEmpty()) {
        q->emitFinished();
        return;
    }

    const PermissionPtr permission = permissions.takeFirst();

    QUrl url = DriveService::createPermissionUrl(fileId);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(supportsAllDrives));

    if (sendNotificationEmails != sendNotificationEmailsDefault) {
        query.addQueryItem(QStringLiteral("sendNotificationEmails"), Utils::bool2Str(sendNotificationEmails));
    }

    if (!emailMessage.isEmpty()) {
        query.addQueryItem(QStringLiteral("emailMessage"), emailMessage);
    }

    if (useDomainAdminAccess != useDomainAdminAccessDefault) {
        query.addQueryItem(QStringLiteral("useDomainAdminAccess"), Utils::bool2Str(useDomainAdminAccess));
    }

    url.setQuery(query);
    QNetworkRequest request(url);

    const QByteArray rawData = Permission::toJSON(permission);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

PermissionCreateJob::PermissionCreateJob(const QString &fileId,
                                         const PermissionPtr &permission,
                                         const AccountPtr &account,
                                         QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->permissions << permission;
}

PermissionCreateJob::PermissionCreateJob(const QString &fileId,
                                         const PermissionsList &permissions,
                                         const AccountPtr &account,
                                         QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->permissions = permissions;
}

PermissionCreateJob::~PermissionCreateJob() = default;

QString PermissionCreateJob::emailMessage() const
{
    return d->emailMessage;
}

void PermissionCreateJob::setEmailMessage(const QString &emailMessage)
{
    d->emailMessage = emailMessage;
}

bool PermissionCreateJob::sendNotificationEmails() const
{
    return d->sendNotificationEmails;
}

void PermissionCreateJob::setSendNotificationEmails(bool sendNotificationEmails)
{
    d->sendNotificationEmails = sendNotificationEmails;
}

bool PermissionCreateJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void PermissionCreateJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

bool PermissionCreateJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess;
}

void PermissionCreateJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    d->useDomainAdminAccess = useDomainAdminAccess;
}

void PermissionCreateJob::start()
{
    d->processNext();
}

ObjectsList PermissionCreateJob::handleReplyWithItems(const QNetworkReply *reply,
                                                      const QByteArray &rawData)
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


