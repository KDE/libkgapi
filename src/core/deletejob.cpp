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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "deletejob.h"

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

    accessManager->deleteResource(r);
}


void DeleteJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    start();
}
