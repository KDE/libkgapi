/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "filecopyjob.h"
#include "account.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileCopyJob::Private
{
public:
    Private(FileCopyJob *parent);
    void processNext();

    QMap<QString, FilePtr> files;

    QList<FilePtr> copies;

private:
    FileCopyJob *const q;
};

FileCopyJob::Private::Private(FileCopyJob *parent)
    : q(parent)
{
}

void FileCopyJob::Private::processNext()
{
    if (files.isEmpty()) {
        q->emitFinished();
        return;
    }

    const QString fileId = files.cbegin().key();
    const FilePtr file = files.take(fileId);

    QUrl url = DriveService::copyFileUrl(fileId);
    q->updateUrl(url);

    QNetworkRequest request(url);

    const QByteArray rawData = File::toJSON(file);

    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

FileCopyJob::FileCopyJob(const QString &sourceFileId, const FilePtr &destinationFile, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    d->files.insert(sourceFileId, destinationFile);
}

FileCopyJob::FileCopyJob(const FilePtr &sourceFile, const FilePtr &destinationFile, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    d->files.insert(sourceFile->id(), destinationFile);
}

FileCopyJob::FileCopyJob(const QMap<QString, FilePtr> &files, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    d->files = files;
}

FileCopyJob::FileCopyJob(const QMap<FilePtr, FilePtr> &files, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    QMap<FilePtr, FilePtr>::ConstIterator iter = files.constBegin();
    QMap<FilePtr, FilePtr>::ConstIterator iterEnd = files.constEnd();
    for (; iter != iterEnd; ++iter) {
        d->files.insert(iter.key()->id(), iter.value());
    }
}

FileCopyJob::~FileCopyJob()
{
    delete d;
}

FilesList FileCopyJob::files() const
{
    return d->copies;
}

void FileCopyJob::start()
{
    d->processNext();
}

void FileCopyJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    QNetworkRequest r = request;
    r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

    accessManager->post(r, data);
}

void FileCopyJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        d->copies << File::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return;
    }

    // Enqueue next item or finish
    d->processNext();
}
