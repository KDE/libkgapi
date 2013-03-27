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
#include "filecopyjob.h"
#include "account.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class DriveFileCopyJob::Private
{
  public:
    Private(DriveFileCopyJob *parent);
    void processNext();

    QMap<QString, DriveFilePtr > files;

    QList<DriveFilePtr> copies;

  private:
    DriveFileCopyJob * const q;
};

DriveFileCopyJob::Private::Private(DriveFileCopyJob *parent):
    q(parent)
{
}

void DriveFileCopyJob::Private::processNext()
{
    if (files.isEmpty()) {
        q->emitFinished();
        return;
    }

    const QString fileId = files.keys().first();
    const DriveFilePtr file = files.take(fileId);

    QUrl url = DriveService::copyFileUrl(fileId);
    q->updateUrl(url);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    const QByteArray rawData = DriveFile::toJSON(file);

    q->enqueueRequest(request, rawData, QLatin1String("application/json"));
}

DriveFileCopyJob::DriveFileCopyJob(const QString &sourceFileId,
                                   const DriveFilePtr &destinationFile,
                                   const AccountPtr &account,
                                   QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(sourceFileId, destinationFile);
}

DriveFileCopyJob::DriveFileCopyJob(const DriveFilePtr &sourceFile,
                                   const DriveFilePtr &destinationFile,
                                   const AccountPtr &account,
                                   QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(sourceFile->id(), destinationFile);
}

DriveFileCopyJob::DriveFileCopyJob(const QMap< QString, DriveFilePtr > &files,
                                   const AccountPtr &account,
                                   QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files = files;
}

DriveFileCopyJob::DriveFileCopyJob(const QMap< DriveFilePtr, DriveFilePtr > &files,
                                   const AccountPtr &account,
                                   QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    QMap<DriveFilePtr, DriveFilePtr>::ConstIterator iter = files.constBegin();
    for ( ; iter != files.constEnd(); ++iter) {
        d->files.insert(iter.key()->id(), iter.value());
    }
}

DriveFileCopyJob::~DriveFileCopyJob()
{
    delete d;
}

DriveFilesList DriveFileCopyJob::files() const
{
    return d->copies;
}

void DriveFileCopyJob::start()
{
    d->processNext();
}

void DriveFileCopyJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                       const QNetworkRequest &request,
                                       const QByteArray &data,
                                       const QString &contentType)
{
    QNetworkRequest r = request;
    r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

    accessManager->post(r, data);
}


void DriveFileCopyJob::handleReply(const QNetworkReply *reply,
                                   const QByteArray &rawData)
{
   const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        d->copies << DriveFile::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return;
    }

    // Enqueue next item or finish
    d->processNext();
}

#include "filecopyjob.moc"
