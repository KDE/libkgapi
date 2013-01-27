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
#include "debug.h"
#include "request.h"
#include "reply.h"
#include "services/accountinfo.h"
#include "objects/accountinfo.h"

#include <QNetworkAccessManager>
#include <QWebFrame>
#include <QWebElement>
#include <qjson/parser.h>

#include <KUrl>
#include <KIO/AccessManager>

#include <QStringBuilder>


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
    label->setText(QLatin1String("<b>") % i18n("Authorizing token. This should take just a moment...") % QLatin1String("</b>"));
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

    KGAPIDebug() << progress;
    this->progress = progress;
    Q_EMIT q->progress(progress);
}


void AuthWidgetPrivate::emitError(const KGAPI::Error errCode, const QString& msg)
{
    Q_Q(AuthWidget);

    label->setVisible(true);
    webview->setVisible(false);
    progressbar->setVisible(false);

    label->setText(QLatin1String("<b>") % msg % QLatin1String("</b>"));

    Q_EMIT q->error(errCode, msg);
    setProgress(AuthWidget::Error);
}


void AuthWidgetPrivate::webviewUrlChanged(const QUrl &url)
{
    KGAPIDebug() << url;

    /* Access token here - hide browser and tell user to wait until we
     * finish the authentication process ourselves */
    if (url.host() == QLatin1String("accounts.google.com") && url.path() == QLatin1String("/o/oauth2/approval")) {
        webview->setVisible(false);
        progressbar->setVisible(false);
        label->setVisible(true);

        setProgress(AuthWidget::TokensRetrieval);
    }
}

void AuthWidgetPrivate::webviewFinished()
{
    QUrl url = webview->url();
    KGAPIDebug() << url;

    if (url.host() == QLatin1String("accounts.google.com") && url.path() == QLatin1String("/ServiceLogin")) {
        if (username.isEmpty() && password.isEmpty()) {
            return;
        }

        QWebFrame *frame = webview->page()->mainFrame();
        if (!username.isEmpty()) {
            QWebElement email = frame->findFirstElement(QLatin1String("input#Email"));
            if (!email.isNull()) {
                email.setAttribute(QLatin1String("value"), username);
            }
        }

        if (!password.isEmpty()) {
            QWebElement passd = frame->findFirstElement(QLatin1String("input#Passwd"));
            if (!passd.isNull()) {
                passd.setAttribute(QLatin1String("value"), password);
            }
        }

        return;
    }

    if (url.host() == QLatin1String("accounts.google.com") && url.path() == QLatin1String("/o/oauth2/approval")) {
        QString title = webview->title();
        QString token;

        if (title.startsWith(QLatin1String("success"), Qt::CaseInsensitive)) {
            int pos = title.indexOf(QLatin1String("code="));
            /* Skip the 'code=' string as well */
            token = title.mid (pos + 5);
        } else {
            KGAPIDebug() << "Parsing token page failed. Title:" << title;
            KGAPIDebugRawData() << webview->page()->mainFrame()->toHtml();
            emitError(KGAPI::AuthError, i18n("Parsing token page failed."));
            return;
        }

        if (token.isEmpty()) {
            KGAPIDebug() << "Failed to obtain token.";
            KGAPIDebugRawData() << webview->page()->mainFrame()->toHtml();
            emitError(KGAPI::AuthError, i18n("Failed to obtain token."));
            return;
        }

        QNetworkAccessManager *nam = new KIO::Integration::AccessManager(this);
        QNetworkRequest request;

        connect(nam, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(networkRequestFinished(QNetworkReply*)));
        connect(nam, SIGNAL(finished(QNetworkReply*)),
                nam, SLOT(deleteLater()));

        request.setUrl(KUrl("https://accounts.google.com/o/oauth2/token"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

        QUrl params;
        params.addQueryItem(QLatin1String("client_id"), KGAPI::Auth::instance()->apiKey());
        params.addQueryItem(QLatin1String("client_secret"), KGAPI::Auth::instance()->apiSecret());
        params.addQueryItem(QLatin1String("code"), token);
        params.addQueryItem(QLatin1String("redirect_uri"), QLatin1String("urn:ietf:wg:oauth:2.0:oob"));
        params.addQueryItem(QLatin1String("grant_type"), QLatin1String("authorization_code"));

        KGAPIDebugRawData() << "Authorizing token:" << params;

        nam->post(request, params.encodedQuery());
    }
}

void AuthWidgetPrivate::networkRequestFinished(QNetworkReply* reply)
{
    const QByteArray data = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        KGAPIDebug() << "Authentization failed:" << reply->errorString();
        KGAPIDebugRawData() << data;
        emitError(KGAPI::AuthError, i18n("Authentization failed:\n%1", reply->errorString()));
        return;
    }

    QJson::Parser parser;
    bool ok;

    QVariantMap parsed_data = parser.parse(data, &ok).toMap();
    if (!ok) {
        KGAPIDebug() << "Failed to parse server response.";
        KGAPIDebugRawData() << data;
        emitError(KGAPI::AuthError, i18n("Failed to parse server response."));
        return;
    }

    KGAPIDebugRawData() << "Retrieved new tokens pair:" << parsed_data;

    account->setAccessToken(parsed_data.value(QLatin1String("access_token")).toString());
    account->setRefreshToken(parsed_data.value(QLatin1String("refresh_token")).toString());

    KGAPI::AccessManager *gam = new KGAPI::AccessManager;
    connect(gam, SIGNAL(replyReceived(KGAPI::Reply*)),
            this, SLOT(accountInfoReceived(KGAPI::Reply*)));
    connect(gam, SIGNAL(replyReceived(KGAPI::Reply*)),
            gam, SLOT(deleteLater()));
    connect(gam, SIGNAL(error(KGAPI::Error,QString)),
            this, SLOT(emitError(KGAPI::Error,QString)));

    KGAPI::Request *request;
    request = new KGAPI::Request(KGAPI::Services::AccountInfo::fetchUrl(),
                                 KGAPI::Request::Fetch, QLatin1String("AccountInfo"),
				 account);

    gam->sendRequest(request);
    KGAPIDebug() << "Requesting AccountInfo";
}

void AuthWidgetPrivate::accountInfoReceived(KGAPI::Reply* reply)
{
    Q_Q(AuthWidget);

    if (reply->error() != KGAPI::OK) {
        KGAPIDebug() << "Error when retrieving AccountInfo:" << reply->errorString();
        KGAPIDebugRawData() << reply->readAll();
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
