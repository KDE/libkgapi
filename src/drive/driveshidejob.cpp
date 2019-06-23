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
        q->emitFinished();
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
        emitFinished();
        return items;
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}


