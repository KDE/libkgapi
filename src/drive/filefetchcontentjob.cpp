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


#include "filefetchcontentjob.h"
#include "account.h"
#include "file.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileFetchContentJob::Private
{
  public:
    Private(FileFetchContentJob *parent);

    void _k_downloadProgress(qint64 downloaded, qint64 total);

    QUrl url;
    QByteArray fileData;

  private:
    FileFetchContentJob * const q;
};

FileFetchContentJob::Private::Private(FileFetchContentJob *parent):
    q(parent)
{
}

void FileFetchContentJob::Private::_k_downloadProgress(qint64 downloaded, qint64 total)
{
    q->emitProgress(downloaded, total);
}

FileFetchContentJob::FileFetchContentJob(const FilePtr &file,
                                         const AccountPtr &account,
                                         QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->url = file->downloadUrl();
}

FileFetchContentJob::FileFetchContentJob(const QUrl &url,
                                         const AccountPtr &account,
                                         QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->url = url;
}

FileFetchContentJob::~FileFetchContentJob()
{
    delete d;
}

QByteArray FileFetchContentJob::data() const
{
    return d->fileData;
}

void FileFetchContentJob::start()
{
    QNetworkRequest request(d->url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

void FileFetchContentJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                          const QNetworkRequest &request,
                                          const QByteArray &data,
                                          const QString &contentType)
{
    Q_UNUSED(data)
    Q_UNUSED(contentType)

    QNetworkReply *reply = accessManager->get(request);
    connect(reply, &QNetworkReply::downloadProgress,
            this, [this](qint64 downloaded, qint64 total) { d->_k_downloadProgress(downloaded, total); });
}

void FileFetchContentJob::handleReply(const QNetworkReply *reply,
                                      const QByteArray &rawData)
{
    Q_UNUSED(reply)

    d->fileData = rawData;
}

ObjectsList FileFetchContentJob::handleReplyWithItems(const QNetworkReply *reply,
                                                      const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}

#include "moc_filefetchcontentjob.cpp"
