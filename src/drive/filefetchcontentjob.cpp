/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "filefetchcontentjob.h"
#include "file.h"

#include <QNetworkReply>
#include <QNetworkRequest>

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
    FileFetchContentJob *const q;
};

FileFetchContentJob::Private::Private(FileFetchContentJob *parent)
    : q(parent)
{
}

void FileFetchContentJob::Private::_k_downloadProgress(qint64 downloaded, qint64 total)
{
    q->emitProgress(downloaded, total);
}

FileFetchContentJob::FileFetchContentJob(const FilePtr &file, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->url = file->downloadUrl();
}

FileFetchContentJob::FileFetchContentJob(const QUrl &url, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
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
    connect(reply, &QNetworkReply::downloadProgress, this, [this](qint64 downloaded, qint64 total) {
        d->_k_downloadProgress(downloaded, total);
    });
}

void FileFetchContentJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)

    d->fileData = rawData;
}

ObjectsList FileFetchContentJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    return ObjectsList();
}

#include "moc_filefetchcontentjob.cpp"
