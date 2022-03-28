/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "modifyjob.h"
#include "debug.h"
#include "object.h"

#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN ModifyJob::Private
{
public:
    ObjectsList items;
    QBuffer buffer;
};

ModifyJob::ModifyJob(QObject *parent)
    : Job(parent)
    , d(new Private)
{
}

ModifyJob::ModifyJob(const AccountPtr &account, QObject *parent)
    : Job(account, parent)
    , d(new Private)
{
}

ModifyJob::~ModifyJob()
{
    delete d;
}

ObjectsList ModifyJob::items() const
{
    return d->items;
}

void ModifyJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    QNetworkRequest r = request;
    if (!r.hasRawHeader("Content-Type")) {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    if (!r.hasRawHeader("If-Match")) {
        r.setRawHeader("If-Match", "*");
    }

    // Note: there is a problem with PUT when using QNAM - it
    // doesn't transfer the body correctly.
    // Using sendCustomRequest() works just fine.
    // accessManager->put(r, data);
    if (!data.isEmpty()) {
        r.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

        d->buffer.close();
        d->buffer.setData(data);
        d->buffer.open(QIODevice::ReadOnly);
        accessManager->sendCustomRequest(r, "PUT", &d->buffer);
    } else {
        accessManager->sendCustomRequest(r, "PUT");
    }
}

void ModifyJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    d->buffer.close();
    d->items << handleReplyWithItems(reply, rawData);
}

void ModifyJob::aboutToStart()
{
    d->items.clear();

    Job::aboutToStart();
}

ObjectsList ModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}
