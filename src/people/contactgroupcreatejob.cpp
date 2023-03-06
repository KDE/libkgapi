/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 *
 */

#include "contactgroupcreatejob.h"
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

class Q_DECL_HIDDEN ContactGroupCreateJob::Private
{
public:
    explicit Private(ContactGroupCreateJob *parent);
    void processNextContactGroup();

    QueueHelper<ContactGroupPtr> groups;
    ContactGroupPtr lastGroup;

private:
    ContactGroupCreateJob * const q;
};

ContactGroupCreateJob::Private::Private(ContactGroupCreateJob *parent)
    : q(parent)
{
}

void ContactGroupCreateJob::Private::processNextContactGroup()
{
    if (groups.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto group = groups.current();
    static const auto createUrl = PeopleService::createContactGroupUrl();
    QNetworkRequest request(createUrl);
    request.setRawHeader("Host", "people.googleapis.com");

    const auto groupJson = group->toJSON().toObject();
    const QJsonObject rootObject {
        { QStringLiteral("contactGroup"), groupJson },
        { QStringLiteral("readGroupFields"), PeopleService::allContactGroupRecentlyCreatedAvailableFields() }
    };
    const auto rawData = QJsonDocument(rootObject).toJson();
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}


ContactGroupCreateJob::ContactGroupCreateJob(const ContactGroupList &contactGroups, const AccountPtr& account, QObject* parent)
    : CreateJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->groups = contactGroups;
}

ContactGroupCreateJob::ContactGroupCreateJob(const ContactGroupPtr &contactGroup, const AccountPtr &account, QObject* parent)
    : CreateJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->groups << contactGroup;
}

ContactGroupCreateJob::~ContactGroupCreateJob() = default;

void ContactGroupCreateJob::start()
{
    d->processNextContactGroup();
}

ObjectsList ContactGroupCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
