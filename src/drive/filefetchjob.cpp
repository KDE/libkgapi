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
#include "filesearchquery.h"
#include "account.h"
#include "debug.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class FileFetchJob::Private
{
  public:
    Private(FileFetchJob *parent);
    void processNext();
    QNetworkRequest createRequest(const QUrl &url);
    QStringList fieldsToStrings(qulonglong fields);

    FileSearchQuery searchQuery;
    QStringList filesIDs;
    bool isFeed;

    bool updateViewedDate;

    qulonglong fields;

  private:
    FileFetchJob *const q;
};

FileFetchJob::Private::Private(FileFetchJob *parent):
    isFeed(false),
    updateViewedDate(false),
    fields(FileFetchJob::AllFields),
    q(parent)
{
}

QNetworkRequest FileFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    return request;
}

QStringList FileFetchJob::Private::fieldsToStrings(qulonglong fields)
{
    if (fields & AllFields) {
        return QStringList();
    }

    QStringList fieldsStrings;
    // Always fetch kind
    fieldsStrings << QLatin1String("kind");

    // FIXME: Use QMetaEnum once it supports enums larger than int
    if (fields & Id) {
        fieldsStrings << QLatin1String("id");
    }
    if (fields & Title) {
        fieldsStrings << QLatin1String("title");
    }
    if (fields & MimeType) {
        fieldsStrings << QLatin1String("mimeType");
    }
    if (fields & Description) {
        fieldsStrings << QLatin1String("description");
    }
    if (fields & Labels) {
        fieldsStrings << QLatin1String("labels");
    }
    if (fields & CreatedDate) {
        fieldsStrings << QLatin1String("createdDate");
    }
    if (fields & ModifiedDate) {
        fieldsStrings << QLatin1String("modifiedDate");
    }
    if (fields & ModifiedByMeDate) {
        fieldsStrings << QLatin1String("modifiedByMeDate");
    }
    if (fields & DownloadUrl) {
        fieldsStrings << QLatin1String("downloadUrl");
    }
    if (fields & IndexableText) {
        fieldsStrings << QLatin1String("indexableText");
    }
    if (fields & UserPermission) {
        fieldsStrings << QLatin1String("userPermission");
    }
    if (fields & FileExtension) {
        fieldsStrings << QLatin1String("fileExtension");
    }
    if (fields & MD5Checksum) {
        fieldsStrings << QLatin1String("md5Checksum");
    }
    if (fields & FileSize) {
        fieldsStrings << QLatin1String("fileSize");
    }
    if (fields & AlternateLink) {
        fieldsStrings << QLatin1String("alternateLink");
    }
    if (fields & EmbedLink) {
        fieldsStrings << QLatin1String("embedLink");
    }
    if (fields & SharedWithMeDate) {
        fieldsStrings << QLatin1String("sharedWithMeDate");
    }
    if (fields & Parents) {
        fieldsStrings << QLatin1String("parents");
    }
    if (fields & ExportLinks) {
        fieldsStrings << QLatin1String("exportLinks");
    }
    if (fields & OriginalFilename) {
        fieldsStrings << QLatin1String("originalFilename");
    }
    if (fields & OwnerNames) {
        fieldsStrings << QLatin1String("ownerNames");
    }
    if (fields & LastModifiedByMeDate) {
        fieldsStrings << QLatin1String("lastModifiedByMeDate");
    }
    if (fields & Editable) {
        fieldsStrings << QLatin1String("editable");
    }
    if (fields & WritersCanShare) {
        fieldsStrings << QLatin1String("writersCanShare");
    }
    if (fields & ThumbnailLink) {
        fieldsStrings << QLatin1String("thumbnailLink");
    }
    if (fields & LastViewedByMeDate) {
        fieldsStrings << QLatin1String("lastViewedByMeDate");
    }
    if (fields & WebContentLink) {
        fieldsStrings << QLatin1String("webContentLink");
    }
    if (fields & ExplicitlyTrashed) {
        fieldsStrings << QLatin1String("explicitlyTrashed");
    }
    if (fields & ImageMediaMetadata) {
        fieldsStrings << QLatin1String("imageMediaMetadata");
    }
    if (fields & Thumbnail) {
        fieldsStrings << QLatin1String("thumbnail");
    }
    if (fields & WebViewLink) {
        fieldsStrings << QLatin1String("webViewLink");
    }
    if (fields & IconLink) {
        fieldsStrings << QLatin1String("iconLink");
    }
    if (fields & Shared) {
        fieldsStrings << QLatin1String("shared");
    }
    if (fields & Owners) {
        fieldsStrings << QLatin1String("owners");
    }
    if (fields & LastModifyingUser) {
        fieldsStrings << QLatin1String("lastModifyingUser");
    }
    if (fields & AppDataContents) {
        fieldsStrings << QLatin1String("appDataContents");
    }
    if (fields & OpenWithLinks) {
        fieldsStrings << QLatin1String("openWithLinks");
    }
    if (fields & DefaultOpenWithLink) {
        fieldsStrings << QLatin1String("defaultOpenWithLink");
    }
    if (fields & HeadRevisionId) {
        fieldsStrings << QLatin1String("headRevisionId");
    }
    if (fields & Copyable) {
        fieldsStrings << QLatin1String("copyable");
    }
    if (fields & Properties) {
        fieldsStrings << QLatin1String("properties");
    }
    if (fields & MarkedViewedByMeDate) {
        fieldsStrings << QLatin1String("markedViewedByMeDate");
    }
    if (fields & Version) {
        fieldsStrings << QLatin1String("version");
    }
    if (fields & SharingUser) {
        fieldsStrings << QLatin1String("sharingUser");
    }
    if (fields & Permissions) {
        fieldsStrings << QLatin1String("permissions");
    }

    return fieldsStrings;
}


