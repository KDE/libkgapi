/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "fileabstractresumablejob.h"
#include "../debug.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMimeDatabase>

#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

namespace {
    static const int ChunkSize = 262144;
}

class Q_DECL_HIDDEN FileAbstractResumableJob::Private
{
  public:
    Private(FileAbstractResumableJob *parent);
    void startUploadSession();
    void uploadChunk(bool lastChunk);
    void processNext();

    FilePtr metaData;

    QString sessionPath;
    QList<QByteArray> chunks;
    int uploadedSize = 0;

    enum SessionState {
        ReadyStart,
        Started,
        ClientEnough,
        Completed
    };

    SessionState sessionState = ReadyStart;

  private:
    FileAbstractResumableJob *const q;
};

FileAbstractResumableJob::Private::Private(FileAbstractResumableJob *parent):
    q(parent)
{
}

void FileAbstractResumableJob::Private::startUploadSession()
{
    qCDebug(KGAPIDebug) << "Opening resumable upload session";

    // Setup job url and generic params
    QUrl url = q->createUrl();
    q->updateUrl(url);

    QUrlQuery query(url);
    query.removeQueryItem(QStringLiteral("uploadType"));
    query.addQueryItem(QStringLiteral("uploadType"), QStringLiteral("resumable"));
    url.setQuery(query);

    QNetworkRequest request(url);
    QByteArray rawData;
    if (!metaData.isNull()) {
        if (metaData->mimeType().isEmpty() && !chunks.isEmpty()) {
            // No mimeType set, determine from title and first chunk
            const QMimeDatabase db;
            const QMimeType mime = db.mimeTypeForFileNameAndData(metaData->title(), chunks.first());
            const QString contentType = mime.name();
            metaData->setMimeType(contentType);
            qCDebug(KGAPIDebug) << "Metadata mimeType was missing, determined" << contentType;
        }
        qCDebug(KGAPIDebug) << "Metadata has mimeType" << metaData->mimeType();
        
        rawData = File::toJSON(metaData);
    }

    QString contentType = QStringLiteral("application/json");

    request.setHeader(QNetworkRequest::ContentLengthHeader, rawData.length());
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

    q->enqueueRequest(request, rawData, contentType);
}

void FileAbstractResumableJob::Private::uploadChunk(bool lastChunk)
{
    QString rangeHeader;
    QByteArray partData;
    if (chunks.isEmpty()) {
        // We have consumed everything but must send one last request with total file size
        qCDebug(KGAPIDebug) << "Chunks is empty, sending only final size" << uploadedSize;
        rangeHeader = QStringLiteral("bytes */%1").arg(uploadedSize);
    } else {
        partData = chunks.takeFirst();
        // Build range header from saved upload size and new
        QString tempRangeHeader = QStringLiteral("bytes %1-%2/%3").arg(uploadedSize).arg(uploadedSize + partData.size() - 1);
        if (lastChunk) {
            // Need to send last chunk, therefore final file size is known now
            tempRangeHeader = tempRangeHeader.arg(uploadedSize + partData.size());
        } else {
            // In the middle of the upload, file size is not yet known so use star
            tempRangeHeader = tempRangeHeader.arg(QStringLiteral("*"));
        }
        rangeHeader = tempRangeHeader;
    }

    qCDebug(KGAPIDebug) << "Sending chunk of" << partData.size() << "bytes with Content-Range header" << rangeHeader;

    QUrl url = QUrl(sessionPath);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Content-Range"), rangeHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentLengthHeader, partData.length());
    q->enqueueRequest(request, partData);
    uploadedSize += partData.size();
}

void FileAbstractResumableJob::Private::processNext()
{
    qCDebug(KGAPIDebug) << "Processing next";

    switch (sessionState) {
        case ReadyStart:
            startUploadSession();
            return;
        case Started: {
            if (chunks.isEmpty() || chunks.first().size() < ChunkSize) {
                qCDebug(KGAPIDebug) << "Chunks empty or not big enough to process, asking for more";

                // Warning: an endless loop could be started here is the signal receiver isn't using
                // a direct connection.
                q->emitReadyWrite();
                processNext();
                return;
            }
            uploadChunk(false);
            return;
        }
        case ClientEnough: {
            uploadChunk(true);
            sessionState = Completed;
            return;
        }
        case Completed:
            qCDebug(KGAPIDebug) << "Nothing left to process, done";
            q->emitFinished();
            return;
    }
}


