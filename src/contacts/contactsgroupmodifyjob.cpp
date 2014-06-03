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

#include "contactsgroupmodifyjob.h"
#include "contactsgroup.h"
#include "contactsservice.h"
#include "debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class ContactsGroupModifyJob::Private
{
  public:
    QueueHelper<ContactsGroupPtr> groups;
};

ContactsGroupModifyJob::ContactsGroupModifyJob(const ContactsGroupsList& groups, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->groups = groups;
}

ContactsGroupModifyJob::ContactsGroupModifyJob(const ContactsGroupPtr& group, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->groups << group;
}

ContactsGroupModifyJob::~ContactsGroupModifyJob()
{
    delete d;
}


void ContactsGroupModifyJob::start()
{
    if (d->groups.atEnd()) {
        emitFinished();
        return;
    }

    const ContactsGroupPtr group = d->groups.current();

    const QUrl url = ContactsService::updateGroupUrl(account()->accountName(), group->id());
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());
    request.setUrl(url);

    QByteArray rawData = ContactsService::contactsGroupToXML(group);
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
    KGAPIDebugRawData() << headers;

    enqueueRequest(request, rawData, QLatin1String("application/atom+xml"));
}

ObjectsList ContactsGroupModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
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
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    // Enqueue next item or finish
    start();

    return items;
}

#include "contactsgroupmodifyjob.moc"
