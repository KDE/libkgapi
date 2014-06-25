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

#include "pagecreatejob.h"
#include "page.h"
#include "bloggerservice.h"

#include <libkgapi2/utils.h>
#include <libkgapi2/account.h>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PageCreateJob::Private
{
  public:
    Private(const PagePtr &page);

    PagePtr page;
};

PageCreateJob::Private::Private(const PagePtr &page_)
    : page(page_)
{
}

PageCreateJob::PageCreateJob(const PagePtr &page,
                             const AccountPtr &account,
                             QObject *parent)
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
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = Page::toJSON(d->page);

    enqueueRequest(request, rawData, QLatin1String("application/json"));
}

ObjectsList PageCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << Page::fromJSON(rawData);
    emitFinished();
    return items;
}

#include "pagecreatejob.moc"

