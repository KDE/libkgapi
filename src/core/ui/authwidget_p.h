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
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI_AUTHWIDGET_P_H
#define LIBKGAPI_AUTHWIDGET_P_H

#include <QObject>

#include "ui/authwidget.h"
#include "types.h"
#include "kgapicore_export.h"

#include <QLineEdit>
#include <QToolButton>
#include <QProgressBar>
#include <QAbstractSocket>

class QVBoxLayout;
class QLabel;
class QWebEngineView;
class QTcpServer;
class QTcpSocket;

namespace KGAPI2 {

class Job;

// Exported for tests, otherwise internal
class KGAPICORE_EXPORT AuthWidgetPrivate: public QObject {

    Q_OBJECT

  public:
    explicit AuthWidgetPrivate(AuthWidget *parent);
    virtual void setupUi();
    virtual void setUrl(const QUrl &url);
    virtual void setVisible(bool visible);

    ~AuthWidgetPrivate() override;

    bool showProgressBar;
    QString username;
    QString password;
    AccountPtr account;
    AuthWidget::Progress progress;

    QString apiKey;
    QString secretKey;

    QToolButton *sslIndicator = nullptr;
    QLineEdit *urlEdit = nullptr;
    QProgressBar *progressbar = nullptr;
    QVBoxLayout *vbox = nullptr;
    QWebEngineView *webview = nullptr;
    QLabel *label = nullptr;

    QTcpServer *server = nullptr;
    int serverPort = 0;
    QTcpSocket *connection = nullptr;

  private Q_SLOTS:
    void emitError(const KGAPI2::Error errCode, const QString &msg);
    void webviewUrlChanged(const QUrl &url);
    void webviewFinished(bool ok);

    void socketReady();
    void socketError(QAbstractSocket::SocketError error);
    void tokensReceived(KGAPI2::Job *job);
    void accountInfoReceived(KGAPI2::Job *job);

  private:
    void setProgress(AuthWidget::Progress progress);

    bool isGoogleHost(const QUrl &url) const { return url.host() == QLatin1String("accounts.google.com"); }
    bool isSigninPage(const QUrl &url) const { return url.path() == QLatin1String("/signin/oauth"); }
    bool isUsernameFrame(const QUrl &url) { return url.path() == QLatin1String("/signin/oauth/identifier"); }
    bool isPasswordFrame(const QUrl &url) { return url.path() == QLatin1String("/signin/v2/challenge/pwd"); }

    void setSslIcon(const QString &icon);

    AuthWidget *q;

    friend class AuthWidget;
};

} // namespace KGAPI2


#endif // LIBKGAPI_AUTHWIDGET_P_H
