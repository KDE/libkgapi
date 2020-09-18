/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactsgroupfetchjob.h"
#include "contactsgroup.h"
#include "contactsservice.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN ContactsGroupFetchJob::Private
{
  public:
    Private(ContactsGroupFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    QString groupId;

  private:
    ContactsGroupFetchJob * const q;
};

ContactsGroupFetchJob::Private::Private(ContactsGroupFetchJob* parent):
    q(parent)
{
}

QNetworkRequest ContactsGroupFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    return request;
}


ContactsGroupFetchJob::ContactsGroupFetchJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

ContactsGroupFetchJob::ContactsGroupFetchJob(const QString& groupId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->groupId = groupId;
}

ContactsGroupFetchJob::~ContactsGroupFetchJob()
{
    delete d;
}

void ContactsGroupFetchJob::start()
{
    QUrl url;
    if (d->groupId.isEmpty()) {
        url = ContactsService::fetchAllGroupsUrl(account()->accountName());
    } else {
        url = ContactsService::fetchGroupUrl(account()->accountName(), d->groupId);
    }
    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList ContactsGroupFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    ObjectsList items;
    QString itemId;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->groupId.isEmpty()) {
            items =  ContactsService::parseJSONFeed(rawData, feedData);
        } else {
            items << ContactsService::JSONToContactsGroup(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitResult();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        emitProgress(feedData.itemsPerPage * feedData.startIndex, feedData.totalResults);

        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


