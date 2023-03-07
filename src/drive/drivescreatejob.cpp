/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "drivescreatejob.h"
#include "drives.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

namespace
{
static const QString RequestIdParam = QStringLiteral("requestId");
}
using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN DrivesCreateJob::Private
{
public:
    Private(DrivesCreateJob *parent);
    void processNext();

    DrivesList drives;
    QString requestId;

private:
    DrivesCreateJob *const q;
};

DrivesCreateJob::Private::Private(DrivesCreateJob *parent)
    : q(parent)
{
}

void DrivesCreateJob::Private::processNext()
{
    if (drives.isEmpty()) {
        q->emitFinished();
        return;
    }

    const DrivesPtr drive = drives.takeFirst();

    QUrl url = DriveService::fetchDrivesUrl();

    QUrlQuery query(url);
    if (!requestId.isEmpty()) {
        query.addQueryItem(RequestIdParam, requestId);
    }
    url.setQuery(query);

    QNetworkRequest request(url);

    const QByteArray rawData = Drives::toJSON(drive);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

DrivesCreateJob::DrivesCreateJob(const QString &requestId, const DrivesPtr &drive, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->requestId = requestId;
    d->drives << drive;
}

DrivesCreateJob::DrivesCreateJob(const QString &requestId, const DrivesList &drives, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->requestId = requestId;
    d->drives = drives;
}

DrivesCreateJob::~DrivesCreateJob() = default;

QString DrivesCreateJob::requestId() const
{
    return d->requestId;
}

void DrivesCreateJob::start()
{
    d->processNext();
}

ObjectsList DrivesCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
