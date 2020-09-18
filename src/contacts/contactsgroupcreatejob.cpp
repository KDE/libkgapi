/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactsgroupcreatejob.h"
#include "contactsgroup.h"
#include "contactsservice.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"
#include "common_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN ContactsGroupCreateJob::Private
{
public:
    QueueHelper<ContactsGroupPtr> groups;
};

ContactsGroupCreateJob::ContactsGroupCreateJob(const ContactsGroupsList& groups, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->groups = groups;
}

ContactsGroupCreateJob::ContactsGroupCreateJob(const ContactsGroupPtr& contactsGroup, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->groups.enqueue(contactsGroup);
}

ContactsGroupCreateJob::~ContactsGroupCreateJob() = default;

void ContactsGroupCreateJob::start()
{
    if (d->groups.atEnd()) {
        emitFinished();
        return;
    }

    const ContactsGroupPtr contact = d->groups.current();
    const QUrl url = ContactsService::createGroupUrl(account()->accountName());
    QNetworkRequest request(url);
    request.setRawHeader(headerGDataVersion, ContactsService::APIVersion().toLatin1());

    const QByteArray rawData = xmlGroupHeader + ContactsService::contactsGroupToXML(contact) + xmlFooter;

    enqueueRequest(request, rawData, mimeTypeApplicationAtomXml);
}

ObjectsList ContactsGroupCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << ContactsService::JSONToContactsGroup(rawData);
        d->groups.currentProcessed();
    } else if (ct == KGAPI2::XML) {
        items << ContactsService::XMLToContactsGroup(rawData);
        d->groups.currentProcessed();
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    start();

    return items;
}


