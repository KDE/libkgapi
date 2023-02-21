/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "personcreatejob.h"
#include "peopleservice.h"
#include "person.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <KContacts/Picture>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN PersonCreateJob::Private
{
public:
    Private(PersonCreateJob *parent);
    void processNextPerson();

    QueueHelper<PersonPtr> people;

private:
    PersonCreateJob * const q;
};

PersonCreateJob::Private::Private(PersonCreateJob *parent)
    : q(parent)
{
}

void PersonCreateJob::Private::processNextPerson()
{
    if (people.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto person = people.current();
    static const auto createUrl = PeopleService::createContactUrl();
    QNetworkRequest request(createUrl);
    request.setRawHeader("Host", "people.googleapis.com");

    const auto personJson = QJsonDocument(person->toJSON().toObject());
    const auto rawData = personJson.toJson();
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

PersonCreateJob::PersonCreateJob(const PersonList &people, const AccountPtr &account, QObject* parent)
    : CreateJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->people = people;
}

PersonCreateJob::PersonCreateJob(const PersonPtr &person, const AccountPtr &account, QObject* parent)
    : CreateJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->people << person;
}

PersonCreateJob::~PersonCreateJob() = default;

void PersonCreateJob::start()
{
    d->processNextPerson();
}

ObjectsList PersonCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
