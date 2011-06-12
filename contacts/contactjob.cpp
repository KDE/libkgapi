/*
    Akonadi Google - Contacts resource
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

#include <KDE/KDebug>
#include <KDE/KABC/PhoneNumber>
#include <KDE/KABC/Address>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <qjson/parser.h>

#include "contactjob.h"

using namespace Contact;

ContactJob::ContactJob(const QString &contactId, const QString &accessToken):
  m_contactId(contactId),
  m_accessToken(accessToken)
{    
}

void ContactJob::start()
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest req;
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(contactRetrieved(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  req.setUrl("https://www.google.com/m8/feeds/contacts/default/full/"+m_contactId+"?alt=json&oauth_token="+m_accessToken);
  req.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  req.setRawHeader("GData-Version", "3.0");  
  
  nam->get(req);
}

void ContactJob::contactRetrieved(QNetworkReply *reply)
{
   if (reply->error() != QNetworkReply::NoError) {
     qDebug() << "Failed to fetch contact: " + reply->errorString();
     setError(1);
     setErrorText("Failed to fetch contact: " + reply->errorString());
     emitResult();
     return;
   }

  QJson::Parser parser;
  bool ok;
  
  QVariantMap data = parser.parse(reply->readAll(), &ok).toMap();
  if (!ok) {
     setError(1);
     setErrorText("Invalid feed format");
     emitResult();
     return;
  }
     
  m_contact = new Contact::Contact();
  m_contact->fromJSON(data["entry"].toMap());
  
  setError(0);
  emitResult();
}
