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

#include "fetchjob.h"
#include "../debug.h"
#include "object.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN FetchJob::Private
{
  public:
    ObjectsList items;
};

FetchJob::FetchJob(QObject* parent):
    Job(parent),
    d(new Private)
{
}

FetchJob::FetchJob(const AccountPtr& account, QObject* parent):
    Job(account, parent),
    d(new Private)
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

void FetchJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request,
                               const QByteArray& data, const QString& contentType)
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

ObjectsList FetchJob::handleReplyWithItems(const QNetworkReply* reply, const QByteArray& rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}
