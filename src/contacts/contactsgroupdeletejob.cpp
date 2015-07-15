/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "contactsgroupdeletejob.h"
#include "contactsgroup.h"
#include "contactsservice.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class ContactsGroupDeleteJob::Private
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
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());
    request.setUrl(url);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    return request;
}


ContactsGroupDeleteJob::ContactsGroupDeleteJob(const ContactsGroupsList& groups, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH(const ContactsGroupPtr &group, groups) {
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
        emitFinished();
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



