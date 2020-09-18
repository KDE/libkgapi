/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "pagedeletejob.h"
#include "page.h"
#include "bloggerservice.h"
#include "account.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PageDeleteJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &pageId);

    const QString blogId;
    const QString pageId;
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

    enqueueRequest(request);
}

void PageDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply);
    Q_UNUSED(rawData);

    emitResult();
}





