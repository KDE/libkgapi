/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fetchjob.h"
#include "debug.h"
#include "object.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN FetchJob::Private
{
public:
    ObjectsList items;
};

FetchJob::FetchJob(QObject *parent)
    : Job(parent)
    , d(new Private)
{
}

FetchJob::FetchJob(const AccountPtr &account, QObject *parent)
    : Job(account, parent)
    , d(new Private)
{
}

FetchJob::~FetchJob()
{
    delete d;
}

ObjectsList FetchJob::items() const
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called items() on a running job, returning empty list.";
        return ObjectsList();
    }

    return d->items;
}

void FetchJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    Q_UNUSED(data)
    Q_UNUSED(contentType)

    accessManager->get(request);
}

void FetchJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    d->items << handleReplyWithItems(reply, rawData);
}

void FetchJob::aboutToStart()
{
    d->items.clear();

    Job::aboutToStart();
}

ObjectsList FetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}

#include "moc_fetchjob.cpp"
