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


#include "fileabstractuploadjob.h"
#include "account.h"
#include "debug.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>
#include <KDE/KMimeType>
#include <QFile>
#include <QCryptographicHash>

using namespace KGAPI2;

class DriveFileAbstractUploadJob::Private
{
  public:
    Private(DriveFileAbstractUploadJob *parent);
    void processNext();
    QByteArray buildMultipart(const QString &filePath,
                              const DriveFilePtr &metaData,
                              QString &boundary);
    QByteArray readFile(const QString &filePath, QString &contentType);

    void _k_uploadProgress(qint64 bytesSent, qint64 totalBytes);

    int originalFilesCount;
    QMap<QString, DriveFilePtr> files;

    QMap<QString, DriveFilePtr> uploadedFiles;


    bool useContentAsIndexableText;

  private:
    DriveFileAbstractUploadJob * const q;
};

DriveFileAbstractUploadJob::Private::Private(DriveFileAbstractUploadJob *parent):
    useContentAsIndexableText(false),
    q(parent)
{
}

QByteArray DriveFileAbstractUploadJob::Private::readFile(const QString &filePath, QString &contentType)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        kWarning() << "Failed to access" << filePath;
        return QByteArray();
    }

    const KMimeType::Ptr mimeType = KMimeType::findByNameAndContent(filePath, &file);
    contentType = mimeType->name();

    file.reset();
    QByteArray output = file.readAll();

    file.close();

    return output;
}

QByteArray DriveFileAbstractUploadJob::Private::buildMultipart(const QString &filePath,
                                                               const DriveFilePtr &metaData,
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

    body += "---" + boundary.toLatin1() + "\n";
    body += "Content-Type: application/json; charset=UTF-8\n";
    body += "\n";
    body += DriveFile::toJSON(metaData);
    body += "\n";
    body += "\n";
    body += "---" + boundary.toLatin1() + "\n";
    body += "Content-Type: " + fileContentType.toLatin1();
    body += "\n";
    body += fileContent;
    body += "\n";
    body += "\n";
    body += "---" + boundary.toLatin1() + "---";

    return body;
}

void DriveFileAbstractUploadJob::Private::processNext()
{
    if (files.isEmpty()) {
        q->emitFinished();
        return;
    }

    const QString filePath = files.keys().first();
    if (!QFile::exists(filePath)) {
        kWarning() << filePath << "is not a valid file path";
        processNext();
        return;
    }

    const DriveFilePtr metaData = files.take(filePath);

    QUrl url = q->createUrl(filePath, metaData);
    q->updateUrl(url);
    url.addQueryItem(QLatin1String("useContentAsIndexableText"), Utils::bool2Str(useContentAsIndexableText));

    QNetworkRequest request;
    QByteArray rawData;
    QString contentType;

    // just to be sure
    url.removeQueryItem(QLatin1String("uploadType"));
    if (metaData.isNull()) {
        url.addQueryItem(QLatin1String("uploadType"), QLatin1String("media"));

        rawData = readFile(filePath, contentType);
        if (rawData.isEmpty()) {
            processNext();
            return;
        }

    } else {
        url.addQueryItem(QLatin1String("uploadType"), QLatin1String("multipart"));

        QString boundary;
        rawData = buildMultipart(filePath, metaData, boundary);

        contentType = QString::fromLatin1("multipart/related; boundary=%1").arg(boundary);
        if (rawData.isEmpty()) {
            processNext();
            return;
        }
    }

    request.setUrl(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setHeader(QNetworkRequest::ContentLengthHeader, rawData.length());
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    request.setAttribute(QNetworkRequest::User, filePath);

    q->enqueueRequest(request, rawData, contentType);
}

void DriveFileAbstractUploadJob::Private::_k_uploadProgress(qint64 bytesSent,
                                                            qint64 totalBytes)
{
    // Each file consists of 100 units, so if we have two files, one already
    // uploaded and the other one uploaded from 50%, the values are (150, 200)

    int processedParts = (originalFilesCount - files.count()) * 100;
    int currentFileParts = 100.0 * ((qreal) bytesSent / (qreal) totalBytes);

    q->emitProgress(processedParts + currentFileParts, originalFilesCount * 100);
}

DriveFileAbstractUploadJob::DriveFileAbstractUploadJob(const QString &filePath,
                                                       const AccountPtr &account,
                                                       QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(filePath, DriveFilePtr());
    d->originalFilesCount = 1;
}

DriveFileAbstractUploadJob::DriveFileAbstractUploadJob(const QString &filePath,
                                                       const DriveFilePtr &metaData,
                                                       const AccountPtr &account,
                                                       QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files.insert(filePath, metaData);
    d->originalFilesCount = 1;
}

DriveFileAbstractUploadJob::DriveFileAbstractUploadJob(const QStringList &filePaths,
                                                       const AccountPtr &account,
                                                       QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH (const QString &filePath, filePaths) {
        d->files.insert(filePath, DriveFilePtr());
    }
    d->originalFilesCount = d->files.count();
}

DriveFileAbstractUploadJob::DriveFileAbstractUploadJob(const QMap< QString, DriveFilePtr > &files,
                                                       const AccountPtr &account,
                                                       QObject *parent):
    DriveFileAbstractDataJob(account, parent),
    d(new Private(this))
{
    d->files = files;
    d->originalFilesCount = d->files.count();
}

DriveFileAbstractUploadJob::~DriveFileAbstractUploadJob()
{
    delete d;
}

void DriveFileAbstractUploadJob::setUseContentAsIndexableText(bool useContentAsIndexableText)
{
    if (isRunning()) {
        kWarning() << "Can't modify useContentAsIndexableText property when job is running";
        return;
    }

    d->useContentAsIndexableText = useContentAsIndexableText;
}

bool DriveFileAbstractUploadJob::useContentAsIndexableText() const
{
    return d->useContentAsIndexableText;
}

void DriveFileAbstractUploadJob::start()
{
    d->processNext();
}

void DriveFileAbstractUploadJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                                 const QNetworkRequest &request,
                                                 const QByteArray &data,
                                                 const QString &contentType)
{
    Q_UNUSED(contentType)

    QNetworkReply *reply = dispatch(accessManager, request, data);

    connect(reply, SIGNAL(uploadProgress(qint64,qint64)),
            this, SLOT(_k_uploadProgress(qint64,qint64)));
}

void DriveFileAbstractUploadJob::handleReply(const QNetworkReply *reply,
                                             const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        const QNetworkRequest request = reply->request();
        const QString filePath = request.attribute(QNetworkRequest::User).toString();

        DriveFilePtr file = DriveFile::fromJSON(rawData);

        d->uploadedFiles.insert(filePath, file);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return;
    }

    d->processNext();
}


#include "fileabstractuploadjob.moc"
