/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "tasklistfetchjob.h"
#include "tasksservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskListFetchJob::Private
{
public:
    Private(TaskListFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

private:
    TaskListFetchJob *const q;
};

TaskListFetchJob::Private::Private(TaskListFetchJob *parent)
    : q(parent)
{
}

QNetworkRequest TaskListFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request(url);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1StringView(str) + QLatin1StringView(": ") + QLatin1StringView(request.rawHeader(str));
    }

    return request;
}

TaskListFetchJob::TaskListFetchJob(const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
}

TaskListFetchJob::~TaskListFetchJob() = default;

void TaskListFetchJob::start()
{
    const QUrl url = TasksService::fetchTaskListsUrl();
    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList TaskListFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    ObjectsList items;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        items = TasksService::parseJSONFeed(rawData, feedData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}

#include "moc_tasklistfetchjob.cpp"
