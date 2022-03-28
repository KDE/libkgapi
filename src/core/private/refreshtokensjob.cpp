/*
 * This file is part of LibKGAPI
 *
 * SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "account.h"
#include "debug.h"
#include "job_p.h"
#include "refreshtokensjob_p.h"

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

using namespace KGAPI2;

namespace KGAPI2
{

class Q_DECL_HIDDEN RefreshTokensJob::Private
{
public:
    Private(const AccountPtr &account, const QString &apiKey, const QString &secretKey, RefreshTokensJob *qq)
        : mAccount(account)
        , mApiKey(apiKey)
        , mSecretKey(secretKey)
        , q(qq)
    {
    }

    void emitError(Error error, const QString &text)
    {
        q->setError(error);
        q->setErrorString(text);
        q->emitFinished();
    }

public:
    AccountPtr mAccount;
    QString mApiKey;
    QString mSecretKey;

private:
    RefreshTokensJob *const q;
};

}

RefreshTokensJob::RefreshTokensJob(const AccountPtr &account, const QString &apiKey, const QString &secretKey, QObject *parent)
    : Job(parent)
    , d(new Private(account, apiKey, secretKey, this))
{
}

RefreshTokensJob::~RefreshTokensJob() = default;

AccountPtr RefreshTokensJob::account() const
{
    return d->mAccount;
}

void RefreshTokensJob::start()
{
    QNetworkRequest request;
    request.setUrl(QUrl(QStringLiteral("https://accounts.google.com/o/oauth2/token")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    QUrlQuery params;
    params.addQueryItem(QStringLiteral("client_id"), d->mApiKey);
    params.addQueryItem(QStringLiteral("client_secret"), d->mSecretKey);
    params.addQueryItem(QStringLiteral("refresh_token"), d->mAccount->refreshToken());
    params.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("refresh_token"));

    qCDebug(KGAPIDebug) << "Requesting token refresh.";

    enqueueRequest(request, params.toString(QUrl::FullyEncoded).toLatin1());
}

void RefreshTokensJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)

    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (document.isNull()) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Failed to parse newly fetched tokens"));
        emitFinished();
        return;
    }

    QVariantMap map = document.toVariant().toMap();

    /* Expected structure:
     * {
     *  "access_token": "the_access_token",
     *  "token_type":"Bearer",
     *  "expires_in":3600
     * }
     */
    const qlonglong expiresIn = map.value(QStringLiteral("expires_in")).toLongLong();
    d->mAccount->setExpireDateTime(QDateTime::currentDateTime().addSecs(expiresIn));
    d->mAccount->setAccessToken(map.value(QStringLiteral("access_token")).toString());
    emitFinished();
}

void RefreshTokensJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    Q_UNUSED(contentType)

    accessManager->setCookieJar(new QNetworkCookieJar);
    accessManager->post(request, data);
}
