/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "revisionmodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "revision.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN RevisionModifyJob::Private
{
  public:
    Private(RevisionModifyJob *parent);
    void processNext();

    QString fileId;
    RevisionsList revisions;

  private:
    RevisionModifyJob *const q;
};

RevisionModifyJob::Private::Private(RevisionModifyJob *parent):
    q(parent)
{
}

void RevisionModifyJob::Private::processNext()
{
    if (revisions.isEmpty()) {
        q->emitFinished();
        return;
    }

    const RevisionPtr revision = revisions.takeFirst();
    const QUrl url = DriveService::modifyRevisionUrl(fileId, revision->id());

    QNetworkRequest request(url);

    const QByteArray rawData = Revision::toJSON(revision);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

RevisionModifyJob::RevisionModifyJob(const QString &fileId,
                                     const RevisionPtr &revision,
                                     const AccountPtr &account,
                                     QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->revisions << revision;
}

RevisionModifyJob::RevisionModifyJob(const QString &fileId,
                                     const RevisionsList &revisions,
                                     const AccountPtr &account, QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->revisions << revisions;
}

RevisionModifyJob::~RevisionModifyJob()
{
    delete d;
}

void RevisionModifyJob::start()
{
    d->processNext();
}

ObjectsList RevisionModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                    const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << Revision::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}



