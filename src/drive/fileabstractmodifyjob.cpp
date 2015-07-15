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

#include "fileabstractmodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class FileAbstractModifyJob::Private
{
  public:
    Private(FileAbstractModifyJob *parent);
    void processNext();

    QStringList filesIds;

  private:
    FileAbstractModifyJob *q;
};

FileAbstractModifyJob::Private::Private(FileAbstractModifyJob *parent):
    q(parent)
{
}

void FileAbstractModifyJob::Private::processNext()
{
    if (filesIds.isEmpty()) {
        q->emitFinished();
        return;
    }

    const QString fileId = filesIds.takeFirst();
    const QUrl url = q->url(fileId);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
    request.setUrl(url);

    q->enqueueRequest(request);
}

FileAbstractModifyJob::FileAbstractModifyJob(const QString &fileId,
                                             const AccountPtr &account,
                                             QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->filesIds << fileId;
}

FileAbstractModifyJob::FileAbstractModifyJob(const QStringList &filesIds,
                                             const AccountPtr &account,
                                             QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->filesIds << filesIds;
}

FileAbstractModifyJob::FileAbstractModifyJob(const FilePtr &file,
                                             const AccountPtr &account,
                                             QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->filesIds << file->id();
}

FileAbstractModifyJob::FileAbstractModifyJob(const FilesList &files,
                                             const AccountPtr &account,
                                             QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH(const FilePtr & file, files) {
        d->filesIds << file->id();
    }
}

FileAbstractModifyJob::~FileAbstractModifyJob()
{
    delete d;
}

void FileAbstractModifyJob::start()
{
    d->processNext();
}

ObjectsList FileAbstractModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                        const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << File::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    d->processNext();

    return items;
}

#include "moc_fileabstractmodifyjob.cpp"
