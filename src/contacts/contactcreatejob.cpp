/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactcreatejob.h"
#include "account.h"
#include "contact.h"
#include "contactsservice.h"
#include "debug.h"
#include "private/queuehelper_p.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactCreateJob::Private
{
public:
    Private(ContactCreateJob *parent);
    void processNextContact();
    void setPhoto(const KContacts::Picture &photo, const QString &uid);

    QueueHelper<ContactPtr> contacts;
    ContactPtr lastContact;
    QPair<QByteArray, QString> pendingPhoto;

private:
    ContactCreateJob *const q;
};

ContactCreateJob::Private::Private(ContactCreateJob *parent)
    : q(parent)
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
    rawData.prepend(
        "<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
        "xmlns:gd=\"http://schemas.google.com/g/2005\" "
        "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
        "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
        "term=\"http://schemas.google.com/contact/2008#contact\"/>");
    rawData.append("</atom:entry>");

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    q->enqueueRequest(request, rawData, QStringLiteral("application/atom+xml"));
}

void ContactCreateJob::Private::setPhoto(const KContacts::Picture &photo, const QString &uid)
{
    const QUrl url = ContactsService::photoUrl(q->account()->accountName(), uid);
    QNetworkRequest photoRequest(url);
    photoRequest.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("image/*"));
    pendingPhoto.first = photo.rawData();
    pendingPhoto.second = photo.type();
    q->enqueueRequest(photoRequest, pendingPhoto.first, QStringLiteral("modifyImage"));
}

ContactCreateJob::ContactCreateJob(const ContactsList &contacts, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->contacts = contacts;
}

ContactCreateJob::ContactCreateJob(const ContactPtr &contact, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
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

ObjectsList ContactCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;
    if (!reply->url().path().contains(QLatin1String("/photos/media/"))) {
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        ContentType ct = Utils::stringToContentType(contentType);
        if (ct == KGAPI2::JSON) {
            d->lastContact = ContactsService::JSONToContact(rawData);
            items << d->lastContact.dynamicCast<Object>();
        } else if (ct == KGAPI2::XML) {
            d->lastContact = ContactsService::XMLToContact(rawData);
            items << d->lastContact.dynamicCast<Object>();
        } else {
            setError(KGAPI2::InvalidResponse);
            setErrorString(tr("Invalid response content type"));
            emitFinished();
            return items;
        }
        if (!d->contacts.current()->photo().isEmpty()) {
            // current contact does not have uid populated
            d->setPhoto(d->contacts.current()->photo(), d->lastContact->uid());
            d->contacts.currentProcessed();
        } else {
            d->contacts.currentProcessed();
            d->processNextContact();
        }
    } else {
        if (d->lastContact) {
            KContacts::Picture picture;
            picture.setRawData(d->pendingPhoto.first, d->pendingPhoto.second);
            d->lastContact->setPhoto(picture);
            d->pendingPhoto.first.clear();
            d->pendingPhoto.second.clear();
        }
        // Enqueue next item or finish
        d->processNextContact();
    }
    return items;
}
