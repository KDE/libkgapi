/*
    Akonadi Google Resource - OAuth authentication dialog
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

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


#include "authdialog.h"

#include <KDE/KDebug>
#include <KDE/KLocalizedString>
#include <KDE/KWindowSystem>
#include <KDE/KWebView>
#include <KIO/AccessManager>

#include <QtCore/QByteArray>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtNetwork/QNetworkReply>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

#include <qjson/parser.h>

#include "accessmanager.h"
#include "auth.h"
#include "reply.h"
#include "request.h"
#include "services/accountinfo.h"
#include "objects/accountinfo.h"

extern int debugArea();

/****************** Private implementation *****************/

void AuthDialog::emitError(const KGoogle::Error errCode, const QString& msg)
{
    m_label->setVisible(true);
    m_webiew->setVisible(false);
    m_progressbar->setVisible(false);

    m_label->setText("<b>" + msg + "</b>");

    Q_EMIT error(errCode, msg);
}


void AuthDialog::webviewUrlChanged(const QUrl &url)
{
    QString token, verifier;

    /* Access token here - hide browser and tell user to wait until we
     * finish the authentication process ourselves */
    if (url.host() == "accounts.google.com" && url.path() == "/o/oauth2/approval") {
        m_webiew->setVisible(false);
        m_progressbar->setVisible(false);
        m_label->setVisible(true);
    }
}

void AuthDialog::webviewFinished()
{
    QUrl url = m_webiew->url();

    if (url.host() == "accounts.google.com" && url.path() == "/ServiceLogin") {
        if (m_username.isEmpty() && m_password.isEmpty()) {
            return;
        }

        QWebFrame *frame = m_webiew->page()->mainFrame();
        if (!m_username.isEmpty()) {
            QWebElement email = frame->findFirstElement("input#Email");
            if (!email.isNull()) {
                email.setAttribute("value", m_username);
            }
        }

        if (!m_password.isEmpty()) {
            QWebElement passd = frame->findFirstElement("input#Passwd");
            if (!passd.isNull()) {
                passd.setAttribute("value", m_password);
            }
        }

        return;
    }

    if (url.host() == "accounts.google.com" && url.path() == "/o/oauth2/approval") {
        QWebElement el = m_webiew->page()->mainFrame()->findFirstElement("textarea");
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

void AuthDialog::networkRequestFinished(QNetworkReply* reply)
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

    m_account->setAccessToken(parsed_data["access_token"].toString());
    m_account->setRefreshToken(parsed_data["refresh_token"].toString());

    KGoogle::AccessManager *gam = new KGoogle::AccessManager;
    connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
            this, SLOT(accountInfoReceived(KGoogle::Reply*)));
    connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
            gam, SLOT(deleteLater()));

    KGoogle::Request *request;
    request = new KGoogle::Request(KGoogle::Services::AccountInfo::fetchUrl(),
                                   KGoogle::Request::Fetch, "AccountInfo", m_account);

    gam->sendRequest(request);
}

void AuthDialog::accountInfoReceived(KGoogle::Reply* reply)
{
    if (reply->error() != KGoogle::OK) {
        emitError(reply->error(), reply->errorString());
        return;
    }

    QList< KGoogle::Object* > data = reply->replyData();
    KGoogle::Objects::AccountInfo *accountInfo = static_cast< KGoogle::Objects::AccountInfo* >(data.first());

    m_account->setAccountName(accountInfo->email());

    delete reply;

    Q_EMIT authenticated(m_account);
    accept();
}


/********************* Public implementation ***************/

AuthDialog::AuthDialog(WId windowId):
    KDialog()
{
    KWindowSystem::setMainWindow(this, windowId);

    qRegisterMetaType< KGoogle::Services::AccountInfo >("AccountInfo");

    setModal(true);

    m_widget = new QWidget(this);
    setMainWidget(m_widget);

    m_vbox = new QVBoxLayout(m_widget);
    m_widget->setLayout(m_vbox);

    m_label = new QLabel(m_widget);
    m_label->setText("<b>" + i18n("Authorizing token. This should take just a moment...") + "</b>");
    m_label->setWordWrap(true);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setVisible(false);
    m_vbox->addWidget(m_label);

    m_progressbar = new QProgressBar(m_widget);
    m_progressbar->setMinimum(0);
    m_progressbar->setMaximum(100);
    m_progressbar->setValue(0);
    m_vbox->addWidget(m_progressbar);

    m_webiew = new KWebView(m_widget, true);
    m_vbox->addWidget(m_webiew);
    connect(m_webiew, SIGNAL(loadProgress(int)),
            m_progressbar, SLOT(setValue(int)));
    connect(m_webiew, SIGNAL(urlChanged(QUrl)),
            this, SLOT(webviewUrlChanged(QUrl)));
    connect(m_webiew, SIGNAL(loadFinished(bool)),
            this, SLOT(webviewFinished()));

    m_hbox = new QHBoxLayout(m_widget);
    m_vbox->addLayout(m_hbox);

    setButtons(KDialog::Cancel);
}

AuthDialog::~AuthDialog()
{
    delete m_label;
    delete m_progressbar;
    delete m_webiew;
    delete m_hbox;
    delete m_vbox;
    delete m_widget;
}

void AuthDialog::authenticate(KGoogle::Account::Ptr &account)
{

    if (account.isNull() || account->scopes().isEmpty()) {
        throw KGoogle::Exception::InvalidAccount();
        return;
    }

    m_account = account;

    QStringList scopes;
    Q_FOREACH(const QUrl & scope, account->scopes()) {
        scopes << scope.toString();
    }

    QUrl url("https://accounts.google.com/o/oauth2/auth");
    url.addQueryItem("client_id", KGoogle::Auth::instance()->apiKey());
    url.addQueryItem("redirect_uri", "urn:ietf:wg:oauth:2.0:oob");
    url.addQueryItem("scope", scopes.join(" "));
    url.addQueryItem("response_type", "code");

#ifdef DEBUG_RAWDATA
    kDebug() << "Requesting new token:" << url;
#endif

    m_webiew->setUrl(url);
}

void AuthDialog::setUsername(const QString& username)
{
    m_username = username;
}

void AuthDialog::setPassword(const QString& password)
{
    m_password = password;
}

void AuthDialog::clearCredentials()
{
    m_username = QString();
    m_password = QString();
}