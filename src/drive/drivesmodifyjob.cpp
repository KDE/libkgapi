/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2019  David Barchiesi <david@barchie.si>
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


#include "drivesmodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "drives.h"
#include "utils.h"
#include "../debug.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>


namespace {
    static const QString UseDomainAdminAccessAttr = QStringLiteral("useDomainAdminAccess");
}
using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN DrivesModifyJob::Private
{
  public:
    Private(DrivesModifyJob *parent);
    void processNext();

    bool useDomainAdminAccess = false;

    DrivesList drives;

  private:
    DrivesModifyJob *const q;
};

DrivesModifyJob::Private::Private(DrivesModifyJob *parent):
    q(parent)
{
}

void DrivesModifyJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify useDomainAdminAccess property when job is running";
        return;
    }

    d->useDomainAdminAccess = useDomainAdminAccess;
}

bool DrivesModifyJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess;
}

void DrivesModifyJob::Private::processNext()
{
    if (drives.isEmpty()) {
        q->emitFinished();
        return;
    }

    const DrivesPtr drive = drives.takeFirst();
    QUrl url = DriveService::fetchDrivesUrl(drive->id());

    QUrlQuery query(url);
    if (useDomainAdminAccess != false) {
        query.addQueryItem(UseDomainAdminAccessAttr, Utils::bool2Str(useDomainAdminAccess));
    }
    url.setQuery(query);

    QNetworkRequest request(url);

    const QByteArray rawData = Drives::toJSON(drive);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

DrivesModifyJob::DrivesModifyJob(const DrivesPtr &drive,
                                         const AccountPtr &account,
                                         QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->drives << drive;
}

DrivesModifyJob::DrivesModifyJob(const DrivesList &drives,
                                         const AccountPtr &account,
                                         QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->drives << drives;
}

DrivesModifyJob::~DrivesModifyJob() = default;

void DrivesModifyJob::start()
{
    d->processNext();
}

ObjectsList DrivesModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                        const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << Drives::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}


