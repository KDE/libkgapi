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

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QtCore/QVariantMap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include "eventjob.h"

EventJob::EventJob(const QString& accessToken, const QString& calendarId, const QString& eventId):
  m_accessToken(accessToken),
  m_calendarId(calendarId),
  m_eventId(eventId),
  m_event(0)
{
}


void EventJob::start()
{
  requestData(QUrl("https://www.google.com/calendar/feeds/"+m_calendarId+"/private/full/"+m_eventId+"?alt=jsonc"));
}
  
  
void EventJob::requestData(const QUrl &url)
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  QUrl requestUrl = url;

  if (!requestUrl.hasQueryItem("alt"))
    requestUrl.addQueryItem("alt", "jsonc");
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "2.0");
  
  nam->get(request);
}

void EventJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error()) {
    setError(reply->error());
    setErrorText(reply->errorString());
    emitResult();
    return;
  }
  
  /* Google may sometimes return 301 Redirect with a new URL where the object is located. */
  if (reply->header(QNetworkRequest::LocationHeader).isValid()) {
    requestData(reply->header(QNetworkRequest::LocationHeader).toUrl());
    return;
  }    
  
  QJson::Parser parser;
  bool ok;
  QVariantMap data;
  
  data = parser.parse(reply->readAll(), &ok).toMap();
  
  if (!ok) {
    setError(1);
    setErrorText("Invalid response format");
    emitResult();
    return;
  }
  
  
  m_event = new Event::Event();
  m_event->fromJSON (data["data"].toMap());
  
  emitResult();
}
