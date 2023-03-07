/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "pagemodifyjob.h"
#include "bloggerservice.h"
#include "page.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PageModifyJob::Private
{
public:
    Private(const PagePtr &page);

    const PagePtr page;
};

PageModifyJob::Private::Private(const PagePtr &page_)
    : page(page_)
{
}

PageModifyJob::PageModifyJob(const PagePtr &page, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(page))
{
}

PageModifyJob::~PageModifyJob()
{
    delete d;
}

void PageModifyJob::start()
{
    const QUrl url = BloggerService::modifyPageUrl(d->page->blogId(), d->page->id());
    QNetworkRequest request(url);

    const QByteArray rawData = Page::toJSON(d->page);
    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList PageModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
