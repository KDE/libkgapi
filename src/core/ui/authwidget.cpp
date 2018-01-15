/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "authwidget.h"
#include "authwidget_p.h"
#include "../../debug.h"

#include <QTcpServer>

using namespace KGAPI2;


AuthWidget::AuthWidget(QWidget* parent):
    QWidget(parent),
    d(new AuthWidgetPrivate(this))
{

}

AuthWidget::~AuthWidget()
{
    delete d;
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
        Q_EMIT error(InvalidAccount, tr("Invalid account"));
        return;
    }
    if (d->account->scopes().isEmpty()) {
        Q_EMIT error(InvalidAccount, tr("No scopes to authenticate for"));
        return;
    }

    QStringList scopes;
    scopes.reserve(d->account->scopes().size());
    Q_FOREACH(const QUrl & scope, d->account->scopes()) {
        scopes << scope.toString();
    }

    d->server = new QTcpServer(this);
    if (! d->server->listen(QHostAddress::LocalHost)) {
        Q_EMIT error(InvalidAccount, tr("Could not start oauth http server"));
        return;
    }
    d->serverPort = d->server->serverPort();
    connect(d->server, &QTcpServer::newConnection, [&]() {
        d->connection = d->server->nextPendingConnection();
        d->connection->setParent(this);
        connect(d->connection, &QTcpSocket::readyRead, d, &AuthWidgetPrivate::socketReady);
        d->server->close();
        delete d->server;
    });
    QUrl url(QStringLiteral("https://accounts.google.com/o/oauth2/auth"));
    url.addQueryItem(QStringLiteral("client_id"), d->apiKey);
    url.addQueryItem(QStringLiteral("redirect_uri"), QStringLiteral("http://127.0.0.1:%1").arg(d->serverPort));
    url.addQueryItem(QStringLiteral("scope"), scopes.join(QStringLiteral(" ")));
    url.addQueryItem(QStringLiteral("response_type"), QStringLiteral("code"));

    qCDebug(KGAPIRaw) << "Requesting new token:" << url;

    d->sslIndicator->setVisible(true);
    d->urlEdit->setVisible(true);
    d->webview->setVisible(true);
    if (d->showProgressBar) {
        d->progressbar->setVisible(true);
    }
    d->webview->setUrl(url);
    d->setProgress(AuthWidget::UserLogin);
}
