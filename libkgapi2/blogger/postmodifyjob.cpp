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

#include "postmodifyjob.h"
#include "post.h"
#include "bloggerservice.h"

#include <libkgapi2/account.h>
#include <libkgapi2/utils.h>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PostModifyJob::Private
{
  public:
    Private(const PostPtr &post);

    PostPtr post;
};

PostModifyJob::Private::Private(const PostPtr &post_)
    : post(post_)
{
}

PostModifyJob::PostModifyJob(const PostPtr &post,
                             const AccountPtr &account,
                             QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(post))
{
}

PostModifyJob::~PostModifyJob()
{
    delete d;
}

void PostModifyJob::start()
{
    const QUrl url = BloggerService::modifyPostUrl(d->post->blogId(), d->post->id());
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = Post::toJSON(d->post);
    enqueueRequest(request, rawData, QLatin1String("application/json"));
}

ObjectsList PostModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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

    items << Post::fromJSON(rawData);
    emitFinished();
    return items;
}
