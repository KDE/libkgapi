/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "postcreatejob.h"
#include "bloggerservice.h"
#include "post.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PostCreateJob::Private
{
public:
    Private(const PostPtr &post, bool isDraft);

    const PostPtr post;
    const bool isDraft;
};

PostCreateJob::Private::Private(const PostPtr &post_, bool isDraft_)
    : post(post_)
    , isDraft(isDraft_)
{
}

PostCreateJob::PostCreateJob(const PostPtr &post, bool isDraft, const AccountPtr &account, QObject *parent)
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
        QUrlQuery query(url);
        query.addQueryItem(QStringLiteral("isDraft"), Utils::bool2Str(d->isDraft));
        url.setQuery(query);
    }
    QNetworkRequest request(url);

    const QByteArray rawData = Post::toJSON(d->post);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList PostCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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

    items << Post::fromJSON(rawData);
    emitFinished();
    return items;
}

#include "moc_postcreatejob.cpp"
