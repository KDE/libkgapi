/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "authjob.h"
#include "account.h"
#include "debug.h"
#include "job_p.h"
#include "private/fullauthenticationjob_p.h"
#include "private/refreshtokensjob_p.h"

using namespace KGAPI2;

// Used by fakeauthbrowser in tests
KGAPICORE_EXPORT uint16_t kgapiTcpAuthServerPort = 0;

class Q_DECL_HIDDEN AuthJob::Private
{
public:
    Private(AuthJob *qq)
        : q(qq)
    {
    }

    template<typename JobType>
    void jobFinished(Job *job)
    {
        if (job->error()) {
            q->setError(job->error());
            q->setErrorString(job->errorString());
        } else {
            account = static_cast<JobType *>(job)->account();
        }

        q->emitFinished();
    }

    AccountPtr account;
    QString apiKey;
    QString secretKey;
    QString username;

private:
    AuthJob *const q;
};

AuthJob::AuthJob(const AccountPtr &account, const QString &apiKey, const QString &secretKey, QObject *parent)
    : Job(parent)
    , d(new Private(this))
{
    d->account = account;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
}

AuthJob::~AuthJob() = default;

AccountPtr AuthJob::account() const
{
    return d->account;
}

void AuthJob::setUsername(const QString &username)
{
    d->username = username;
}

void AuthJob::setPassword(const QString & /*password*/)
{
}

void AuthJob::handleReply(const QNetworkReply * /*reply*/, const QByteArray & /*rawData*/)
{
    // Should never be called.
    Q_UNREACHABLE();
}

void AuthJob::dispatchRequest(QNetworkAccessManager * /*accessManager*/,
                              const QNetworkRequest & /*request*/,
                              const QByteArray & /*data*/,
                              const QString & /*contentType*/)
{
    // Should never be called.
    Q_UNREACHABLE();
}

void AuthJob::start()
{
    if (d->account->refreshToken().isEmpty() || (d->account->m_scopesChanged == true)) {
        d->account->addScope(Account::accountInfoEmailScopeUrl());

        auto job = new FullAuthenticationJob(d->account, d->apiKey, d->secretKey, this);
        job->setUsername(d->username);
        job->setServerPort(kgapiTcpAuthServerPort);
        connect(job, &Job::finished, this, [this](Job *job) {
            d->jobFinished<FullAuthenticationJob>(job);
        });
    } else {
        if (d->account->accountName().isEmpty()) {
            setError(KGAPI2::InvalidAccount);
            setErrorString(tr("Account name is empty"));
            emitFinished();
            return;
        }

        auto job = new RefreshTokensJob(d->account, d->apiKey, d->secretKey, this);
        connect(job, &Job::finished, this, [this](Job *job) {
            d->jobFinished<RefreshTokensJob>(job);
        });
    }
}

#include "moc_authjob.cpp"
