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

#include "contactcreatejob.h"
#include "contact.h"
#include "contactsservice.h"
#include "utils.h"
#include "../debug.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QBuffer>


using namespace KGAPI2;

class ContactCreateJob::Private
{
  public:
    Private(ContactCreateJob *parent);
    void processNextContact();

    QueueHelper<ContactPtr> contacts;

  private:
    ContactCreateJob * const q;
};

ContactCreateJob::Private::Private(ContactCreateJob *parent):
    q(parent)
{
}

void ContactCreateJob::Private::processNextContact()
{
    if (contacts.atEnd()) {
        q->emitFinished();
        return;
    }

    const ContactPtr contact = contacts.current();
    const QUrl url = ContactsService::createContactUrl(q->account()->accountName());
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());
    request.setUrl(url);

    QByteArray rawData = ContactsService::contactToXML(contact);
    rawData.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
                     "xmlns:gd=\"http://schemas.google.com/g/2005\" "
                     "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
                    "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
                     "term=\"http://schemas.google.com/contact/2008#contact\"/>");
    rawData.append("</atom:entry>");

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    q->enqueueRequest(request, rawData, QLatin1String("application/atom+xml"));

    if (!contact->photo().isEmpty()) {
        QNetworkRequest photoRequest;
        photoRequest.setUrl(ContactsService::photoUrl(q->account()->accountName(), contact->uid()));

        if (!contact->photo().isEmpty()) {
            photoRequest.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("image/*"));
            QImage image = contact->photo().data();
            QByteArray ba;
            QBuffer buffer(&ba);
            image.save(&buffer, "JPG", 100);

            q->enqueueRequest(photoRequest, ba, QLatin1String("modifyImage"));
        }
    }
}

ContactCreateJob::ContactCreateJob(const ContactsList& contacts, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->contacts = contacts;
}

ContactCreateJob::ContactCreateJob(const ContactPtr& contact, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->contacts << contact;
}

ContactCreateJob::~ContactCreateJob()
{
    delete d;
}

void ContactCreateJob::start()
{
    d->processNextContact();
}

void ContactCreateJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    QNetworkRequest r = request;

    if (contentType == QLatin1String("modifyImage")) {
        accessManager->put(r, data);
    } else {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
        accessManager->post(r, data);
    }
}

ObjectsList ContactCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << ContactsService::JSONToContact(rawData).dynamicCast<Object>();
        d->contacts.currentProcessed();
    } else if (ct == KGAPI2::XML) {
        items << ContactsService::XMLToContact(rawData).dynamicCast<Object>();
        d->contacts.currentProcessed();
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    // Enqueue next item or finish
    d->processNextContact();

    return items;
}


