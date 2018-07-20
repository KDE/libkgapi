/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());
    request.setUrl(url);

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



