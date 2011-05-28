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
#include <KDE/KWindowSystem>
#include <KDE/KLocalizedString>

#include <QtCore/QByteArray>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QDebug>

#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <qjson/parser.h>

/****************** Private implementation *****************/

void AuthDialog::setError(const QString& error)
{
  m_label->setVisible(true);
  m_webiew->setVisible(false);
  m_progressbar->setVisible(false);
  
  m_label->setText(error);
}


/********************* Public implementation ***************/

AuthDialog::AuthDialog(Akonadi::ResourceBase *parent, WId windowId): 
  KDialog()
{
  KWindowSystem::setMainWindow(this, windowId);
  
  m_widget = new QWidget(this);
  setMainWidget(m_widget);
  
  m_vbox = new QVBoxLayout(m_widget);
  m_widget->setLayout(m_vbox);

  m_label = new QLabel(m_widget);
  m_label->setText(tr("<b>Authorizing token. This should take just a moment...</b>"));
  m_label->setWordWrap(true);
  m_label->setAlignment(Qt::AlignCenter);
  m_label->setVisible(false);
  m_vbox->addWidget(m_label);
  
  m_progressbar = new QProgressBar (m_widget);
  m_progressbar->setMinimum(0);
  m_progressbar->setMaximum(100);
  m_progressbar->setValue(0);
  m_vbox->addWidget(m_progressbar);
  
  m_webiew = new KWebView(m_widget, true);
  m_vbox->addWidget(m_webiew);
  connect (m_webiew, SIGNAL(loadProgress(int)),
	   m_progressbar, SLOT(setValue(int)));
  connect (m_webiew, SIGNAL(urlChanged(QUrl)),
	   this, SLOT (webviewUrlChanged(QUrl)));
  connect (m_webiew, SIGNAL (loadFinished(bool)),
	   this, SLOT (webviewFinished()));
  
  m_hbox = new QHBoxLayout(m_widget);
  m_vbox->addLayout(m_hbox);
  
  setButtons(KDialog::Cancel);  
  
  Q_UNUSED (parent);
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

void AuthDialog::setScopes(const QStringList& scopes)
{
  m_scopes << scopes;
}

void AuthDialog::auth(const QString& clientId, const QString& clientSecret)
{
  m_clientId = clientId;
  m_clientSecret = clientSecret;
  
  m_webiew->setUrl(QUrl("https://accounts.google.com/o/oauth2/auth?"
			"client_id="+clientId+"&"
			"redirect_uri=urn:ietf:wg:oauth:2.0:oob&"
			"scope="+m_scopes.join(" ")+"&"
			"response_type=code"));
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
  
  if (url.host() == "accounts.google.com" && url.path() == "/o/oauth2/approval") {
    QWebElement el = m_webiew->page()->mainFrame()->findFirstElement("textarea");
    if (el.isNull()) {
      setError("<b>Parsing token page failed.</b>");
      return;
    }

    QString token = el.toInnerXml();
    if (token.isEmpty()) {
      setError("<b>Failed to obtain token.</b>");
      return;
    }
    
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    QNetworkRequest request;
    QByteArray data;

    connect (nam, SIGNAL(finished(QNetworkReply*)),
	     this, SLOT(networkRequestFinished(QNetworkReply*)));
    connect (nam, SIGNAL(finished(QNetworkReply*)),
	     nam, SLOT(deleteLater()));

    request.setUrl(QUrl("https://accounts.google.com/o/oauth2/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    data.append("client_id="+m_clientId.toLatin1()+"&");
    data.append("client_secret="+m_clientSecret.toLatin1()+"&");
    data.append("code="+token.toLatin1()+"&");
    data.append("redirect_uri=urn:ietf:wg:oauth:2.0:oob&");
    data.append("grant_type=authorization_code");
    
    nam->post(request, data);    
  }
}

void AuthDialog::networkRequestFinished(QNetworkReply* reply)
{
  if (reply->error() != QNetworkReply::NoError) {
    setError("<b>Authentization failed:</b><br>"+reply->errorString());
    return;
  }
  
  QJson::Parser parser;
  bool ok;
  
  QVariantMap parsed_data = parser.parse(reply->readAll(), &ok).toMap();
  if (!ok) {
    setError("<b>Failed to parse server response.</b>");
    return;
  }
  
  QString access_token = parsed_data["access_token"].toString();
  QString refresh_token = parsed_data["refresh_token"].toString();
  
  m_accessToken = access_token;
  m_refreshToken = refresh_token;
  
  emit finished();
  accept();
}
