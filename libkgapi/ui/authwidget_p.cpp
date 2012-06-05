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


using namespace KGAPI;
using namespace KGAPI::Ui;

AuthWidgetPrivate::AuthWidgetPrivate(AuthWidget *parent):
    QObject(),
    showProgressBar(true),
    progress(AuthWidget::None),
    q_ptr(parent)
{
    qRegisterMetaType< KGAPI::Services::AccountInfo >("AccountInfo");

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


void AuthWidgetPrivate::emitError(const KGAPI::Error errCode, const QString& msg)
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
        QString title = webview->title();
        QString token;

        if (title.startsWith(QLatin1String("success"), Qt::CaseInsensitive)) {
            int pos = title.indexOf(QLatin1String("code="));
            /* Skip the 'code=' string as well */
            token = title.mid (pos + 5);
        } else {
            emitError(KGAPI::AuthError, i18n("Parsing token page failed."));
            return;
        }

        if (token.isEmpty()) {
            emitError(KGAPI::AuthError, i18n("Failed to obtain token."));
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
        params.addQueryItem("client_id", KGAPI::Auth::instance()->apiKey());
        params.addQueryItem("client_secret", KGAPI::Auth::instance()->apiSecret());
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
        emitError(KGAPI::AuthError, i18n("Authentization failed:\n%1", reply->errorString()));
        return;
    }

    QJson::Parser parser;
    bool ok;

    QVariantMap parsed_data = parser.parse(reply->readAll(), &ok).toMap();
    if (!ok) {
        emitError(KGAPI::AuthError, i18n("Failed to parse server response."));
        return;
    }

#ifdef DEBUG_RAWDATA
    kDebug() << "Retrieved new tokens pair:" << parsed_data;
#endif

    account->setAccessToken(parsed_data["access_token"].toString());
    account->setRefreshToken(parsed_data["refresh_token"].toString());

    KGAPI::AccessManager *gam = new KGAPI::AccessManager;
    connect(gam, SIGNAL(replyReceived(KGAPI::Reply*)),
            this, SLOT(accountInfoReceived(KGAPI::Reply*)));
    connect(gam, SIGNAL(replyReceived(KGAPI::Reply*)),
            gam, SLOT(deleteLater()));
    connect(gam, SIGNAL(error(KGAPI::Error,QString)),
            this, SLOT(emitError(KGAPI::Error,QString)));

    KGAPI::Request *request;
    request = new KGAPI::Request(KGAPI::Services::AccountInfo::fetchUrl(),
                                   KGAPI::Request::Fetch, "AccountInfo", account);

    gam->sendRequest(request);
}

void AuthWidgetPrivate::accountInfoReceived(KGAPI::Reply* reply)
{
    Q_Q(AuthWidget);

    if (reply->error() != KGAPI::OK) {
        emitError(reply->error(), reply->errorString());
        return;
    }

    QList< KGAPI::Object* > data = reply->replyData();
    KGAPI::Objects::AccountInfo *accountInfo = static_cast< KGAPI::Objects::AccountInfo* >(data.first());

    account->setAccountName(accountInfo->email());

    delete reply;

    Q_EMIT q->authenticated(account);
    setProgress(AuthWidget::Finished);
}
