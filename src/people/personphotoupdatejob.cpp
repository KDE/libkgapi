/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "personphotoupdatejob.h"
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

class Q_DECL_HIDDEN PersonPhotoUpdateJob::Private
{
public:
    explicit Private(PersonPhotoUpdateJob *parent);
    void processPersonPhoto();

    QString personResourceName;
    QByteArray photoRawData;

private:
    PersonPhotoUpdateJob * const q;
};

PersonPhotoUpdateJob::Private::Private(PersonPhotoUpdateJob *parent)
    : q(parent)
{
}

void PersonPhotoUpdateJob::Private::processPersonPhoto()
{
    const auto modifyUrl = PeopleService::updateContactPhotoUrl(personResourceName);
    QNetworkRequest request(modifyUrl);
    request.setRawHeader("Host", "people.googleapis.com");

    const QJsonObject rootObject {
        {QStringLiteral("photoBytes"), QString::fromUtf8(photoRawData.toBase64())},
        {QStringLiteral("personFields"), PeopleService::allPersonFields()}
    };
    const QJsonDocument jsonDoc(rootObject);
    const auto rawData = jsonDoc.toJson();

    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

PersonPhotoUpdateJob::PersonPhotoUpdateJob(const QString &personResourceName, const QByteArray &photoRawData, const AccountPtr &account, QObject* parent)
    : ModifyJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->personResourceName = personResourceName;
    d->photoRawData = photoRawData;
}

PersonPhotoUpdateJob::~PersonPhotoUpdateJob() = default;

void PersonPhotoUpdateJob::start()
{
    d->processPersonPhoto();
}

void PersonPhotoUpdateJob::dispatchRequest(QNetworkAccessManager *accessManager,
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

ObjectsList PersonPhotoUpdateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
