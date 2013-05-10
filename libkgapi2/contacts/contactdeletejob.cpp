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

#include "contactdeletejob.h"
#include "contact.h"
#include "contactsservice.h"
#include "debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class ContactDeleteJob::Private
{
  public:
    Private(ContactDeleteJob *parent);
    void processNextContact();

    QueueHelper<QString> contactIds;

  private:
    ContactDeleteJob * const q;
};

ContactDeleteJob::Private::Private(ContactDeleteJob* parent):
    q(parent)
{
}

void ContactDeleteJob::Private::processNextContact()
{
    if (contactIds.atEnd()) {
        q->emitFinished();
        return;
    }

    const QString contactId = contactIds.current();
    const QUrl url = ContactsService::removeContactUrl(q->account()->accountName(), contactId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    q->enqueueRequest(request);
}

ContactDeleteJob::ContactDeleteJob(const ContactsList& contacts, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH(const ContactPtr &contact, contacts) {
        d->contactIds <<  contact->uid();
    }
}

ContactDeleteJob::ContactDeleteJob(const ContactPtr& contact, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->contactIds << contact->uid();
}

ContactDeleteJob::ContactDeleteJob(const QStringList contactIds, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->contactIds = contactIds;
}

ContactDeleteJob::ContactDeleteJob(const QString &contactId, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->contactIds << contactId;
}

ContactDeleteJob::~ContactDeleteJob()
{
    delete d;
}

void ContactDeleteJob::start()
{
    d->processNextContact();
}

void ContactDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    d->contactIds.currentProcessed();
    d->processNextContact();
}


#include "contactdeletejob.moc"
