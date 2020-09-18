/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "postmodifyjob.h"
#include "post.h"
#include "bloggerservice.h"
#include "account.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PostModifyJob::Private
{
  public:
    Private(const PostPtr &post);

    const PostPtr post;
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
    QNetworkRequest request(url);

    const QByteArray rawData = Post::toJSON(d->post);
    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList PostModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitResult();
        return items;
    }

    items << Post::fromJSON(rawData);
    emitResult();
    return items;
}
