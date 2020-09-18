/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "parentreferencefetchjob.h"
#include "account.h"
#include "driveservice.h"
#include "parentreference.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ParentReferenceFetchJob::Private
{
  public:
    QString fileId;
    QString referenceId;
};

ParentReferenceFetchJob::ParentReferenceFetchJob(const QString &fileId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
                                                 FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
}

ParentReferenceFetchJob::ParentReferenceFetchJob(const QString &fileId,
                                                 const QString &referenceId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->referenceId = referenceId;
}

ParentReferenceFetchJob::~ParentReferenceFetchJob()
{
    delete d;
}

void ParentReferenceFetchJob::start()
{
    QUrl url;
    if (d->referenceId.isEmpty()) {
        url = DriveService::fetchParentReferencesUrl(d->fileId);
    } else {
        url = DriveService::fetchParentReferenceUrl(d->fileId, d->referenceId);
    }
    QNetworkRequest request(url);

    enqueueRequest(request);
}

ObjectsList ParentReferenceFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                                          const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->referenceId.isEmpty()) {
            items << ParentReference::fromJSONFeed(rawData);
        } else {
            items << ParentReference::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitResult();
    return items;
}


