/*
    Akonadi Google - Contacts Resource
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

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QtXml/QDomDocument>

#include "contactcreatejob.h"
#include "contactjob.h"


ContactCreateJob::ContactCreateJob(KABC::Addressee addressee, const QString& accessToken):
  m_accessToken(accessToken),
  m_addressee(addressee)
{}

void ContactCreateJob::start()
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  QByteArray data;
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  request.setUrl(QUrl("https://www.google.com/m8/feeds/contacts/default/full"));
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "3.0");
  request.setRawHeader("Content-type", "application/atom+xml");
  
  data = ContactJob::KABCToXmlEntry(m_addressee);
  
  QDomDocument doc;
  doc.setContent(data);
  
  nam->post(request, doc.toString().toLatin1());
}

void ContactCreateJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error() != QNetworkReply::NoError) {
    setError(1);
    setErrorText("Error creating contact. Server replied: " + reply->errorString());
    emitResult();
    return;
  }
  
  QDomDocument doc;
  doc.setContent(reply->readAll());

  QDomNode entry = doc.documentElement().elementsByTagName("entry").at(0);
  
  m_outputAddressee = ContactJob::xmlEntryToKABC(entry.toElement());
  
  emitResult();
}
