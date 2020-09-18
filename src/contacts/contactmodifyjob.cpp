/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactmodifyjob.h"
#include "contactsservice.h"
#include "contact.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QBuffer>
#include <QImage>

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactModifyJob::Private
{
  public:
    Private(ContactModifyJob *parent);
    void processNextContact();
    void updatePhoto(const ContactPtr &contact);

    QueueHelper<ContactPtr> contacts;
    ContactPtr lastContact;
    QPair<QByteArray, QString> pendingPhoto;
  private:
    ContactModifyJob *const q;
};

ContactModifyJob::Private::Private(ContactModifyJob *parent):
    q(parent)
{
}

void ContactModifyJob::Private::processNextContact()
{
    if (contacts.atEnd()) {
        if (pendingPhoto.first.isEmpty()) {
            q->emitFinished();
        }
        return;
    }

    const ContactPtr contact = contacts.current();

    const QUrl url = ContactsService::updateContactUrl(q->account()->accountName(), contact->uid());
    QNetworkRequest request(url);

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
}

void ContactModifyJob::Private::updatePhoto(const ContactPtr &contact)
{
    const QUrl photoUrl = ContactsService::photoUrl(q->account()->accountName(), contact->uid());
    QNetworkRequest photoRequest(photoUrl);
    if (!contact->photo().isEmpty()) {
        photoRequest.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("image/*"));
        pendingPhoto.first = contact->photo().rawData();
        pendingPhoto.second = contact->photo().type();
        q->enqueueRequest(photoRequest, pendingPhoto.first, QStringLiteral("modifyImage"));
    } else {
        q->enqueueRequest(photoRequest, QByteArray(), QStringLiteral("deleteImage"));
    }
}


ContactModifyJob::ContactModifyJob(const ContactsList& contacts, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->contacts = contacts;
}

ContactModifyJob::ContactModifyJob(const ContactPtr& contact, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->contacts.enqueue(contact);
}

ContactModifyJob::~ContactModifyJob()
{
    delete d;
}

void ContactModifyJob::start()
{
    d->processNextContact();
}

void ContactModifyJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    QNetworkRequest r = request;
    r.setRawHeader("If-Match", "*");
    r.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());

    if (contentType == QLatin1String("modifyImage")) {
        accessManager->put(r, data);
    } else if (contentType == QLatin1String("deleteImage")) {
        // Delete requests have no response body so there isn't anything to pretty print
        QUrl cleanedUrl = r.url();
        QUrlQuery cleanedQuery(cleanedUrl);
        cleanedQuery.removeAllQueryItems(Job::StandardParams::PrettyPrint);
        cleanedUrl.setQuery(cleanedQuery);
        r.setUrl(cleanedUrl);

        accessManager->deleteResource(r);
    } else {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
        accessManager->put(r, data);
    }
}

ObjectsList ContactModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    ObjectsList items;
    if (!reply->url().path().contains(QLatin1String("/photos/media/"))) {
        const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        ContentType ct = Utils::stringToContentType(contentType);
        if (ct == KGAPI2::JSON) {
            d->lastContact = ContactsService::JSONToContact(rawData);
            items << d->lastContact;
        } else if (ct == KGAPI2::XML) {
            d->lastContact = ContactsService::XMLToContact(rawData);
            items << d->lastContact;
        } else {
            setError(KGAPI2::InvalidResponse);
            setErrorString(tr("Invalid response content type"));
            emitFinished();
            return items;
        }
        d->updatePhoto(d->contacts.current());
        d->contacts.currentProcessed();
    } else {
        if (d->lastContact && !d->pendingPhoto.first.isEmpty()) {
            KContacts::Picture picture;
            picture.setRawData(d->pendingPhoto.first, d->pendingPhoto.second);
            d->lastContact->setPhoto(picture);
            d->pendingPhoto.first.clear();
            d->pendingPhoto.second.clear();
        }

        // Enqueue next item and picture or finish
        d->processNextContact();
    }

    return items;
}


