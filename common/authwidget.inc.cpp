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

AuthWidget::AuthWidget(QWidget* parent):
    QWidget(parent),
    d(new Private(this))
{

}

AuthWidget::~AuthWidget()
{

}

void AuthWidget::setUsername(const QString& username)
{
    d->username = username;
}

void AuthWidget::setPassword(const QString& password)
{
    d->password = password;
}

void AuthWidget::clearCredentials()
{
    d->username.clear();
    d->password.clear();
}

void AuthWidget::setAccount(const AccountPtr& account)
{
    d->account = account;
}

void AuthWidget::setShowProgressBar(bool showProgressBar)
{
    d->showProgressBar = showProgressBar;

    if (showProgressBar && d->progress == UserLogin) {
        d->progressbar->setVisible(true);
    } else {
        d->progressbar->setVisible(false);
    }
}

bool AuthWidget::getShowProgressBar() const
{
    return d->showProgressBar;
}

AuthWidget::Progress AuthWidget::getProgress() const
{
    return d->progress;
}

void AuthWidget::authenticate()
{
    Q_ASSERT(!d->apiKey.isEmpty());

    if (d->account.isNull()) {
        Q_EMIT error(InvalidAccount, i18n("Invalid account"));
        return;
    }
    if (d->account->scopes().isEmpty()) {
        Q_EMIT error(InvalidAccount, i18n("No scopes to authenticate for"));
        return;
    }

    QStringList scopes;
    Q_FOREACH(const QUrl & scope, d->account->scopes()) {
        scopes << scope.toString();
    }

    KUrl url("https://accounts.google.com/o/oauth2/auth");
    url.addQueryItem(QLatin1String("client_id"), d->apiKey);
    url.addQueryItem(QLatin1String("redirect_uri"), QLatin1String("urn:ietf:wg:oauth:2.0:oob"));
    url.addQueryItem(QLatin1String("scope"), scopes.join(QLatin1String(" ")));
    url.addQueryItem(QLatin1String("response_type"), QLatin1String("code"));

    KGAPIDebugRawData() << "Requesting new token:" << url;

    d->webview->setVisible(true);
    if (d->showProgressBar) {
        d->progressbar->setVisible(true);
    }

    d->webview->setUrl(url);
    d->setProgress(AuthWidget::UserLogin);
}
