/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "persondeletejob.h"
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

class Q_DECL_HIDDEN PersonDeleteJob::Private
{
public:
    Private(PersonDeleteJob *parent);
    void processNextPerson();

    QueueHelper<PersonPtr> people;
    PersonPtr lastPerson;

private:
    PersonDeleteJob * const q;
};

PersonDeleteJob::Private::Private(PersonDeleteJob *parent)
    : q(parent)
{
}

void PersonDeleteJob::Private::processNextPerson()
{
    if (people.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto person = people.current();
    const auto deleteUrl = PeopleService::deleteContactUrl(person->resourceName());
    QNetworkRequest request(deleteUrl);
    request.setRawHeader("Host", "people.googleapis.com");

    const auto personJson = QJsonDocument(person->toJSON().toObject());
    const auto rawData = personJson.toJson();
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

PersonDeleteJob::PersonDeleteJob(const PersonList &people, const AccountPtr &account, QObject* parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->people = people;
}

PersonDeleteJob::PersonDeleteJob(const PersonPtr &person, const AccountPtr &account, QObject* parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->people << person;
}

PersonDeleteJob::~PersonDeleteJob() = default;

void PersonDeleteJob::start()
{
    d->processNextPerson();
}

void PersonDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply);
    Q_UNUSED(rawData);

    d->people.currentProcessed();
    d->processNextPerson();
}

}
