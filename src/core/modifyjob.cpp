/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modifyjob.h"
#include "../debug.h"
#include "object.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QBuffer>

using namespace KGAPI2;

class Q_DECL_HIDDEN ModifyJob::Private
{
  public:
    ObjectsList items;
    QBuffer buffer;
};

ModifyJob::ModifyJob(QObject* parent):
    Job(parent),
    d(new Private)
{
}

ModifyJob::ModifyJob(const AccountPtr& account, QObject* parent):
    Job(account, parent),
    d(new Private)
{
}

ModifyJob::~ModifyJob()
{
    delete d;
}

ObjectsList ModifyJob::items()
{
    return d->items;
}

void ModifyJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request,
                                const QByteArray& data, const QString& contentType)
{
    QNetworkRequest r = request;
    if (!r.hasRawHeader("Content-Type")) {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    if (!r.hasRawHeader("If-Match")) {
        r.setRawHeader("If-Match", "*");
    }

    // Note: there is a problem with PUT when using KIO::AccessManager - it does
    // not transfer the body correctly.
    // Using sendCustomRequest() works just fine.
    //accessManager->put(r, data);
    if (data.size() > 0) {
        r.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

        d->buffer.close();
        d->buffer.setData(data);
        d->buffer.open(QIODevice::ReadOnly);
        accessManager->sendCustomRequest(r, "PUT", &d->buffer);
    } else {
        accessManager->sendCustomRequest(r, "PUT");
    }
}

void ModifyJob::handleReply( const QNetworkReply *reply, const QByteArray &rawData )
{
    d->buffer.close();
    d->items << handleReplyWithItems(reply, rawData);
}

void ModifyJob::aboutToStart()
{
    d->items.clear();

    Job::aboutToStart();
}

ObjectsList ModifyJob::handleReplyWithItems(const QNetworkReply* reply,
                                            const QByteArray& rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}
