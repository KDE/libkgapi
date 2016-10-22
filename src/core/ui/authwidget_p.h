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

#ifndef LIBKGAPI_AUTHWIDGET_P_H
#define LIBKGAPI_AUTHWIDGET_P_H

#include <QObject>

#include "ui/authwidget.h"
#include "types.h"

#include <QProgressBar>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QLabel>

namespace KGAPI2 {

class Job;

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent=0);
    ~WebView();

protected:
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
};


class Q_DECL_HIDDEN AuthWidget::Private: public QObject {

    Q_OBJECT

  public:
    explicit Private(AuthWidget *parent);

    virtual ~Private();

    bool showProgressBar;
    QString username;
    QString password;
    AccountPtr account;
    AuthWidget::Progress progress;

    QString apiKey;
    QString secretKey;

    QProgressBar *progressbar;
    QVBoxLayout *vbox;
    WebView *webview;
    QLabel *label;

  private Q_SLOTS:
    void emitError(const KGAPI2::Error errCode, const QString &msg);
    void webviewUrlChanged(const QUrl &url);
    void webviewFinished(bool ok);

    void tokensReceived(KGAPI2::Job *job);
    void accountInfoReceived(KGAPI2::Job *job);

  private:
    void setupUi();
    void setProgress(AuthWidget::Progress progress);

    AuthWidget *q;

    friend class AuthWidget;
};

} // namespace KGAPI2


#endif // LIBKGAPI_AUTHWIDGET_P_H
