/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "pagecreatejob.h"
#include "account.h"
#include "bloggerservice.h"
#include "page.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PageCreateJob::Private
{
public:
    Private(const PagePtr &page);

    const PagePtr page;
};

PageCreateJob::Private::Private(const PagePtr &page_)
    : page(page_)
{
}

PageCreateJob::PageCreateJob(const PagePtr &page, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(page))
{
}

PageCreateJob::~PageCreateJob()
{
    delete d;
}

void PageCreateJob::start()
{
    const QUrl url = BloggerService::createPageUrl(d->page->blogId());
    QNetworkRequest request(url);

    const QByteArray rawData = Page::toJSON(d->page);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList PageCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << Page::fromJSON(rawData);
    emitFinished();
    return items;
}
