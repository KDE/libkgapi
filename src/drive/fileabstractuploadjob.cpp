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


#include "fileabstractuploadjob.h"
#include "account.h"
#include "../debug.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QMimeType>
#include <QMimeDatabase>
#include <QFile>
#include <QCryptographicHash>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileAbstractUploadJob::Private
{
  public:
    Private(FileAbstractUploadJob *parent);
    void processNext();
    QByteArray buildMultipart(const QString &filePath,
                              const FilePtr &metaData,
                              QString &boundary);
    QByteArray readFile(const QString &filePath, QString &contentType);

    void _k_uploadProgress(qint64 bytesSent, qint64 totalBytes);

    int originalFilesCount;
    QMap<QString, FilePtr> files;

    QMap<QString, FilePtr> uploadedFiles;


    bool useContentAsIndexableText;
    File::SerializationOptions serializationOptions = File::NoOptions;

  private:
    FileAbstractUploadJob *const q;
};

FileAbstractUploadJob::Private::Private(FileAbstractUploadJob *parent):
    originalFilesCount(0),
    useContentAsIndexableText(false),
    q(parent)
{
}

QByteArray FileAbstractUploadJob::Private::readFile(const QString &filePath,
                                                    QString &contentType)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(KGAPIDebug) << "Failed to access" << filePath;
        return QByteArray();
    }

    const QMimeDatabase db;
    const QMimeType mime = db.mimeTypeForFileNameAndData(filePath, &file);
    contentType = mime.name();

    file.reset();
    QByteArray output = file.readAll();

    file.close();

    return output;
}

QByteArray FileAbstractUploadJob::Private::buildMultipart(const QString &filePath,
                                                          const FilePtr &metaData,
                                                          QString &boundary)
{
    QString fileContentType;
    QByteArray fileContent;

    fileContent = readFile(filePath, fileContentType);
    if (fileContent.isEmpty()) {
        return QByteArray();
    }

    // Wannabe implementation of RFC2387, i.e. multipart/related
    QByteArray body;
    const QByteArray md5 = QCryptographicHash::hash(filePath.toLatin1(), QCryptographicHash::Md5);
    boundary = QString::fromLatin1(md5.toHex());

    body += "--" + boundary.toLatin1() + '\n';
    body += "Content-Type: application/json; charset=UTF-8\n";
    body += '\n';
    body += File::toJSON(metaData);
    body += '\n';
    body += '\n';
    body += "--" + boundary.toLatin1() + '\n';
    body += "Content-Type: " + fileContentType.toLatin1() + '\n';
    body += '\n';
    body += fileContent;
    body += '\n';
    body += "--" + boundary.toLatin1() + "--";

    return body;
}

void FileAbstractUploadJob::Private::processNext()
{
    if (files.isEmpty()) {
        q->emitFinished();
        return;
    }

    const QString filePath = files.cbegin().key();
    if (!filePath.startsWith(QLatin1String("?=")) && !QFile::exists(filePath)) {
        qCWarning(KGAPIDebug) << filePath << "is not a valid file path";
        processNext();
        return;
    }

    const FilePtr metaData = files.take(filePath);

    QUrl url;
    if (filePath.startsWith(QLatin1String("?="))) {
        url = q->createUrl(QString(), metaData);
    } else {
        url = q->createUrl(filePath, metaData);
    }

    q->updateUrl(url);
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("useContentAsIndexableText"), Utils::bool2Str(useContentAsIndexableText));

    QNetworkRequest request;
    QByteArray rawData;
    QString contentType;

    // just to be sure
    query.removeQueryItem(QStringLiteral("uploadType"));
    if (metaData.isNull()) {
        query.addQueryItem(QStringLiteral("uploadType"), QStringLiteral("media"));

        rawData = readFile(filePath, contentType);
        if (rawData.isEmpty()) {
            processNext();
            return;
        }

    } else if (!filePath.startsWith(QLatin1String("?="))) {
        query.addQueryItem(QStringLiteral("uploadType"), QStringLiteral("multipart"));

        QString boundary;
        rawData = buildMultipart(filePath, metaData, boundary);

        contentType = QStringLiteral("multipart/related; boundary=%1").arg(boundary);
        if (rawData.isEmpty()) {
            processNext();
            return;
        }
    } else {
        rawData = File::toJSON(metaData, q->serializationOptions());
        contentType = QStringLiteral("application/json");
    }
    url.setQuery(query);

    request.setUrl(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setHeader(QNetworkRequest::ContentLengthHeader, rawData.length());
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    request.setAttribute(QNetworkRequest::User, filePath);

    q->enqueueRequest(request, rawData, contentType);
}

