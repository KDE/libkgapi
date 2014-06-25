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

#include "postcreatejob.h"
#include "post.h"
#include "bloggerservice.h"

#include <libkgapi2/account.h>
#include <libkgapi2/utils.h>

#include <QNetworkReply>
#include <QNetworkRequest>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PostCreateJob::Private
{
  public:
    Private(const PostPtr &post,
            bool isDraft);

    PostPtr post;
    bool isDraft;
};

PostCreateJob::Private::Private(const PostPtr &post_,
                                bool isDraft_)
    : post(post_)
    , isDraft(isDraft_)
{
}

PostCreateJob::PostCreateJob(const PostPtr &post,
                             bool isDraft,
                             const AccountPtr &account,
                             QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(post, isDraft))
{
}

PostCreateJob::~PostCreateJob()
{
    delete d;
}


void PostCreateJob::start()
{
    QUrl url = BloggerService::createPostUrl(d->post->blogId());
    if (d->isDraft) {
        url.addQueryItem(QLatin1String("isDraft"), Utils::bool2Str(d->isDraft));
    }
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = Post::toJSON(d->post);

    enqueueRequest(request, rawData, QLatin1String("application/json"));
}

ObjectsList PostCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
