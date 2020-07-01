/*
    SPDX-FileCopyrightText: 2012, 2013 Daniel Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "account.h"
#include "utils_p.h"

#include <QDateTime>

using namespace KGAPI2;

class Q_DECL_HIDDEN Account::Private
{
  public:
    Private();
    Private(const Private &other);

    QString accName;
    QString accessToken;
    QString refreshToken;
    QDateTime expireDateTime;
    QList< QUrl > scopes;
};

Account::Private::Private()
    : expireDateTime(QDateTime())
{ }


Account::Private::Private(const Private& other):
    accName(other.accName),
    accessToken(other.accessToken),
    refreshToken(other.refreshToken),
    expireDateTime(other.expireDateTime),
    scopes(other.scopes)
{ }


Account::Account():
    d(new Private),
    m_scopesChanged(false)
{ }


Account::Account(const QString &accName, const QString &accessToken,
                 const QString &refreshToken, const QList< QUrl > &scopes):
    d(new Private),
    m_scopesChanged(false)
{
    d->accName = accName;
    d->accessToken = accessToken;
    d->refreshToken = refreshToken;
    d->scopes = scopes;
}

Account::Account(const Account& other):
    d(new Private(*(other.d))),
    m_scopesChanged(other.m_scopesChanged)
{ }


Account::~Account()
{
    delete d;
}

bool Account::operator==(const Account &other) const
{
    if (d == other.d) {
        return true;
    }

    GAPI_COMPARE(accName)
    GAPI_COMPARE(accessToken)
    GAPI_COMPARE(refreshToken)
    GAPI_COMPARE(expireDateTime)
    GAPI_COMPARE(scopes)

    return true;
}

QString Account::accountName() const
{
    return d->accName;
}

void Account::setAccountName(const QString& accountName)
{
    d->accName = accountName;
}

QString Account::accessToken() const
{
    return d->accessToken;
}

void Account::setAccessToken(const QString &accessToken)
{
    d->accessToken = accessToken;
}

QString Account::refreshToken() const
{
    return d->refreshToken;
}

void Account::setRefreshToken(const QString &refreshToken)
{
    d->refreshToken = refreshToken;
}

QList< QUrl > Account::scopes() const
{
    return d->scopes;
}

void Account::setScopes(const QList< QUrl > &scopes)
{
    d->scopes = scopes;
    m_scopesChanged = true;
}

void Account::addScope(const QUrl &scope)
{
    if (!d->scopes.contains(scope)) {
        d->scopes.append(scope);
        m_scopesChanged = true;
    }
}

void Account::removeScope(const QUrl &scope)
{
    if (d->scopes.contains(scope)) {
        d->scopes.removeOne(scope);
        m_scopesChanged = true;
    }
}

QDateTime Account::expireDateTime() const
{
    return d->expireDateTime;
}

void Account::setExpireDateTime(const QDateTime &expire)
{
    d->expireDateTime = expire;
}

QUrl Account::accountInfoScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/userinfo.profile"));
}

QUrl Account::accountInfoEmailScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/userinfo.email"));
}

QUrl Account::calendarScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/calendar"));
}

QUrl Account::contactsScopeUrl()
{
    return QUrl(QStringLiteral("https://www.google.com/m8/feeds/"));
}

QUrl Account::latitudeScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/latitude.all.best"));
}

QUrl Account::tasksScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/tasks"));
}

QUrl Account::bloggerScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/blogger"));
}

QUrl Account::mailScopeUrl()
{
    return QUrl(QStringLiteral("https://mail.google.com/"));
}

QUrl Account::driveScopeUrl()
{
    return QUrl(QStringLiteral("https://www.googleapis.com/auth/drive"));
}
