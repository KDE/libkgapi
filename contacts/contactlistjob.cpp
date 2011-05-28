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
#include <QtCore/QDebug>
#include <QtCore/QPair>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

#include "contactjob.h"
#include "contactlistjob.h"
#include "settings.h"

ContactListJob::ContactListJob(const QString& accessToken):
  m_nam(new QNetworkAccessManager),
  m_accessToken(accessToken),
  m_contacts(new QList<KABC::Addressee>)
{
  connect (m_nam, SIGNAL (finished(QNetworkReply*)),
	   this, SLOT(contactListRetrieved(QNetworkReply*)));
}

ContactListJob::~ContactListJob()
{
  delete m_contacts;
  delete m_nam;
}

void ContactListJob::requestContacts(const QUrl &url)
{
  
  QNetworkRequest request;  
  QUrl requestUrl("https://www.google.com/m8/feeds/contacts/default/full");
  
  /* Google refuses to send data from request google.com/m8/feeds/contacts/user@domain.com/full,
   * but google.com/m8/feeods/contacts/default/full is OK */
  QList<QPair<QString,QString> > qItems = url.queryItems();
  QPair<QString,QString> pair;
  foreach (pair, qItems)
    requestUrl.addQueryItem(pair.first, pair.second);
  
  qDebug() << "Requesting" << requestUrl.toString();
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth "+Settings::self()->accessToken().toLatin1());
  
  m_nam->get(request);
}

void ContactListJob::start()
{
  requestContacts (QUrl("https://www.google.com/m8/feeds/contacts/default/full"));
}

void ContactListJob::contactListRetrieved(QNetworkReply *reply)
{
  QString nextUrl;
  int startIndex;
  int resultsCnt;
  int itemsPerPage;

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Contact list reqeust failed. Server replied:" << reply->errorString();
    setError(1);
    setErrorText("Contact list request failed. Server replied: "+reply->errorString());
    emitResult();
    return;
  }
  
  QDomDocument doc;
  doc.setContent(reply->readAll());
  
  QDomElement root = doc.documentElement();
  if (root.tagName() != "feed") {
     qDebug() << "Invalid feed format (root tag is " << root.tagName() << ")";
     setError(1);
     setErrorText("Invalid feed format");
     emitResult();
     return;
  }
     
  QDomNode n = root.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
   
      /* Get URL of next batch of contacts */
      if ((e.tagName() == "link") && (e.attribute("rel") == "next")) {
	nextUrl = e.attribute("href");
      }
      
      /* Total number of contacts to be downloaded */
      if (e.tagName() == "openSearch:totalResults") {
	resultsCnt = e.text().toInt();
      }
      
      /* Index of first item in this batch */
      if (e.tagName() == "openSearch:startIndex") {
	startIndex = e.text().toInt();
      }
      
      /* Items per batch */
      if (e.tagName() == "openSearch:itemsPerPage") {
	itemsPerPage = e.text().toInt();
      }
      
      /* Contact entry */
      if (e.tagName() == "entry") {
	KABC::Addressee contact = ContactJob::xmlEntryToKABC(e); 
      
	m_contacts->append(contact);
      }
    }
    
    n = n.nextSibling();    
  }
  
  if (resultsCnt > 0)
    emitPercent(((startIndex - 1)*itemsPerPage / resultsCnt)*100, 100);

  if (!nextUrl.isEmpty()) {
    qDebug() << "Done. Requesting next batch of contacts (" << startIndex+itemsPerPage << " to " << startIndex+2*itemsPerPage << ")";
    requestContacts(QUrl(nextUrl));
  } else {
    qDebug() << "No more items to fetch. Leaving";
    setError(0);
    emitResult();
  }
}