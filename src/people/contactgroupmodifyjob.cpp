/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactgroupmodifyjob.h"
#include "contactgroup.h"
#include "peopleservice.h"
#include "private/queuehelper_p.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN ContactGroupModifyJob::Private
{
public:
    explicit Private(ContactGroupModifyJob *parent);
    void processNextContactGroup();

    QueueHelper<ContactGroupPtr> groups;
    ContactGroupPtr lastGroup;

private:
    ContactGroupModifyJob * const q;
};

ContactGroupModifyJob::Private::Private(ContactGroupModifyJob *parent)
    : q(parent)
{
}

void ContactGroupModifyJob::Private::processNextContactGroup()
{
    if (groups.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto group = groups.current();
    const auto modifyUrl = PeopleService::updateContactGroupUrl(group->resourceName());
    QNetworkRequest request(modifyUrl);
    request.setRawHeader("Host", "people.googleapis.com");

    const auto groupJson = group->toJSON().toObject();
    const QJsonObject rootObject {
        { QStringLiteral("contactGroup"), groupJson }
    };
    const auto rawData = QJsonDocument(rootObject).toJson();
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ContactGroupModifyJob::ContactGroupModifyJob(const ContactGroupList &contactGroups, const AccountPtr &account, QObject* parent)
    : ModifyJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->groups = contactGroups;
}

ContactGroupModifyJob::ContactGroupModifyJob(const ContactGroupPtr &group, const AccountPtr &account, QObject* parent)
    : ModifyJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->groups << group;
}

ContactGroupModifyJob::~ContactGroupModifyJob() = default;

void ContactGroupModifyJob::start()
{
    d->processNextContactGroup();
}

ObjectsList ContactGroupModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const auto contentTypeString = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    const auto contentType = Utils::stringToContentType(contentTypeString);

    if (contentType != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return {};
    }

    ObjectsList items;
    const auto json = QJsonDocument::fromJson(rawData);
    if (json.isObject()) {
        const auto group = ContactGroup::fromJSON(json.object());
        items << group;
    }

    d->groups.currentProcessed();
    d->processNextContactGroup();

    return items;
}

}

#include "moc_contactgroupmodifyjob.cpp"