void FileFetchJob::Private::processNext()
{
    QUrl url;

    if (isFeed) {
        url = DriveService::fetchFilesUrl();
        if (!searchQuery.isEmpty()) {
            url.addQueryItem(QLatin1String("q"), searchQuery.serialize());
        }
        if (fields != FileFetchJob::AllFields) {
            const QStringList fieldsStrings = fieldsToStrings(fields);
            url.addQueryItem(QLatin1String("fields"),
                             QString::fromLatin1("etag,kind,nextLink,nextPageToken,selfLink,items(%1)").arg(fieldsStrings.join(QLatin1String(","))));
        }
    } else {
        if (filesIDs.isEmpty()) {
            q->emitFinished();
            return;
        }

        const QString fileId = filesIDs.takeFirst();
        url = DriveService::fetchFileUrl(fileId);

        if (fields != FileFetchJob::AllFields) {
            const QStringList fieldsStrings = fieldsToStrings(fields);
            url.addQueryItem(QLatin1String("fields"), fieldsStrings.join(QLatin1String(",")));
        }
    }

    q->enqueueRequest(createRequest(url));
}

FileFetchJob::FileFetchJob(const QString &fileId,
                           const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->filesIDs << fileId;
}

FileFetchJob::FileFetchJob(const QStringList &filesIds,
                           const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->filesIDs << filesIds;
}

FileFetchJob::FileFetchJob(const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->isFeed = true;
}

FileFetchJob::FileFetchJob(const FileSearchQuery &query,
                           const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->isFeed = true;
    d->searchQuery = query;
}


FileFetchJob::~FileFetchJob()
{
    delete d;
}

bool FileFetchJob::updateViewedDate() const
{
    return d->updateViewedDate;
}

void FileFetchJob::setUpdateViewedDate(bool updateViewedDate)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify updateViewedDate property when job is running.";
        return;
    }

    d->updateViewedDate = updateViewedDate;
}

void FileFetchJob::start()
{
    d->processNext();
}

void FileFetchJob::setFields(qulonglong fields)
{
    d->fields = fields;
}

qulonglong FileFetchJob::fields() const
{
    return d->fields;
}


ObjectsList FileFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                               const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->isFeed) {
            FeedData feedData;

            items << File::fromJSONFeed(rawData, feedData);

            if (feedData.nextPageUrl.isValid()) {
                const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
                enqueueRequest(request);
            }

        } else {
            items << File::fromJSON(rawData);

            d->processNext();
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    return items;
}


