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
    explicit Private(PersonDeleteJob *parent);
    void processNextPerson();

    QueueHelper<QString> peopleResourceNames;

private:
    PersonDeleteJob * const q;
};

PersonDeleteJob::Private::Private(PersonDeleteJob *parent)
    : q(parent)
{
}

void PersonDeleteJob::Private::processNextPerson()
{
    if (peopleResourceNames.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto person = peopleResourceNames.current();
    const auto deleteUrl = PeopleService::deleteContactUrl(person);
    QNetworkRequest request(deleteUrl);
    request.setRawHeader("Host", "people.googleapis.com");
    q->enqueueRequest(request);
}

PersonDeleteJob::PersonDeleteJob(const QString &peopleResourceName, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->peopleResourceNames << peopleResourceName;
}

PersonDeleteJob::PersonDeleteJob(const QStringList &peopleResourceNames, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->peopleResourceNames = peopleResourceNames;
}

PersonDeleteJob::PersonDeleteJob(const PersonList &people, const AccountPtr &account, QObject* parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    QStringList peopleResourceNames;
    std::transform(people.cbegin(), people.cend(), std::back_inserter(peopleResourceNames), [](const PersonPtr &person) {
        return person->resourceName();
    });
    d->peopleResourceNames = peopleResourceNames;
}

PersonDeleteJob::PersonDeleteJob(const PersonPtr &person, const AccountPtr &account, QObject* parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->peopleResourceNames << person->resourceName();
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

    d->peopleResourceNames.currentProcessed();
    d->processNextPerson();
}

}
