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

#include "createjob.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class CreateJob::Private
{
  public:
    ObjectsList items;
};

CreateJob::CreateJob(QObject* parent):
    Job(parent),
    d(new Private)
{
}

CreateJob::CreateJob(const AccountPtr& account, QObject* parent):
    Job(account, parent),
    d(new Private)
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

void CreateJob::dispatchRequest(QNetworkAccessManager* accessManager,
                                const QNetworkRequest& request,
                                const QByteArray& data,
                                const QString& contentType)
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

ObjectsList CreateJob::handleReplyWithItems(const QNetworkReply* reply,
                                            const QByteArray& rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}

#include "createjob.moc"
