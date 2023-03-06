/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 *
 */

#include "contactgroupdeletejob.h"
#include "peopleservice.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN ContactGroupDeleteJob::Private
{
public:
    explicit Private(ContactGroupDeleteJob *parent);
    void processNextContactGroup();

    QueueHelper<QString> groupResourceNames;
    QString lastGroupResourceName;

private:
    ContactGroupDeleteJob * const q;
};

ContactGroupDeleteJob::Private::Private(ContactGroupDeleteJob *parent)
    : q(parent)
{
}

void ContactGroupDeleteJob::Private::processNextContactGroup()
{
    if (groupResourceNames.atEnd()) {
        q->emitFinished();
        return;
    }

    const auto groupResourceName = groupResourceNames.current();
    const auto deleteUrl = PeopleService::deleteContactGroupUrl(groupResourceName, false);
    QNetworkRequest request(deleteUrl);
    request.setRawHeader("Host", "people.googleapis.com");
    q->enqueueRequest(request);
}


ContactGroupDeleteJob::ContactGroupDeleteJob(const QStringList &contactGroupResourceNames, const AccountPtr& account, QObject* parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->groupResourceNames = contactGroupResourceNames;
}

ContactGroupDeleteJob::ContactGroupDeleteJob(const QString &contactGroupResourceName, const AccountPtr &account, QObject* parent)
    : DeleteJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->groupResourceNames << contactGroupResourceName;
}

ContactGroupDeleteJob::~ContactGroupDeleteJob() = default;

void ContactGroupDeleteJob::start()
{
    d->processNextContactGroup();
}

void ContactGroupDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply);
    Q_UNUSED(rawData);

    d->groupResourceNames.currentProcessed();
    d->processNextContactGroup();
}

}