void FileAbstractUploadJob::Private::_k_uploadProgress(qint64 bytesSent,
        qint64 totalBytes)
{
    // Each file consists of 100 units, so if we have two files, one already
    // uploaded and the other one uploaded from 50%, the values are (150, 200)

    int processedParts = (originalFilesCount - files.count()) * 100;
    int currentFileParts = 100.0 * ((qreal) bytesSent / (qreal) totalBytes);

    q->emitProgress(processedParts + currentFileParts, originalFilesCount * 100);
}

FileAbstractUploadJob::FileAbstractUploadJob(const FilePtr &metadata,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(QStringLiteral("?=0"), metadata);
    d->originalFilesCount = 1;
}

FileAbstractUploadJob::FileAbstractUploadJob(const FilesList &metadata,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    int i = 0;
    for (const FilePtr &file : metadata) {
        d->files.insert(QStringLiteral("?=%1").arg(i), file);
        ++i;
    }
    d->originalFilesCount = d->files.count();
}

FileAbstractUploadJob::FileAbstractUploadJob(const QString &filePath,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(filePath, FilePtr());
    d->originalFilesCount = 1;
}

FileAbstractUploadJob::FileAbstractUploadJob(const QString &filePath,
                                             const FilePtr &metaData,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(filePath, metaData);
    d->originalFilesCount = 1;
}

FileAbstractUploadJob::FileAbstractUploadJob(const QStringList &filePaths,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    for (const QString & filePath : filePaths) {
        d->files.insert(filePath, FilePtr());
    }
    d->originalFilesCount = d->files.count();
}

FileAbstractUploadJob::FileAbstractUploadJob(const QMap< QString, FilePtr > &files,
                                             const AccountPtr &account,
                                             QObject *parent):
    FileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files = files;
    d->originalFilesCount = d->files.count();
}

FileAbstractUploadJob::~FileAbstractUploadJob()
{
    delete d;
}

void FileAbstractUploadJob::setUseContentAsIndexableText(bool useContentAsIndexableText)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify useContentAsIndexableText property when job is running";
        return;
    }

    d->useContentAsIndexableText = useContentAsIndexableText;
}

bool FileAbstractUploadJob::useContentAsIndexableText() const
{
    return d->useContentAsIndexableText;
}

void FileAbstractUploadJob::start()
{
    d->processNext();
}

void FileAbstractUploadJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                            const QNetworkRequest &request,
                                            const QByteArray &data,
                                            const QString &contentType)
{
    Q_UNUSED(contentType)

    QNetworkReply *reply = dispatch(accessManager, request, data);

    connect(reply, &QNetworkReply::uploadProgress,
            this, [this](qint64 bytesSent, qint64 totalBytes) {d->_k_uploadProgress(bytesSent, totalBytes); });
}

void FileAbstractUploadJob::handleReply(const QNetworkReply *reply,
                                        const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        const QNetworkRequest request = reply->request();
        const QString filePath = request.attribute(QNetworkRequest::User).toString();

        FilePtr file = File::fromJSON(rawData);

        d->uploadedFiles.insert(filePath, file);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return;
    }

    d->processNext();
}

void FileAbstractUploadJob::setSerializationOptions(File::SerializationOptions options)
{
    d->serializationOptions = options;
}

File::SerializationOptions FileAbstractUploadJob::serializationOptions() const
{
    return d->serializationOptions;
}


#include "moc_fileabstractuploadjob.cpp"
