/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "account.h"

namespace KGAPI
{

class AccountPrivate: public QSharedData
{
  public:
    AccountPrivate();

    AccountPrivate(const AccountPrivate &other);

    virtual ~AccountPrivate();

    QString accName;
    QString accessToken;
    QString refreshToken;
    QList< QUrl > scopes;
};
}

using namespace KGAPI;

AccountPrivate::AccountPrivate():
    QSharedData()
{ }

AccountPrivate::AccountPrivate(const AccountPrivate& other):
    QSharedData(other),
    accName(other.accName),
    accessToken(other.accessToken),
    refreshToken(other.refreshToken),
    scopes(other.scopes)
{ }

AccountPrivate::~AccountPrivate()
{ }


Account::Account():
    d(new AccountPrivate()),
    m_scopesChanged(false)
{ }



Account::Account(const QString &accName, const QString &accessToken,
                 const QString &refreshToken, const QList< QUrl > &scopes):
    d(new AccountPrivate()),
    m_scopesChanged(false)
{
    d->accName = accName;
    d->accessToken = accessToken;
    d->refreshToken = refreshToken;
    d->scopes = scopes;
}

Account::Account(const Account& other):
    d(other.d),
    m_scopesChanged(other.m_scopesChanged)
{ }


Account::~Account()
{ }

QString Account::accountName() const
{
    return d->accName;
}

void Account::setAccountName(const QString& accountName)
{
    d->accName = accountName;
}

const QString &Account::accessToken() const
{
    return d->accessToken;
}

void Account::setAccessToken(const QString &accessToken)
{
    d->accessToken = accessToken;
}

const QString &Account::refreshToken() const
{
    return d->refreshToken;
}

void Account::setRefreshToken(const QString &refreshToken)
{
    d->refreshToken = refreshToken;
}

const QList< QUrl > &Account::scopes() const
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
