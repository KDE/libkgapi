/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "pagedeletejob.h"
#include "page.h"
#include "bloggerservice.h"

#include <libkgapi2/account.h>
#include <libkgapi2/utils.h>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PageDeleteJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &pageId);

    QString blogId;
    QString pageId;
};

PageDeleteJob::Private::Private(const QString &blogId_, const QString &pageId_)
    : blogId(blogId_)
    , pageId(pageId_)
{
}

PageDeleteJob::PageDeleteJob(const QString &blogId,
                             const QString &pageId,
                             const AccountPtr &account,
                             QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(blogId, pageId))
{
}

PageDeleteJob::PageDeleteJob(const PagePtr &page,
                             const AccountPtr &account,
                             QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(page->blogId(), page->id()))
{
}

PageDeleteJob::~PageDeleteJob()
{
    delete d;
}

void PageDeleteJob::start()
{
    QNetworkRequest request(BloggerService::deletePageUrl(d->blogId, d->pageId));
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

void PageDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply);
    Q_UNUSED(rawData);

    emitFinished();
}




#include "pagedeletejob.moc"
