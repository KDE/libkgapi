/*
    Akonadi Google - Calendar Resource
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

#include <QtNetwork/QNetworkRequest>

#include "qjson/parser.h"

#include "eventlistjob.h"
#include "event.h"

EventListJob::EventListJob(const QString &accessToken, const QString &lastSync, const QString &calendarId):
  m_accessToken(accessToken),
  m_lastSync(lastSync),
  m_calendarId(calendarId),
  m_nam(new QNetworkAccessManager)
{
  connect(m_nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
}

EventListJob::~EventListJob()
{
  delete m_nam;
}

void EventListJob::start()
{
  requestData(QUrl("https://www.google.com/calendar/feeds/"+m_calendarId+"/private/full"));
}


void EventListJob::requestData(const QUrl& url)
{
  QNetworkRequest request;
  QString url_str = url.toString().replace("%40", "@"); /* Replace %40 by "@" */
  QUrl requestUrl = url_str;
  
  /* Fetch only items created, edited or removed since last sync */
  if (!m_lastSync.isEmpty() && !requestUrl.hasQueryItem("start-min"))
    requestUrl.addQueryItem("start-min", m_lastSync);
  
  /* Include deleted items. */
  if (!requestUrl.hasQueryItem("showdeleted"))
    requestUrl.addQueryItem("showdeleted", "true");

  if (!requestUrl.hasQueryItem("alt"))
    requestUrl.addQueryItem("alt","jsonc");
    
  qDebug() << "Fetching batch from" << requestUrl.toString();
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "2.0");
  
  m_nam->get(request);
}

void EventListJob::requestFinished(QNetworkReply* reply)
{
  if (reply->error()) {
    setError(reply->error());
    setErrorText(reply->errorString());
    emitResult();
    return;
  }
  
  /* Google may send as 301 Redirect for the first time with a new URL of the feed. */
  if (reply->hasRawHeader("Location")) {
    requestData(reply->header(QNetworkRequest::LocationHeader).toUrl());
    return;
  }
  
  int itemsPerPage;
  int startIndex;
  int totalResults;
  QUrl nextLink;
  
  QJson::Parser parser;
  bool ok;
  QVariantMap data = parser.parse(reply->readAll(), &ok).toMap()["data"].toMap();
  
  itemsPerPage = data["itemsPerPage"].toInt();
  startIndex = data["startIndex"].toInt();
  totalResults = data["totalResults"].toInt();
  nextLink = data["nextLink"].toUrl();
  
  QVariantList eventsList = data["items"].toList();
  
  foreach (const QVariant &inc, eventsList) {
    Event::Event event;
    event.fromJSON(inc.toMap());
    m_events.append(event);
  }
  
  emitPercent(startIndex, totalResults);    
  
  if (nextLink.isValid()) {
    qDebug() << "Fetching items" << startIndex << "to" << (startIndex+itemsPerPage) << " of" << totalResults << "done.";
    requestData(nextLink);
  } else {
    emitResult();
    qDebug() << "EventListJob finished. No more items to fetch";
  }
}
