/*
 
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "newtokensfetchjob_p.h"
#include "../debug.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QUrlQuery>

using namespace KGAPI2;

class Q_DECL_HIDDEN NewTokensFetchJob::Private
{
  public:
    QString tmpToken;
    QString apiKey;
    QString secretKey;
    int localPort = 0;

    QString accessToken;
    QString refreshToken;
    qulonglong expiresIn;
};

NewTokensFetchJob::NewTokensFetchJob(const QString &tmpToken, const QString &apiKey, const QString &secretKey, int localPort, QObject *parent):
    Job(parent),
    d(new Private)
{
    d->tmpToken = tmpToken;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
    d->localPort = localPort;
}

NewTokensFetchJob::~NewTokensFetchJob()
{
    delete d;
}

QString NewTokensFetchJob::accessToken() const
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called accessToken() on running job!";
        return QString();
    }

    return d->accessToken;
}

QString NewTokensFetchJob::refreshToken() const
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called refreshToken() on running job!";
        return QString();
    }

    return d->refreshToken;
}

qulonglong NewTokensFetchJob::expiresIn() const
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called expiresIn() on running job!";
        return 0;
    }

    return d->expiresIn;
}

void NewTokensFetchJob::start()
{
    QNetworkRequest request;

    request.setUrl(QUrl(QStringLiteral("https://accounts.google.com/o/oauth2/token")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

    QUrlQuery params;
    params.addQueryItem(QStringLiteral("client_id"), d->apiKey);
    params.addQueryItem(QStringLiteral("client_secret"), d->secretKey);
    params.addQueryItem(QStringLiteral("code"), d->tmpToken);
    params.addQueryItem(QStringLiteral("redirect_uri"), QStringLiteral("http://127.0.0.1:%1").arg(d->localPort)); // we need to use the same URL as in AuthWidget
    params.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("authorization_code"));

    enqueueRequest(request, params.toString(QUrl::FullyEncoded).toLatin1());
}

void NewTokensFetchJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    Q_UNUSED(contentType);

    accessManager->post(request, data);
}

void NewTokensFetchJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply);

    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        qCWarning(KGAPIDebug) << "Failed to parse server response.";
        qCDebug(KGAPIDebug) << rawData;
        setError(KGAPI2::AuthCancelled);
        setErrorString(tr("Failed to parse server response."));
        return;
    }
    QVariantMap parsed_data = document.toVariant().toMap();

    qCDebug(KGAPIDebug) << "Retrieved new tokens pair.";

    d->accessToken = parsed_data.value(QStringLiteral("access_token")).toString();
    d->refreshToken = parsed_data.value(QStringLiteral("refresh_token")).toString();
    d->expiresIn = parsed_data.value(QStringLiteral("expires_in")).toULongLong();
}


