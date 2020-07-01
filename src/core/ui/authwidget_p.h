/*
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
