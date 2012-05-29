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

#ifndef AUTHWIDGET_P_H
#define AUTHWIDGET_P_H

#include <QObject>

#include "ui/authwidget.h"

#include <QProgressBar>
#include <QVBoxLayout>
#include <QWebView>
#include <QLabel>

class QNetworkReply;

namespace KGoogle {

class Reply;

class AuthWidgetPrivate: public QObject {

    Q_OBJECT

  public:
    explicit AuthWidgetPrivate(AuthWidget *parent);

    virtual ~AuthWidgetPrivate();

    bool showProgressBar;
    QString username;
    QString password;
    KGoogle::Account::Ptr account;
    AuthWidget::Progress progress;

    QProgressBar *progressbar;
    QVBoxLayout *vbox;
    QWebView *webview;
    QLabel *label;

  private Q_SLOTS:
    void emitError(const KGoogle::Error errCode, const QString &msg);
    void webviewUrlChanged(const QUrl &url);
    void webviewFinished();

    void networkRequestFinished(QNetworkReply *reply);
    void accountInfoReceived(KGoogle::Reply *reply);

  private:
    void setupUi();
    void setProgress(AuthWidget::Progress progress);

    AuthWidget *q_ptr;
    Q_DECLARE_PUBLIC(AuthWidget);
};

} /* namespace KGoogle */


#endif // AUTHWIDGET_P_H
