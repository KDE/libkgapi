/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fileabstractresumablejob.h"
#include "debug.h"
#include "utils.h"

#include <QMimeDatabase>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

namespace
{
static const int ChunkSize = 262144;
}

class Q_DECL_HIDDEN FileAbstractResumableJob::Private
{
public:
    Private(FileAbstractResumableJob *parent);
    void startUploadSession();
    void uploadChunk(bool lastChunk);
    void processNext();
    void readFromDevice();
    bool isTotalSizeKnown() const;

    void _k_uploadProgress(qint64 bytesSent, qint64 totalBytes);

    FilePtr metaData;
    QIODevice *device = nullptr;

    QString sessionPath;
    QList<QByteArray> chunks;
    int uploadedSize = 0;
    int totalUploadSize = 0;

    enum SessionState { ReadyStart, Started, ClientEnough, Completed };

    SessionState sessionState = ReadyStart;

private:
    FileAbstractResumableJob *const q;
};

FileAbstractResumableJob::Private::Private(FileAbstractResumableJob *parent)
    : q(parent)
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
            // Use star in the case that total upload size in unknown
            QString totalSymbol = isTotalSizeKnown() ? QString::number(totalUploadSize) : QStringLiteral("*");
            tempRangeHeader = tempRangeHeader.arg(totalSymbol);
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

            if (device) {
                readFromDevice();
            } else {
                // Warning: an endless loop could be started here if the signal receiver isn't using
                // a direct connection.
                q->emitReadyWrite();
            }
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

void KGAPI2::Drive::FileAbstractResumableJob::Private::readFromDevice()
{
    char buf[ChunkSize];
    int read = device->read(buf, ChunkSize);
    if (read == -1) {
        qCWarning(KGAPIDebug) << "Failed reading from device" << device->errorString();
        return;
    }
    qCDebug(KGAPIDebug) << "Read from device bytes" << read;
    q->write(QByteArray(buf, read));
}

bool FileAbstractResumableJob::Private::isTotalSizeKnown() const
{
    return totalUploadSize != 0;
}

void FileAbstractResumableJob::Private::_k_uploadProgress(qint64 bytesSent, qint64 totalBytes)
{
    // uploadedSize corresponds to total bytes enqueued (including current chunk upload)
    qint64 totalUploaded = uploadedSize - totalBytes + bytesSent;
    q->emitProgress(totalUploaded, totalUploadSize);
}

FileAbstractResumableJob::FileAbstractResumableJob(const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
}

FileAbstractResumableJob::FileAbstractResumableJob(const FilePtr &metadata, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    d->metaData = metadata;
}

FileAbstractResumableJob::FileAbstractResumableJob(QIODevice *device, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    d->device = device;
}

FileAbstractResumableJob::FileAbstractResumableJob(QIODevice *device, const FilePtr &metadata, const AccountPtr &account, QObject *parent)
    : FileAbstractDataJob(account, parent)
    , d(new Private(this))
{
    d->device = device;
    d->metaData = metadata;
}

FileAbstractResumableJob::~FileAbstractResumableJob() = default;

FilePtr FileAbstractResumableJob::metadata() const
{
    return d->metaData;
}

void FileAbstractResumableJob::setUploadSize(int size)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't set upload size when the job is already running";
        return;
    }

    d->totalUploadSize = size;
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
    while (pos < dataSize) {
        QByteArray chunk = data.mid(pos, ChunkSize);
        chunks << chunk;
        pos += chunk.size();
    }

    qCDebug(KGAPIDebug) << "Added" << chunks.size() << "new chunks";
    d->chunks << chunks;
}

void FileAbstractResumableJob::start()
{
    if (d->device) {
        d->readFromDevice();
    }
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

    QNetworkReply *reply;
    if (d->sessionState == Private::ReadyStart) {
        reply = accessManager->post(request, data);
    } else {
        reply = accessManager->put(request, data);
    }

    if (d->isTotalSizeKnown()) {
        connect(reply, &QNetworkReply::uploadProgress, this, [this](qint64 bytesSent, qint64 totalBytes) {
            d->_k_uploadProgress(bytesSent, totalBytes);
        });
    }
}

void FileAbstractResumableJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
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
        // If during upload total size is declared via Content-Range header, Google will
        // respond with 200 on the last chunk upload. The job is complete in that case.
        if (d->isTotalSizeKnown() && replyCode == KGAPI2::OK) {
            d->sessionState = Private::Completed;
            const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
            ContentType ct = Utils::stringToContentType(contentType);
            if (ct == KGAPI2::JSON) {
                d->metaData = File::fromJSON(rawData);
            }
            return;
        }

        // Google will continue answering ResumeIncomplete until the total upload size is declared
        // in the Content-Range header or until last upload range not total upload size.
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
