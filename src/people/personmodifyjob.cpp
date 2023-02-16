/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "personmodifyjob.h"
#include "peopleservice.h"
#include "person.h"
#include "../debug.h"
#include "qbuffer.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN PersonModifyJob::Private
{
public:
    Private(PersonModifyJob *parent);
    void processNextPerson();

    QueueHelper<PersonPtr> people;

private:
    PersonModifyJob * const q;
};

PersonModifyJob::Private::Private(PersonModifyJob *parent)
    : q(parent)
{
}

void PersonModifyJob::Private::processNextPerson()
{
    if (people.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto person = people.current();
    static const auto createUrl = PeopleService::updateContactUrl(person->resourceName(), PeopleService::allUpdatablePersonFields());
    QNetworkRequest request(createUrl);
    request.setRawHeader("Host", "people.googleapis.com");

    const auto personJson = QJsonDocument(person->toJSON().toObject());
    const auto rawData = personJson.toJson();
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

PersonModifyJob::PersonModifyJob(const PersonList &people, const AccountPtr &account, QObject* parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->people = people;
}

PersonModifyJob::PersonModifyJob(const PersonPtr &person, const AccountPtr &account, QObject* parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->people << person;
}

PersonModifyJob::~PersonModifyJob() = default;

void PersonModifyJob::start()
{
    d->processNextPerson();
}

void PersonModifyJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                      const QNetworkRequest &request,
                                      const QByteArray &data,
                                      const QString &contentType)
{
    QNetworkRequest r = request;
    if (!r.hasRawHeader("Content-Type")) {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    if (!r.hasRawHeader("If-Match")) {
        r.setRawHeader("If-Match", "*");
    }

    accessManager->sendCustomRequest(r, "PATCH", data);
}

ObjectsList PersonModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
        const auto person = Person::fromJSON(json.object());
        items << person;
    }

    d->people.currentProcessed();
    d->processNextPerson();

    return items;
}

}
