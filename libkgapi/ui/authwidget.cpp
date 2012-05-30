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

#include "authwidget.h"
#include "authwidget_p.h"
#include "auth.h"

#include <KDebug>


using namespace KGAPI;
using namespace KGAPI::Ui;

AuthWidget::AuthWidget(QWidget* parent):
    QWidget(parent),
    d_ptr(new AuthWidgetPrivate(this))
{

}

AuthWidget::~AuthWidget()
{

}

void AuthWidget::setUsername(const QString& username)
{
    Q_D(AuthWidget);
    d->username = username;
}

void AuthWidget::setPassword(const QString& password)
{
    Q_D(AuthWidget);
    d->password = password;
}

void AuthWidget::clearCredentials()
{
    Q_D(AuthWidget);
    d->username.clear();
    d->password.clear();
}

void AuthWidget::setAccount(Account::Ptr& account)
{
    Q_D(AuthWidget);
    d->account = account;
}

void AuthWidget::setShowProgressBar(bool showProgressBar)
{
    Q_D(AuthWidget);
    d->showProgressBar = showProgressBar;

    if (showProgressBar && d->progress == UserLogin) {
        d->progressbar->setVisible(true);
    } else {
        d->progressbar->setVisible(false);
    }
}

bool AuthWidget::getShowProgressBar() const
{
    return d_func()->showProgressBar;
}

AuthWidget::Progress AuthWidget::getProgress() const
{
    return d_func()->progress;
}

void AuthWidget::authenticate()
{
    Q_D(AuthWidget);

    if (d->account.isNull() || d->account->scopes().isEmpty()) {
        throw KGAPI::Exception::InvalidAccount();
        return;
    }

    QStringList scopes;
    Q_FOREACH(const QUrl & scope, d->account->scopes()) {
        scopes << scope.toString();
    }

    QUrl url("https://accounts.google.com/o/oauth2/auth");
    url.addQueryItem("client_id", KGAPI::Auth::instance()->apiKey());
    url.addQueryItem("redirect_uri", "urn:ietf:wg:oauth:2.0:oob");
    url.addQueryItem("scope", scopes.join(" "));
    url.addQueryItem("response_type", "code");

#ifdef DEBUG_RAWDATA
    kDebug() << "Requesting new token:" << url;
#endif

    d->webview->setVisible(true);
    if (d->showProgressBar) {
        d->progressbar->setVisible(true);
    }

    d->webview->setUrl(url);
    d->setProgress(AuthWidget::UserLogin);
}
