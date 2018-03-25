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

#include "contactfetchphotojob.h"
#include "account.h"
#include "contact.h"
#include "contactsservice.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QImage>

Q_DECLARE_METATYPE(KGAPI2::ContactPtr)

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactFetchPhotoJob::Private
{
  public:
    Private(ContactFetchPhotoJob *parent);
    void processNextContact();

    QueueHelper<ContactPtr> contacts;

  private:
    ContactFetchPhotoJob * const q;
};

ContactFetchPhotoJob::Private::Private(ContactFetchPhotoJob *parent):
    q(parent)
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
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());
    q->enqueueRequest(request);
}


ContactFetchPhotoJob::ContactFetchPhotoJob(const ContactsList &contacts, const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->contacts = contacts;
}

ContactFetchPhotoJob::ContactFetchPhotoJob(const ContactPtr &contact, const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
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
    if (reply->error() == QNetworkReply::ContentNotFoundError
        || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == NotFound) {
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