FileAbstractResumableJob::FileAbstractResumableJob(const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
}

FileAbstractResumableJob::FileAbstractResumableJob(const FilePtr &metadata,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->metaData = metadata;
}

FileAbstractResumableJob::~FileAbstractResumableJob() = default;

FilePtr FileAbstractResumableJob::metadata() const
{
    return d->metaData;
}

void FileAbstractResumableJob::write(const QByteArray &data)
{
    qCDebug(KGAPIDebug) << "Received" << data.size() << "bytes to upload";

    if (data.isEmpty()) {
        qCDebug(KGAPIDebug) << "Data empty, won't receive any more data from client";
        d->sessionState = Private::ClientEnough;
        return;
    }

    int pos = 0;
    // Might need to add to last chunk
    if (!d->chunks.isEmpty() && d->chunks.last().size() < ChunkSize) {
        QByteArray lastChunk = d->chunks.takeLast();
        int missing = ChunkSize - lastChunk.size();
        qCDebug(KGAPIDebug) << "Previous last chunk was" << lastChunk.size() << "bytes and could use" << missing << "bytes more, adding to it";
        lastChunk.append(data.mid(0, missing));
        pos = missing;
        d->chunks << lastChunk;
    }

    int dataSize = data.size();
    QList<QByteArray> chunks;
    while(pos < dataSize) {
        QByteArray chunk = data.mid(pos, ChunkSize);
        chunks << chunk;
        pos += chunk.size();
    }

    qCDebug(KGAPIDebug) << "Added" << chunks.size() << "new chunks";
    d->chunks << chunks;
}

void FileAbstractResumableJob::start()
{
    // Ask for more chunks right away in case
    // write() wasn't called before starting
    if (d->chunks.isEmpty()) {
        emitReadyWrite();
    }
    d->processNext();
}

void FileAbstractResumableJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                            const QNetworkRequest &request,
                                            const QByteArray &data,
                                            const QString &contentType)
{
    Q_UNUSED(contentType)

    if (d->sessionState == Private::ReadyStart) {
        accessManager->post(request, data);
    } else {
        accessManager->put(request, data);
    }

}

void FileAbstractResumableJob::handleReply(const QNetworkReply *reply,
                                        const QByteArray &rawData)
{
    Q_UNUSED(rawData)

    int replyCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch (d->sessionState) {
        case Private::ReadyStart: {
            if (replyCode != KGAPI2::OK) {
                qCWarning(KGAPIDebug) << "Failed opening upload session" << replyCode;
                setError(KGAPI2::UnknownError);
                setErrorString(tr("Failed opening upload session"));
                emitFinished();
                return;
            }

            const QString uploadLocation = reply->header(QNetworkRequest::LocationHeader).toString();
            qCDebug(KGAPIDebug) << "Got upload session location" << uploadLocation;
            d->sessionPath = uploadLocation;
            d->sessionState = Private::Started;
            break;
        }
        case Private::Started: {
            if (replyCode != KGAPI2::ResumeIncomplete) {
                qCWarning(KGAPIDebug) << "Failed uploading chunk" << replyCode;
                setError(KGAPI2::UnknownError);
                setErrorString(tr("Failed uploading chunk"));
                emitFinished();
                return;
            }

            // Server could send us a new upload session location any time, use it if present
            const QString newUploadLocation = reply->header(QNetworkRequest::LocationHeader).toString();
            if (!newUploadLocation.isEmpty()) {
                qCDebug(KGAPIDebug) << "Got new location" << newUploadLocation;
                d->sessionPath = newUploadLocation;
            }

            const QString readRange = QString::fromUtf8(reply->rawHeader(QStringLiteral("Range").toUtf8()));
            qCDebug(KGAPIDebug) << "Server confirms range" << readRange;
            break;
        }
        case Private::ClientEnough:
        case Private::Completed:
            if (replyCode != KGAPI2::OK) {
                qCWarning(KGAPIDebug) << "Failed completing upload session" << replyCode;
                setError(KGAPI2::UnknownError);
                setErrorString(tr("Failed completing upload session"));
                emitFinished();
                return;
            }
            const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
            ContentType ct = Utils::stringToContentType(contentType);
            if (ct == KGAPI2::JSON) {
                d->metaData = File::fromJSON(rawData);
            }
            break;
    }


    d->processNext();
}

void FileAbstractResumableJob::emitReadyWrite()
{
    Q_EMIT readyWrite(this);
}

#include "moc_fileabstractresumablejob.cpp"
