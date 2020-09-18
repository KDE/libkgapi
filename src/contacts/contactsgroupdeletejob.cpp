/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactsgroupdeletejob.h"
#include "contactsgroup.h"
#include "contactsservice.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactsGroupDeleteJob::Private
{
  public:
    Private(ContactsGroupDeleteJob *parent);
    QNetworkRequest createRequest(const QUrl &url) const;

    QueueHelper<QString> groupsIds;

  private:
    ContactsGroupDeleteJob * const q;
};

ContactsGroupDeleteJob::Private::Private(ContactsGroupDeleteJob* parent):
    q(parent)
{
}

QNetworkRequest ContactsGroupDeleteJob::Private::createRequest(const QUrl& url) const
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


ContactsGroupDeleteJob::ContactsGroupDeleteJob(const ContactsGroupsList& groups, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->groupsIds.reserve(groups.size());
    for(const ContactsGroupPtr &group : groups) {
        d->groupsIds << group->id();
    }
}

ContactsGroupDeleteJob::ContactsGroupDeleteJob(const ContactsGroupPtr& group, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->groupsIds << group->id();
}

ContactsGroupDeleteJob::ContactsGroupDeleteJob(const QStringList &groupsIds, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->groupsIds = groupsIds;
}

ContactsGroupDeleteJob::ContactsGroupDeleteJob(const QString &groupId, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->groupsIds << groupId;
}

ContactsGroupDeleteJob::~ContactsGroupDeleteJob()
{
    delete d;
}

void ContactsGroupDeleteJob::start()
{
    if (d->groupsIds.atEnd()) {
        emitResult();
        return;
    }

    const QString groupId = d->groupsIds.current();
    const QUrl url = ContactsService::removeGroupUrl(account()->accountName(), groupId);
    const QNetworkRequest request = d->createRequest(url);

    enqueueRequest(request);
}

void ContactsGroupDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->groupsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}



