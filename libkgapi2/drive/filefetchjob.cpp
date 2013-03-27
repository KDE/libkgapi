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

#include "filefetchjob.h"
#include "account.h"
#include "debug.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class DriveFileFetchJob::Private
{
  public:
    Private(DriveFileFetchJob *parent);
    void processNext();
    QNetworkRequest createRequest(const QUrl &url);

    QStringList filesIDs;
    bool isFeed;

    bool updateViewedDate;

  private:
    DriveFileFetchJob * const q;
};

DriveFileFetchJob::Private::Private(DriveFileFetchJob *parent):
    isFeed(false),
    updateViewedDate(false),
    q(parent)
{
}

QNetworkRequest DriveFileFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    return request;
}

void DriveFileFetchJob::Private::processNext()
{
    QUrl url;

    if (isFeed) {
        url = DriveService::fetchFilesUrl();
    } else {
        if (filesIDs.isEmpty()) {
            q->emitFinished();
            return;
        }

        const QString fileId = filesIDs.takeFirst();
        url = DriveService::fetchFileUrl(fileId);
    }

    q->enqueueRequest(createRequest(url));
}

DriveFileFetchJob::DriveFileFetchJob(const QString &fileId,
                                     const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->filesIDs << fileId;
}

DriveFileFetchJob::DriveFileFetchJob(const QStringList &filesIds,
                                     const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->filesIDs << filesIds;
}

DriveFileFetchJob::DriveFileFetchJob(const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->isFeed = true;
}

DriveFileFetchJob::~DriveFileFetchJob()
{
    delete d;
}

bool DriveFileFetchJob::updateViewedDate() const
{
    return d->updateViewedDate;
}

void DriveFileFetchJob::setUpdateViewedDate(bool updateViewedDate)
{
    if (isRunning()) {
        kWarning() << "Can't modify updateViewedDate property when job is running.";
        return;
    }

    d->updateViewedDate = updateViewedDate;
}

void DriveFileFetchJob::start()
{
    d->processNext();
}

ObjectsList DriveFileFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                                    const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->isFeed) {
            FeedData feedData;

            items << DriveFile::fromJSONFeed(rawData, feedData);

            if (feedData.nextPageUrl.isValid()) {
                const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
                enqueueRequest(request);
            }

        } else {
            items << DriveFile::fromJSON(rawData);

            d->processNext();
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    return items;
}

#include "filefetchjob.moc"
