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

#include <qjson/parser.h>

#include "contactjob.h"
#include "contactlistjob.h"
#include "settings.h"

ContactListJob::ContactListJob(const QString& accessToken, const QString &lastSync):
  m_nam(new QNetworkAccessManager),
  m_accessToken(accessToken),
  m_lastSync(lastSync),
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
  QUrl requestUrl("https://www.google.com/m8/feeds/contacts/default/full?alt=json");
  
  /* Google refuses to send data for request to www.google.com/m8/feeds/contacts/user@domain.com/full,
   * but www.google.com/m8/feeods/contacts/default/full is OK */
  QList<QPair<QString,QString> > qItems = url.queryItems();
  QPair<QString,QString> pair;
  foreach (pair, qItems)
    requestUrl.addQueryItem(pair.first, pair.second);

  if (!requestUrl.hasQueryItem("updated-min") && !m_lastSync.isEmpty())
    requestUrl.addQueryItem("updated-min", m_lastSync);
  
  if (!requestUrl.hasQueryItem("showdeleted") && !m_lastSync.isEmpty())
    requestUrl.addQueryItem("showdeleted", "true");
  
  qDebug() << "Requesting" << requestUrl.toString();
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "3.0");  
  
  m_nam->get(request);
}

void ContactListJob::start()
{
  requestContacts (QUrl("https://www.google.com/m8/feeds/contacts/default/full?alt=json"));
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
  
  QJson::Parser parser;
  bool ok;
  QVariantMap data = parser.parse(reply->readAll(), &ok).toMap();
     
  if (!ok) {
      setError(1);
      setErrorText("Failed to parse server response.");
      emitResult();
      return;
  }
  
  QVariantMap feed = data["feed"].toMap();
  QVariantList links = feed["link"].toList();
  
  foreach (const QVariant &link, links) {
    if (link.toMap()["rel"].toString() == "next") {
      nextUrl = link.toMap()["href"].toString();
      break;
    }
  }
  
  itemsPerPage = feed["openSearch$itemsPerPage"].toMap()["$t"].toInt();
  startIndex = feed["openSearch$startIndex"].toMap()["$t"].toInt();
  resultsCnt = feed["openSearch$totalResults"].toMap()["$t"].toInt();
  
  QVariantList entries = feed["entry"].toList();
  foreach (const QVariant &ent, entries) {
   
      QVariantMap entry = ent.toMap();
    
      KABC::Addressee contact = ContactJob::JSONToKABC (entry);
      
      m_contacts->append(contact);
  }
   
  
  emitPercent(startIndex, resultsCnt);

  if (!nextUrl.isEmpty()) {
    qDebug() << "Done. Requesting next batch of contacts (" << startIndex+itemsPerPage << " to " << startIndex+2*itemsPerPage << ")";
    requestContacts(QUrl(nextUrl));
  } else {
    qDebug() << "No more items to fetch. Leaving";
    setError(0);
    emitResult();
  }
}