/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "createjob.h"
#include "object.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN CreateJob::Private
{
public:
    ObjectsList items;
};

CreateJob::CreateJob(QObject *parent)
    : Job(parent)
    , d(new Private)
{
}

CreateJob::CreateJob(const AccountPtr &account, QObject *parent)
    : Job(account, parent)
    , d(new Private)
{
}

CreateJob::~CreateJob()
{
    delete d;
}

ObjectsList CreateJob::items() const
{
    return d->items;
}

void CreateJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    QNetworkRequest r = request;
    if (!r.hasRawHeader("Content-Type")) {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    accessManager->post(r, data);
}

void CreateJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    d->items << handleReplyWithItems(reply, rawData);
}

void CreateJob::aboutToStart()
{
    d->items.clear();

    Job::aboutToStart();
}

ObjectsList CreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}

#include "moc_createjob.cpp"
