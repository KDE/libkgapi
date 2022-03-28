/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "parentreferencecreatejob.h"
#include "account.h"
#include "driveservice.h"
#include "parentreference.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ParentReferenceCreateJob::Private
{
public:
    Private(ParentReferenceCreateJob *parent);
    void processNext();
    bool supportsAllDrives = true;

    QString fileId;
    ParentReferencesList references;

private:
    ParentReferenceCreateJob *const q;
};

ParentReferenceCreateJob::Private::Private(ParentReferenceCreateJob *parent)
    : q(parent)
{
}

void ParentReferenceCreateJob::Private::processNext()
{
    if (references.isEmpty()) {
        q->emitFinished();
        return;
    }

    const ParentReferencePtr reference = references.takeFirst();
    QUrl url = DriveService::createParentReferenceUrl(fileId);

    QUrlQuery withDriveSupportQuery(url);
    withDriveSupportQuery.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(supportsAllDrives));
    url.setQuery(withDriveSupportQuery);

    QNetworkRequest request(url);

    const QByteArray rawData = ParentReference::toJSON(reference);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId, const QString &parentId, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->fileId = fileId;
    d->references << ParentReferencePtr(new ParentReference(parentId));
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId, const QStringList &parentsIds, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->fileId = fileId;
    for (const QString &parentId : std::as_const(parentsIds)) {
        d->references << ParentReferencePtr(new ParentReference(parentId));
    }
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId, const ParentReferencePtr &reference, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->fileId = fileId;
    d->references << reference;
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId, const ParentReferencesList &references, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->fileId = fileId;
    d->references << references;
}

ParentReferenceCreateJob::~ParentReferenceCreateJob()
{
    delete d;
}

bool ParentReferenceCreateJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void ParentReferenceCreateJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

void ParentReferenceCreateJob::start()
{
    d->processNext();
}

ObjectsList ParentReferenceCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << ParentReference::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}
