/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fileabstractmodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "file.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileAbstractModifyJob::Private
{
  public:
    Private(FileAbstractModifyJob *parent);
    void processNext();

    QStringList filesIds;

    bool supportsAllDrives = true;

  private:
    FileAbstractModifyJob *const q;
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
    QUrl url = q->url(fileId);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(supportsAllDrives));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentLengthHeader, 0);

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
    for (const FilePtr & file : qAsConst(files)) {
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

bool FileAbstractModifyJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void FileAbstractModifyJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
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
