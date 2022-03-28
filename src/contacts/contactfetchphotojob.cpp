/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactfetchphotojob.h"
#include "account.h"
#include "contact.h"
#include "contactsservice.h"
#include "private/queuehelper_p.h"

#include <QNetworkReply>
#include <QNetworkRequest>

Q_DECLARE_METATYPE(KGAPI2::ContactPtr)

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactFetchPhotoJob::Private
{
public:
    Private(ContactFetchPhotoJob *parent);
    void processNextContact();

    QueueHelper<ContactPtr> contacts;

private:
    ContactFetchPhotoJob *const q;
};

ContactFetchPhotoJob::Private::Private(ContactFetchPhotoJob *parent)
    : q(parent)
{
}

void ContactFetchPhotoJob::Private::processNextContact()
{
    if (contacts.atEnd()) {
        q->emitFinished();
        return;
    }

    const ContactPtr contact = contacts.current();
    const QUrl url = ContactsService::photoUrl(q->account()->accountName(), contact->uid());
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());
    q->enqueueRequest(request);
}

ContactFetchPhotoJob::ContactFetchPhotoJob(const ContactsList &contacts, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->contacts = contacts;
}

ContactFetchPhotoJob::ContactFetchPhotoJob(const ContactPtr &contact, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->contacts << contact;
}

ContactFetchPhotoJob::~ContactFetchPhotoJob()
{
    delete d;
}

void ContactFetchPhotoJob::start()
{
    d->processNextContact();
}

void ContactFetchPhotoJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    if (reply->error() == QNetworkReply::ContentNotFoundError || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == NotFound) {
        d->contacts.currentProcessed();
        d->processNextContact();
        // If the last photo failed, make sure we don't fail the whole job!
        setError(KGAPI2::NoError);
        setErrorString(QString());
        return;
    }

    ContactPtr contact = d->contacts.current();
    KContacts::Picture picture;
    picture.setRawData(rawData, reply->header(QNetworkRequest::ContentTypeHeader).toString());
    contact->setPhoto(picture);
    Q_EMIT photoFetched(this, contact);

    d->contacts.currentProcessed();
    d->processNextContact();
}
