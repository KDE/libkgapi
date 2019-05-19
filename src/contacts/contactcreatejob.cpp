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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "contactcreatejob.h"
#include "contact.h"
#include "contactsservice.h"
#include "utils.h"
#include "../debug.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QBuffer>
#include <QImage>

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactCreateJob::Private
{
  public:
    Private(ContactCreateJob *parent);
    void processNextContact();

    QueueHelper<ContactPtr> contacts;
    ContactPtr lastContact;
    QPair<QByteArray, QString> pendingPhoto;
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
        if (pendingPhoto.first.isEmpty()) {
            q->emitFinished();
        }
        return;
    }

    const ContactPtr contact = contacts.current();
    const QUrl url = ContactsService::createContactUrl(q->account()->accountName());
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());

    QByteArray rawData = ContactsService::contactToXML(contact);
    rawData.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
                     "xmlns:gd=\"http://schemas.google.com/g/2005\" "
                     "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
                    "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
                     "term=\"http://schemas.google.com/contact/2008#contact\"/>");
    rawData.append("</atom:entry>");

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    q->enqueueRequest(request, rawData, QStringLiteral("application/atom+xml"));

    if (!contact->photo().isEmpty()) {
        const QUrl url = ContactsService::photoUrl(q->account()->accountName(), contact->uid());
        QNetworkRequest photoRequest(url);
        photoRequest.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("image/*"));
        pendingPhoto.first = contact->photo().rawData();
        pendingPhoto.second = contact->photo().type();
        q->enqueueRequest(photoRequest, pendingPhoto.first, QStringLiteral("modifyImage"));
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
    ObjectsList items;
    if (!reply->url().path().contains(QLatin1String("/photos/media/"))) {
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        ContentType ct = Utils::stringToContentType(contentType);
        if (ct == KGAPI2::JSON) {
            d->lastContact = ContactsService::JSONToContact(rawData);
            items << d->lastContact.dynamicCast<Object>();
            d->contacts.currentProcessed();
        } else if (ct == KGAPI2::XML) {
            d->lastContact = ContactsService::XMLToContact(rawData);
            items << d->lastContact.dynamicCast<Object>();
            d->contacts.currentProcessed();
        } else {
            setError(KGAPI2::InvalidResponse);
            setErrorString(tr("Invalid response content type"));
            emitFinished();
            return items;
        }
    } else {
        if (d->lastContact) {
            KContacts::Picture picture;
            picture.setRawData(d->pendingPhoto.first, d->pendingPhoto.second);
            d->lastContact->setPhoto(picture);
            d->pendingPhoto.first.clear();
            d->pendingPhoto.second.clear();
        }
    }

    // Enqueue next item or finish
    d->processNextContact();

    return items;
}


