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

#include "filefetchjob.h"
#include "filesearchquery.h"
#include "account.h"
#include "../debug.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileFetchJob::Private
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
    fieldsStrings << QStringLiteral("kind");

    // FIXME: Use QMetaEnum once it supports enums larger than int
    if (fields & Id) {
        fieldsStrings << QStringLiteral("id");
    }
    if (fields & Title) {
        fieldsStrings << QStringLiteral("title");
    }
    if (fields & MimeType) {
        fieldsStrings << QStringLiteral("mimeType");
    }
    if (fields & Description) {
        fieldsStrings << QStringLiteral("description");
    }
    if (fields & Labels) {
        fieldsStrings << QStringLiteral("labels");
    }
    if (fields & CreatedDate) {
        fieldsStrings << QStringLiteral("createdDate");
    }
    if (fields & ModifiedDate) {
        fieldsStrings << QStringLiteral("modifiedDate");
    }
    if (fields & ModifiedByMeDate) {
        fieldsStrings << QStringLiteral("modifiedByMeDate");
    }
    if (fields & DownloadUrl) {
        fieldsStrings << QStringLiteral("downloadUrl");
    }
    if (fields & IndexableText) {
        fieldsStrings << QStringLiteral("indexableText");
    }
    if (fields & UserPermission) {
        fieldsStrings << QStringLiteral("userPermission");
    }
    if (fields & FileExtension) {
        fieldsStrings << QStringLiteral("fileExtension");
    }
    if (fields & MD5Checksum) {
        fieldsStrings << QStringLiteral("md5Checksum");
    }
    if (fields & FileSize) {
        fieldsStrings << QStringLiteral("fileSize");
    }
    if (fields & AlternateLink) {
        fieldsStrings << QStringLiteral("alternateLink");
    }
    if (fields & EmbedLink) {
        fieldsStrings << QStringLiteral("embedLink");
    }
    if (fields & SharedWithMeDate) {
        fieldsStrings << QStringLiteral("sharedWithMeDate");
    }
    if (fields & Parents) {
        fieldsStrings << QStringLiteral("parents");
    }
    if (fields & ExportLinks) {
        fieldsStrings << QStringLiteral("exportLinks");
    }
    if (fields & OriginalFilename) {
        fieldsStrings << QStringLiteral("originalFilename");
    }
    if (fields & OwnerNames) {
        fieldsStrings << QStringLiteral("ownerNames");
    }
    if (fields & LastModifiedByMeDate) {
        fieldsStrings << QStringLiteral("lastModifiedByMeDate");
    }
    if (fields & Editable) {
        fieldsStrings << QStringLiteral("editable");
    }
    if (fields & WritersCanShare) {
        fieldsStrings << QStringLiteral("writersCanShare");
    }
    if (fields & ThumbnailLink) {
        fieldsStrings << QStringLiteral("thumbnailLink");
    }
    if (fields & LastViewedByMeDate) {
        fieldsStrings << QStringLiteral("lastViewedByMeDate");
    }
    if (fields & WebContentLink) {
        fieldsStrings << QStringLiteral("webContentLink");
    }
    if (fields & ExplicitlyTrashed) {
        fieldsStrings << QStringLiteral("explicitlyTrashed");
    }
    if (fields & ImageMediaMetadata) {
        fieldsStrings << QStringLiteral("imageMediaMetadata");
    }
    if (fields & Thumbnail) {
        fieldsStrings << QStringLiteral("thumbnail");
    }
    if (fields & WebViewLink) {
        fieldsStrings << QStringLiteral("webViewLink");
    }
    if (fields & IconLink) {
        fieldsStrings << QStringLiteral("iconLink");
    }
    if (fields & Shared) {
        fieldsStrings << QStringLiteral("shared");
    }
    if (fields & Owners) {
        fieldsStrings << QStringLiteral("owners");
    }
    if (fields & LastModifyingUser) {
        fieldsStrings << QStringLiteral("lastModifyingUser");
    }
    if (fields & AppDataContents) {
        fieldsStrings << QStringLiteral("appDataContents");
    }
    if (fields & OpenWithLinks) {
        fieldsStrings << QStringLiteral("openWithLinks");
    }
    if (fields & DefaultOpenWithLink) {
        fieldsStrings << QStringLiteral("defaultOpenWithLink");
    }
    if (fields & HeadRevisionId) {
        fieldsStrings << QStringLiteral("headRevisionId");
    }
    if (fields & Copyable) {
        fieldsStrings << QStringLiteral("copyable");
    }
    if (fields & Properties) {
        fieldsStrings << QStringLiteral("properties");
    }
    if (fields & MarkedViewedByMeDate) {
        fieldsStrings << QStringLiteral("markedViewedByMeDate");
    }
    if (fields & Version) {
        fieldsStrings << QStringLiteral("version");
    }
    if (fields & SharingUser) {
        fieldsStrings << QStringLiteral("sharingUser");
    }
    if (fields & Permissions) {
        fieldsStrings << QStringLiteral("permissions");
    }

    return fieldsStrings;
}


void FileFetchJob::Private::processNext()
{
    QUrl url;

    if (isFeed) {
        url = DriveService::fetchFilesUrl();
        QUrlQuery query(url);
        if (!searchQuery.isEmpty()) {
            query.addQueryItem(QStringLiteral("q"), searchQuery.serialize());
        }
        if (fields != FileFetchJob::AllFields) {
            const QStringList fieldsStrings = fieldsToStrings(fields);
            query.addQueryItem(QStringLiteral("fields"),
                             QStringLiteral("etag,kind,nextLink,nextPageToken,selfLink,items(%1)").arg(fieldsStrings.join(QStringLiteral(","))));
        }
        url.setQuery(query);
    } else {
        if (filesIDs.isEmpty()) {
            q->emitFinished();
            return;
        }

        const QString fileId = filesIDs.takeFirst();
        url = DriveService::fetchFileUrl(fileId);
        if (fields != FileFetchJob::AllFields) {
            const QStringList fieldsStrings = fieldsToStrings(fields);
            QUrlQuery query(url);
            query.addQueryItem(QStringLiteral("fields"), fieldsStrings.join(QStringLiteral(",")));
            url.setQuery(query);
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
        qCWarning(KGAPIDebug) << "Can't modify updateViewedDate property when job is running.";
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


