/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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


