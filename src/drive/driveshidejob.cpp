/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "driveshidejob.h"
#include "account.h"
#include "driveservice.h"
#include "drives.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN DrivesHideJob::Private
{
    public:
        Private(DrivesHideJob *parent);
        void processNext();

        bool hide = false;

        DrivesList drives;

    private:
        DrivesHideJob *const q;
};

DrivesHideJob::Private::Private(DrivesHideJob *parent):
    q(parent)
{
}

void DrivesHideJob::Private::processNext()
{
    if (drives.isEmpty()) {
        q->emitResult();
        return;
    }

    const DrivesPtr drive = drives.takeFirst();

    QUrl url = DriveService::hideDrivesUrl(drive->id(), hide);

    QNetworkRequest request(url);

    // A hide request doesn't have a body
    q->enqueueRequest(request, nullptr, QStringLiteral("application/json"));
}

DrivesHideJob::DrivesHideJob(const DrivesPtr &drive,
        bool hide,
        const AccountPtr &account,
        QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->drives << drive;
    d->hide = hide;
}

DrivesHideJob::DrivesHideJob(const DrivesList &drives,
        bool hide,
        const AccountPtr &account,
        QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->drives = drives;
    d->hide = hide;
}

DrivesHideJob::~DrivesHideJob() = default;

void DrivesHideJob::start()
{
    d->processNext();
}

ObjectsList DrivesHideJob::handleReplyWithItems(const QNetworkReply *reply,
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
        emitResult();
        return items;
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}


