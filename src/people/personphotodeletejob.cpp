/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "personphotodeletejob.h"
#include "peopleservice.h"
#include "person.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN PersonPhotoDeleteJob::Private
{
public:
    explicit Private(PersonPhotoDeleteJob *parent);
    void processPersonPhoto();

    QString personResourceName;
    QByteArray photoRawData;

private:
    PersonPhotoDeleteJob * const q;
};

PersonPhotoDeleteJob::Private::Private(PersonPhotoDeleteJob *parent)
    : q(parent)
{
}

void PersonPhotoDeleteJob::Private::processPersonPhoto()
{
    const auto deleteUrl = PeopleService::deleteContactPhotoUrl(personResourceName, PeopleService::allPersonFields());
    QNetworkRequest request(deleteUrl);
    request.setRawHeader("Host", "people.googleapis.com");
    q->enqueueRequest(request);
}

PersonPhotoDeleteJob::PersonPhotoDeleteJob(const QString &personResourceName, const AccountPtr &account, QObject* parent)
    : ModifyJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->personResourceName = personResourceName;
}

PersonPhotoDeleteJob::~PersonPhotoDeleteJob() = default;

void PersonPhotoDeleteJob::start()
{
    d->processPersonPhoto();
}

void PersonPhotoDeleteJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                           const QNetworkRequest &request,
                                           const QByteArray &data,
                                           const QString &contentType)
{
    Q_UNUSED(data);
    Q_UNUSED(contentType);

    QNetworkRequest r = request;
    if (!r.hasRawHeader("If-Match")) {
        r.setRawHeader("If-Match", "*");
    }

    accessManager->sendCustomRequest(r, "DELETE");
}

ObjectsList PersonPhotoDeleteJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
        const auto jsonObject = json.object();
        const auto personJson = jsonObject.value(QStringLiteral("person")).toObject();
        const auto person = Person::fromJSON(personJson);
        items << person;
    }

    emitFinished();
    return items;
}

}

#include "moc_personphotodeletejob.cpp"
