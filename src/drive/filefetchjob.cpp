/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "filefetchjob.h"
#include "debug.h"
#include "driveservice.h"
#include "file.h"
#include "filesearchquery.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileFetchJob::Private
{
public:
    Private(FileFetchJob *parent);
    void processNext();

    FileSearchQuery searchQuery;
    QStringList filesIDs;
    bool isFeed = false;
    bool includeItemsFromAllDrives = true;
    bool supportsAllDrives = true;

    bool updateViewedDate = false;

    QStringList fields;

private:
    FileFetchJob *const q;
};

FileFetchJob::Private::Private(FileFetchJob *parent)
    : q(parent)
{
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

        query.addQueryItem(QStringLiteral("includeItemsFromAllDrives"), Utils::bool2Str(includeItemsFromAllDrives));

        url.setQuery(query);

        if (!fields.isEmpty()) {
            // Deserializing requires kind attribute, always force add it
            if (!fields.contains(File::Fields::Kind)) {
                fields << File::Fields::Kind;
            }
            Job *baseJob = dynamic_cast<Job *>(q);
            baseJob->setFields({File::Fields::Etag,
                                File::Fields::Kind,
                                File::Fields::NextLink,
                                File::Fields::NextPageToken,
                                File::Fields::SelfLink,
                                Job::buildSubfields(File::Fields::Items, fields)});
        }
    } else {
        if (filesIDs.isEmpty()) {
            q->emitFinished();
            return;
        }

        const QString fileId = filesIDs.takeFirst();
        url = DriveService::fetchFileUrl(fileId);

        if (!fields.isEmpty()) {
            // Deserializing requires kind attribute, always force add it
            if (!fields.contains(File::Fields::Kind)) {
                fields << File::Fields::Kind;
            }
            Job *baseJob = dynamic_cast<Job *>(q);
            baseJob->setFields(fields);
        }
    }

    QUrlQuery withDriveSupportQuery(url);
    withDriveSupportQuery.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(supportsAllDrives));
    url.setQuery(withDriveSupportQuery);

    QNetworkRequest request(url);
    q->enqueueRequest(request);
}

FileFetchJob::FileFetchJob(const QString &fileId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->filesIDs << fileId;
}

FileFetchJob::FileFetchJob(const QStringList &filesIds, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->filesIDs << filesIds;
}

FileFetchJob::FileFetchJob(const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->isFeed = true;
}

FileFetchJob::FileFetchJob(const FileSearchQuery &query, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
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

void FileFetchJob::setFields(const QStringList &fields)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called setFields() on running job. Ignoring.";
        return;
    }

    d->fields = fields;
}

QStringList FileFetchJob::fields() const
{
    return d->fields;
}

bool FileFetchJob::includeItemsFromAllDrives() const
{
    return d->includeItemsFromAllDrives;
}

void FileFetchJob::setIncludeItemsFromAllDrives(bool includeItemsFromAllDrives)
{
    d->includeItemsFromAllDrives = includeItemsFromAllDrives;
}

bool FileFetchJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void FileFetchJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

const QStringList FileFetchJob::FieldShorthands::BasicFields = {File::Fields::Id,
                                                                File::Fields::Title,
                                                                File::Fields::MimeType,
                                                                File::Fields::CreatedDate,
                                                                File::Fields::ModifiedDate,
                                                                File::Fields::FileSize,
                                                                File::Fields::DownloadUrl,
                                                                File::Fields::Permissions};

const QStringList FileFetchJob::FieldShorthands::AccessFields = {File::Fields::CreatedDate,
                                                                 File::Fields::ModifiedDate,
                                                                 File::Fields::ModifiedByMeDate,
                                                                 File::Fields::LastModifiedByMeDate,
                                                                 File::Fields::LastViewedByMeDate,
                                                                 File::Fields::MarkedViewedByMeDate};

const QStringList FileFetchJob::FieldShorthands::SharingFields = {File::Fields::SharedWithMeDate,
                                                                  File::Fields::WritersCanShare,
                                                                  File::Fields::Shared,
                                                                  File::Fields::Owners,
                                                                  File::Fields::SharingUser,
                                                                  File::Fields::OwnerNames};

ObjectsList FileFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->isFeed) {
            FeedData feedData;

            items << File::fromJSONFeed(rawData, feedData);

            if (feedData.nextPageUrl.isValid()) {
                QNetworkRequest request(feedData.nextPageUrl);
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

#include "moc_filefetchjob.cpp"
