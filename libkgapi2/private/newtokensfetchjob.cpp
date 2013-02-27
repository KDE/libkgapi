/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "newtokensfetchjob_p.h"
#include "debug.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <qjson/parser.h>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class NewTokensFetchJob::Private
{
  public:
    QString tmpToken;
    QString apiKey;
    QString secretKey;

    QString accessToken;
    QString refreshToken;
};

NewTokensFetchJob::NewTokensFetchJob(const QString &tmpToken, const QString &apiKey, const QString &secretKey, QObject *parent):
    Job(parent),
    d(new Private)
{
    d->tmpToken = tmpToken;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
}

NewTokensFetchJob::~NewTokensFetchJob()
{
    delete d;
}

QString NewTokensFetchJob::accessToken() const
{
    if (isRunning()) {
        kWarning() << "Called accessToken() on running job!";
        return QString();
    }

    return d->accessToken;
}

QString NewTokensFetchJob::refreshToken() const
{
    if (isRunning()) {
        kWarning() << "Called refreshToken() on running job!";
        return QString();
    }

    return d->refreshToken;
}

void NewTokensFetchJob::start()
{
    QNetworkRequest request;

    request.setUrl(QUrl(QLatin1String("https://accounts.google.com/o/oauth2/token")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

    QUrl params;
    params.addQueryItem(QLatin1String("client_id"), d->apiKey);
    params.addQueryItem(QLatin1String("client_secret"), d->secretKey);
    params.addQueryItem(QLatin1String("code"), d->tmpToken);
    params.addQueryItem(QLatin1String("redirect_uri"), QLatin1String("urn:ietf:wg:oauth:2.0:oob"));
    params.addQueryItem(QLatin1String("grant_type"), QLatin1String("authorization_code"));

    enqueueRequest(request, params.encodedQuery());
}

void NewTokensFetchJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    Q_UNUSED(contentType);

    accessManager->post(request, data);
}

void NewTokensFetchJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply);

    QJson::Parser parser;
    bool ok;

    QVariantMap parsed_data = parser.parse(rawData, &ok).toMap();
    if (!ok) {
        KGAPIDebug() << "Failed to parse server response.";
        KGAPIDebugRawData() << rawData;
        setError(KGAPI2::AuthCancelled);
        setErrorString(i18n("Failed to parse server response."));
        return;
    }

    KGAPIDebugRawData() << "Retrieved new tokens pair:" << parsed_data;

    d->accessToken = parsed_data.value(QLatin1String("access_token")).toString();
    d->refreshToken = parsed_data.value(QLatin1String("refresh_token")).toString();
}

#include "newtokensfetchjob_p.moc"
