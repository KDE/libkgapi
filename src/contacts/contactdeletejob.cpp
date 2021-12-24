/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactdeletejob.h"
#include "contact.h"
#include "contactsservice.h"
#include "debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactDeleteJob::Private
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
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    q->enqueueRequest(request);
}

ContactDeleteJob::ContactDeleteJob(const ContactsList& contacts, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->contactIds.reserve(contacts.size());
    for (const ContactPtr &contact : contacts) {
        d->contactIds <<  contact->uid();
    }
}

ContactDeleteJob::ContactDeleteJob(const ContactPtr& contact, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->contactIds << contact->uid();
}

ContactDeleteJob::ContactDeleteJob(const QStringList &contactIds, const AccountPtr &account, QObject *parent):
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



