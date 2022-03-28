/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "childreferencecreatejob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChildReferenceCreateJob::Private
{
public:
    Private(ChildReferenceCreateJob *parent);
    void processNext();

    QString folderId;
    ChildReferencesList references;
    bool supportsAllDrives = true;

private:
    ChildReferenceCreateJob *const q;
};

ChildReferenceCreateJob::Private::Private(ChildReferenceCreateJob *parent)
    : q(parent)
{
}

void ChildReferenceCreateJob::Private::processNext()
{
    if (references.isEmpty()) {
        q->emitFinished();
        return;
    }

    const ChildReferencePtr reference = references.takeFirst();
    QUrl url = DriveService::createChildReference(folderId);

    QUrlQuery withDriveSupportQuery(url);
    withDriveSupportQuery.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(supportsAllDrives));
    url.setQuery(withDriveSupportQuery);

    QNetworkRequest request(url);

    const QByteArray rawData = ChildReference::toJSON(reference);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId, const QString &childId, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->folderId = folderId;
    d->references << ChildReferencePtr(new ChildReference(childId));
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId, const QStringList &childrenIds, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->folderId = folderId;
    for (const QString &childId : std::as_const(childrenIds)) {
        d->references << ChildReferencePtr(new ChildReference(childId));
    }
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId, const ChildReferencePtr &reference, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->folderId = folderId;
    d->references << reference;
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId, const ChildReferencesList &references, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->folderId = folderId;
    d->references << references;
}

ChildReferenceCreateJob::~ChildReferenceCreateJob()
{
    delete d;
}

bool ChildReferenceCreateJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void ChildReferenceCreateJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

void ChildReferenceCreateJob::start()
{
    d->processNext();
}

ObjectsList ChildReferenceCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << ChildReference::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}
