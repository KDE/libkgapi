/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "deletejob.h"

#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN DeleteJob::Private
{
};

DeleteJob::DeleteJob(QObject* parent):
    Job(parent),
    d(new Private)
{
}

DeleteJob::DeleteJob(const AccountPtr& account, QObject* parent):
    Job(account, parent),
    d(new Private)
{
}

DeleteJob::~DeleteJob()
{
    delete d;
}

void DeleteJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    Q_UNUSED(data)
    Q_UNUSED(contentType)

    QNetworkRequest r = request;
    if (!r.hasRawHeader("If-Match")) {
        r.setRawHeader("If-Match", "*");
    }

    // Delete requests have no response body so there isn't anything to pretty print
    QUrl cleanedUrl = r.url();
    QUrlQuery cleanedQuery(cleanedUrl);
    cleanedQuery.removeAllQueryItems(Job::StandardParams::PrettyPrint);
    cleanedUrl.setQuery(cleanedQuery);
    r.setUrl(cleanedUrl);

    accessManager->deleteResource(r);
}


void DeleteJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    start();
}
