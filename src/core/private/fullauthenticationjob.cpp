/*
 * This file is part of LibKGAPI
 *
 * SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "account.h"
#include "accountinfo/accountinfo.h"
#include "accountinfo/accountinfofetchjob.h"
#include "debug.h"
#include "fullauthenticationjob_p.h"
#include "newtokensfetchjob_p.h"

#include <QAbstractSocket>
#include <QDateTime>
#include <QDesktopServices>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include <QUrlQuery>
#include <memory>

using namespace KGAPI2;

namespace KGAPI2
{

class Q_DECL_HIDDEN FullAuthenticationJob::Private
{
public:
    Private(const AccountPtr &account, const QString &apiKey, const QString &secretKey, FullAuthenticationJob *qq)
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

    void socketError(QAbstractSocket::SocketError error)
    {
        if (mConnection) {
            mConnection->deleteLater();
        }
        qCDebug(KGAPIDebug) << "Socket error when receiving response:" << error;
        emitError(InvalidResponse, tr("Error receiving response: %1").arg(error));
    }

    void socketReady()
    {
        Q_ASSERT(mConnection);
        const QByteArray data = mConnection->readLine();
        const QString title = tr("Authentication successful");
        const QString text = tr("You can close this tab and return to the application now.");
        mConnection->write("HTTP/1.1 200 OK\n"
                           "Content-Type: text/html\n"
                           "\n"
                           "<!DOCTYPE><html>"
                           "<head><meta charset=\"UTF-8\"><title>" + title.toUtf8() + "</title></head>"
                           "<body><h1>" + text.toUtf8() + "</h1></body>"
                           "</html>\n");
        mConnection->flush();
        mConnection->deleteLater();
        qCDebug(KGAPIDebug) << "Got connection on socket";

        const auto line = data.split(' ');
        if (line.size() != 3 || line.at(0) != QByteArray("GET") || !line.at(2).startsWith(QByteArray("HTTP/1.1"))) {
            qCDebug(KGAPIDebug) << "Token response invalid";
            emitError(InvalidResponse, tr("Token response invalid"));
            return;
        }

        // qCDebug(KGAPIDebug) << "Receiving data on socket: " << data;
        const QUrl url(QString::fromLatin1(line.at(1)));
        const QUrlQuery query(url);
        const QString code = query.queryItemValue(QStringLiteral("code"));
        if (code.isEmpty()) {
            const QString error = query.queryItemValue(QStringLiteral("error"));
            if (!error.isEmpty()) {
                qCDebug(KGAPIDebug) << "Google has returned an error response:" << error;
                emitError(UnknownError, error);
            } else {
                qCDebug(KGAPIDebug) << "Could not extract token from HTTP answer";
                emitError(InvalidAccount, tr("Could not extract token from HTTP answer"));
            }
            return;
        }

        auto fetch = new KGAPI2::NewTokensFetchJob(code, mApiKey, mSecretKey, mServerPort);
        q->connect(fetch, &Job::finished, q, [this](Job *job) {
            tokensReceived(job);
        });
    }

    void tokensReceived(Job *job)
    {
        auto tokensFetchJob = qobject_cast<NewTokensFetchJob *>(job);
        if (tokensFetchJob->error()) {
            qCDebug(KGAPIDebug) << "Error when retrieving tokens:" << job->errorString();
            emitError(static_cast<Error>(job->error()), job->errorString());
            return;
        }

        mAccount->setAccessToken(tokensFetchJob->accessToken());
        mAccount->setRefreshToken(tokensFetchJob->refreshToken());
        mAccount->setExpireDateTime(QDateTime::currentDateTime().addSecs(tokensFetchJob->expiresIn()));
        tokensFetchJob->deleteLater();

        auto fetchJob = new KGAPI2::AccountInfoFetchJob(mAccount, q);
        q->connect(fetchJob, &Job::finished, q, [this](Job *job) {
            accountInfoReceived(job);
        });
        qCDebug(KGAPIDebug) << "Requesting AccountInfo";
    }

    void accountInfoReceived(Job *job)
    {
        if (job->error()) {
            qCDebug(KGAPIDebug) << "Error when retrieving AccountInfo:" << job->errorString();
            emitError(static_cast<Error>(job->error()), job->errorString());
            return;
        }

        const auto objects = qobject_cast<AccountInfoFetchJob *>(job)->items();
        Q_ASSERT(!objects.isEmpty());

        const auto accountInfo = objects.first().staticCast<AccountInfo>();
        mAccount->setAccountName(accountInfo->email());

        job->deleteLater();

        q->emitFinished();
    }

public:
    AccountPtr mAccount;
    QString mApiKey;
    QString mSecretKey;
    QString mUsername;

    std::unique_ptr<QTcpServer> mServer;
    QTcpSocket *mConnection = nullptr;
    uint16_t mServerPort = 0;

private:
    FullAuthenticationJob *const q;
};

} // namespace KGAPI2

FullAuthenticationJob::FullAuthenticationJob(const AccountPtr &account, const QString &apiKey, const QString &secretKey, QObject *parent)
    : Job(parent)
    , d(new Private(account, apiKey, secretKey, this))
{
}

FullAuthenticationJob::~FullAuthenticationJob() = default;

void FullAuthenticationJob::setServerPort(uint16_t port)
{
    d->mServerPort = port;
}

void FullAuthenticationJob::setUsername(const QString &username)
{
    d->mUsername = username;
}

AccountPtr FullAuthenticationJob::account() const
{
    return d->mAccount;
}

void FullAuthenticationJob::start()
{
    if (d->mAccount.isNull()) {
        d->emitError(InvalidAccount, tr("Invalid account"));
        return;
    }
    if (d->mAccount->scopes().isEmpty()) {
        d->emitError(InvalidAccount, tr("No scopes to authenticate for"));
        return;
    }

    QStringList scopes;
    scopes.reserve(d->mAccount->scopes().size());
    const auto scopesList = d->mAccount->scopes();
    for (const QUrl &scope : scopesList) {
        scopes << scope.toString();
    }

    d->mServer = std::make_unique<QTcpServer>();
    if (!d->mServer->listen(QHostAddress::LocalHost, d->mServerPort)) {
        d->emitError(InvalidAccount, tr("Could not start OAuth HTTP server"));
        return;
    }
    d->mServerPort = d->mServer->serverPort();
    connect(d->mServer.get(), &QTcpServer::acceptError, this, [this](QAbstractSocket::SocketError e) {
        d->socketError(e);
    });
    connect(d->mServer.get(), &QTcpServer::newConnection, this, [this]() {
        d->mConnection = d->mServer->nextPendingConnection();
        d->mConnection->setParent(this);
        connect(d->mConnection,
                static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::errorOccurred),
                this,
                [this](QAbstractSocket::SocketError e) {
                    d->socketError(e);
                });
        connect(d->mConnection, &QTcpSocket::readyRead, this, [this]() {
            d->socketReady();
        });
        d->mServer->close();
    });

    QUrl url(QStringLiteral("https://accounts.google.com/o/oauth2/auth"));
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("client_id"), d->mApiKey);
    query.addQueryItem(QStringLiteral("redirect_uri"), QStringLiteral("http://127.0.0.1:%1").arg(d->mServerPort));
    query.addQueryItem(QStringLiteral("scope"), scopes.join(QLatin1Char(' ')));
    query.addQueryItem(QStringLiteral("response_type"), QStringLiteral("code"));
    if (!d->mUsername.isEmpty()) {
        query.addQueryItem(QStringLiteral("login_hint"), d->mUsername);
    }
    url.setQuery(query);

    QDesktopServices::openUrl(url);
}

void FullAuthenticationJob::handleReply(const QNetworkReply * /*reply*/, const QByteArray & /*rawData*/)
{
    // This is never supposed to be called.
    Q_UNREACHABLE();
}

void FullAuthenticationJob::dispatchRequest(QNetworkAccessManager * /*accessManager*/,
                                            const QNetworkRequest & /*request*/,
                                            const QByteArray & /*data*/,
                                            const QString & /*contentType*/)
{
    // This is never supposed to be called.
    Q_UNREACHABLE();
}
