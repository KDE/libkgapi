/*
    Copyright (C) 2012 - 2018  Daniel Vrátil <dvratil@kde.org>

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
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QUrlQuery>

using namespace KGAPI2;


AuthWidget::AuthWidget(QWidget* parent):
    QWidget(parent),
    d(new AuthWidgetPrivate(this))
{
    d->setupUi();
}

AuthWidget::AuthWidget(AuthWidgetPrivate *dptr, QWidget *parent)
    : QWidget(parent)
    , d(dptr)
{
    d->setupUi();
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
    if (!d->server->listen(QHostAddress::LocalHost, d->serverPort)) {
        Q_EMIT error(InvalidAccount, tr("Could not start oauth http server"));
        return;
    }
    connect(d->server, &QTcpServer::acceptError, d, &AuthWidgetPrivate::socketError);
    d->serverPort = d->server->serverPort();
    connect(d->server, &QTcpServer::newConnection, [&]() {
        d->connection = d->server->nextPendingConnection();
        d->connection->setParent(this);
        connect(d->connection, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>
            (&QAbstractSocket::error), d, &AuthWidgetPrivate::socketError);
        connect(d->connection, &QTcpSocket::readyRead, d, &AuthWidgetPrivate::socketReady);
        d->server->close();
        d->server->deleteLater();
    });

    QUrl url(QStringLiteral("https://accounts.google.com/o/oauth2/auth"));
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("client_id"), d->apiKey);
    query.addQueryItem(QStringLiteral("redirect_uri"), QStringLiteral("http://127.0.0.1:%1").arg(d->serverPort));
    query.addQueryItem(QStringLiteral("scope"), scopes.join(QStringLiteral(" ")));
    query.addQueryItem(QStringLiteral("response_type"), QStringLiteral("code"));
    url.setQuery(query);

    qCDebug(KGAPIRaw) << "Requesting new token:" << url;

    d->setVisible(true);
    d->setUrl(url);
    d->setProgress(AuthWidget::UserLogin);
}
