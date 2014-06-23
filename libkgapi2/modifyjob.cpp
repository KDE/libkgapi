/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "modifyjob.h"
#include "debug.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QBuffer>

using namespace KGAPI2;

class ModifyJob::Private
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



#include "modifyjob.moc"
