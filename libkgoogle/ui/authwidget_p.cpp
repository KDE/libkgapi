/*
    LibKGoogle - AuthWidgetPrivate
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "authwidget_p.h"
#include "auth.h"
#include "accessmanager.h"
#include "request.h"
#include "reply.h"
#include "services/accountinfo.h"
#include "objects/accountinfo.h"

#include <QNetworkAccessManager>
#include <QWebFrame>
#include <QWebElement>
#include <qjson/parser.h>

#include <KIO/AccessManager>
#include <KDebug>


using namespace KGoogle;

AuthWidgetPrivate::AuthWidgetPrivate(AuthWidget *parent):
    QObject(),
    showProgressBar(true),
    progress(AuthWidget::None),
    q_ptr(parent)
{
    qRegisterMetaType< KGoogle::Services::AccountInfo >("AccountInfo");

    setupUi();
}

AuthWidgetPrivate::~AuthWidgetPrivate()
{
    delete vbox;
    delete label;
    delete progressbar;
    delete webview;
}


void AuthWidgetPrivate::setupUi()
{
    Q_Q(AuthWidget);

    vbox = new QVBoxLayout(q);
    q->setLayout(vbox);

    label = new QLabel(q);
    label->setText("<b>" + i18n("Authorizing token. This should take just a moment...") + "</b>");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setVisible(false);
    vbox->addWidget(label);

    progressbar = new QProgressBar(q);
    progressbar->setMinimum(0);
    progressbar->setMaximum(100);
    progressbar->setValue(0);
    vbox->addWidget(progressbar);

    webview = new QWebView(q);
    vbox->addWidget(webview);
    connect(webview, SIGNAL(loadProgress(int)), progressbar, SLOT(setValue(int)));
    connect(webview, SIGNAL(urlChanged(QUrl)), this, SLOT(webviewUrlChanged(QUrl)));
    connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(webviewFinished()));
}

void AuthWidgetPrivate::setProgress(AuthWidget::Progress progress)
{
    Q_Q(AuthWidget);

    this->progress = progress;
    Q_EMIT q->progress(progress);
}


void AuthWidgetPrivate::emitError(const KGoogle::Error errCode, const QString& msg)
{
    Q_Q(AuthWidget);

    label->setVisible(true);
    webview->setVisible(false);
    progressbar->setVisible(false);

    label->setText("<b>" + msg + "</b>");

    Q_EMIT q->error(errCode, msg);
    setProgress(AuthWidget::Error);
}


void AuthWidgetPrivate::webviewUrlChanged(const QUrl &url)
{
    /* Access token here - hide browser and tell user to wait until we
     * finish the authentication process ourselves */
    if (url.host() == "accounts.google.com" && url.path() == "/o/oauth2/approval") {
        webview->setVisible(false);
        progressbar->setVisible(false);
        label->setVisible(true);

        setProgress(AuthWidget::TokensRetrieval);
    }
}

void AuthWidgetPrivate::webviewFinished()
{
    QUrl url = webview->url();

    if (url.host() == "accounts.google.com" && url.path() == "/ServiceLogin") {
        if (username.isEmpty() && password.isEmpty()) {
            return;
        }

        QWebFrame *frame = webview->page()->mainFrame();
        if (!username.isEmpty()) {
            QWebElement email = frame->findFirstElement("input#Email");
            if (!email.isNull()) {
                email.setAttribute("value", username);
            }
        }

        if (!password.isEmpty()) {
            QWebElement passd = frame->findFirstElement("input#Passwd");
            if (!passd.isNull()) {
                passd.setAttribute("value", password);
            }
        }

        return;
    }

    if (url.host() == "accounts.google.com" && url.path() == "/o/oauth2/approval") {
        QWebElement el = webview->page()->mainFrame()->findFirstElement("textarea");
        if (el.isNull()) {
            emitError(KGoogle::AuthError, i18n("Parsing token page failed."));
            return;
        }

        QString token = el.toInnerXml();
        if (token.isEmpty()) {
            emitError(KGoogle::AuthError, i18n("Failed to obtain token."));
            return;
        }

        QNetworkAccessManager *nam = new KIO::Integration::AccessManager(this);
        QNetworkRequest request;

        connect(nam, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(networkRequestFinished(QNetworkReply*)));
        connect(nam, SIGNAL(finished(QNetworkReply*)),
                nam, SLOT(deleteLater()));

        request.setUrl(QUrl("https://accounts.google.com/o/oauth2/token"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QUrl params;
        params.addQueryItem("client_id", KGoogle::Auth::instance()->apiKey());
        params.addQueryItem("client_secret", KGoogle::Auth::instance()->apiSecret());
        params.addQueryItem("code", token);
        params.addQueryItem("redirect_uri", "urn:ietf:wg:oauth:2.0:oob");
        params.addQueryItem("grant_type", "authorization_code");

#ifdef DEBUG_RAWDATA
        kDebug() << "Authorizing token:" << params;
#endif

        nam->post(request, params.encodedQuery());
    }
}

void AuthWidgetPrivate::networkRequestFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emitError(KGoogle::AuthError, i18n("Authentization failed:<br>%1", reply->errorString()));
        return;
    }

    QJson::Parser parser;
    bool ok;

    QVariantMap parsed_data = parser.parse(reply->readAll(), &ok).toMap();
    if (!ok) {
        emitError(KGoogle::AuthError, i18n("Failed to parse server response."));
        return;
    }

#ifdef DEBUG_RAWDATA
    kDebug() << "Retrieved new tokens pair:" << parsed_data;
#endif

    account->setAccessToken(parsed_data["access_token"].toString());
    account->setRefreshToken(parsed_data["refresh_token"].toString());

    KGoogle::AccessManager *gam = new KGoogle::AccessManager;
    connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
            this, SLOT(accountInfoReceived(KGoogle::Reply*)));
    connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
            gam, SLOT(deleteLater()));
    connect(gam, SIGNAL(error(KGoogle::Error,QString)),
            this, SLOT(emitError(KGoogle::Error,QString)));

    KGoogle::Request *request;
    request = new KGoogle::Request(KGoogle::Services::AccountInfo::fetchUrl(),
                                   KGoogle::Request::Fetch, "AccountInfo", account);

    gam->sendRequest(request);
}

void AuthWidgetPrivate::accountInfoReceived(KGoogle::Reply* reply)
{
    Q_Q(AuthWidget);

    if (reply->error() != KGoogle::OK) {
        emitError(reply->error(), reply->errorString());
        return;
    }

    QList< KGoogle::Object* > data = reply->replyData();
    KGoogle::Objects::AccountInfo *accountInfo = static_cast< KGoogle::Objects::AccountInfo* >(data.first());

    account->setAccountName(accountInfo->email());

    delete reply;

    Q_EMIT q->authenticated(account);
    setProgress(AuthWidget::Finished);
}
