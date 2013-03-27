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

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class DriveFileAbstractModifyJob::Private
{
  public:
    Private(DriveFileAbstractModifyJob *parent);
    void processNext();

    QStringList filesIds;

  private:
    DriveFileAbstractModifyJob *q;
};

DriveFileAbstractModifyJob::Private::Private(DriveFileAbstractModifyJob *parent):
    q(parent)
{
}

void DriveFileAbstractModifyJob::Private::processNext()
{
    if (filesIds.isEmpty()) {
        q->emitFinished();
        return;
    }

    const QString fileId = filesIds.takeFirst();
    const QUrl url = q->url(fileId);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    q->enqueueRequest(request);
}

DriveFileAbstractModifyJob::DriveFileAbstractModifyJob(const QString &fileId,
                                     const AccountPtr &account,
                                     QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->filesIds << fileId;
}

DriveFileAbstractModifyJob::DriveFileAbstractModifyJob(const QStringList &filesIds,
                                     const AccountPtr &account,
                                     QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->filesIds << filesIds;
}

DriveFileAbstractModifyJob::DriveFileAbstractModifyJob(const DriveFilePtr &file,
                                     const AccountPtr &account,
                                     QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->filesIds << file->id();
}

DriveFileAbstractModifyJob::DriveFileAbstractModifyJob(const DriveFilesList &files,
                                     const AccountPtr &account,
                                     QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH (const DriveFilePtr &file, files) {
        d->filesIds << file->id();
    }
}

DriveFileAbstractModifyJob::~DriveFileAbstractModifyJob()
{
    delete d;
}

void DriveFileAbstractModifyJob::start()
{
    d->processNext();
}

KGAPI2::ObjectsList DriveFileAbstractModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                            const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << DriveFile::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    d->processNext();

    return items;
}

#include "fileabstractmodifyjob.moc"
