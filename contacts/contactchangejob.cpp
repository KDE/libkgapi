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
#include <QtXml/QDomElement>

#include <KDE/KDateTime>

#include "contactjob.h"
#include "contactchangejob.h"

using namespace Contact;

ContactChangeJob::ContactChangeJob(KABC::Addressee addressee, const QString& contactId, const QString& accessToken):
  m_addressee(addressee),
  m_contactId(contactId),
  m_accessToken(accessToken)
{}


void ContactChangeJob::start()
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  QByteArray data;
  Contact::Contact *contact = new Contact::Contact();
  contact->fromKABC(&m_addressee);
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  request.setUrl(QUrl("https://www.google.com/m8/feeds/contacts/default/full/"+m_contactId));
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "3.0");
  request.setRawHeader("If-Match", "*");
  request.setRawHeader("Content-type", "application/atom+xml");
  
  QString header = "<atom:entry xmlns:atom='http://www.w3.org/2005/Atom' xmlns:gd='http://schemas.google.com/g/2005'>"
		   "<atom:category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/contact/2008#contact' />"
		   "<id>http://www.google.com/m8/feeds/contacts/default/base/" + m_contactId + "</id>"
		   "<updated>"+ KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ")+"</updated>"
		   "<category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/contact/2008#contact' />"
		   "<title type='text'>"+m_addressee.formattedName()+"</title>";
  data = contact->toXML();
  data.prepend(header.toLatin1());
  data.append("</atom:entry>");
  
  nam->put(request, data);
}

void ContactChangeJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error() != QNetworkReply::NoError) {
    setError(1);
    setErrorText("Failed to update contact. Server replied: " + reply->errorString());
    emitResult();
    return;
  }
  
  QDomDocument doc;
  doc.setContent(reply->readAll());
  
  QDomElement el = doc.documentElement().elementsByTagName("entry").at(0).toElement();
  m_contact = new Contact::Contact();
  m_contact->fromXML(el);
  
  setError(0);
  emitResult();
}


