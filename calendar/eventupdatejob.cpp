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
#include <qjson/serializer.h>
#include <qjson/parser.h>

#include "eventupdatejob.h"



EventUpdateJob::EventUpdateJob(Event::Event* event, const QString& calendarId, const QString& accessToken): 
  EventCreateJob(event, calendarId, accessToken)
{ }

void EventUpdateJob::start()
{
  requestData(QUrl("https://www.google.com/calendar/feeds/" + m_calendarId + "/private/full/" + m_event->uid()));
}


void EventUpdateJob::requestData(const QUrl& url)
{
 QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  /* Google gives us "@" URI-escaped (%40), but does not accept it, so we need to
   * unescape it */
  QUrl requestUrl = QUrl(url.toString().replace("%40", "@"));
  
  if (!requestUrl.hasQueryItem("alt"))
    requestUrl.addQueryItem("alt", "jsonc");
      
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  if (!m_event) {
    setError(1);
    setErrorText("No event to create");
    emitResult();
    return;
  }
  
  QJson::Serializer serializer;
  QByteArray data;
  QVariantMap jsonData, eventJson;
  eventJson = m_event->toJSON();
  eventJson["selfLink"] = "https://www.google.com/calendar/feeds/" + m_calendarId + "/private/full/" + m_event->uid();
  eventJson["canEdit"] = true;
  jsonData["data"] = eventJson;
  jsonData["apiVersion"] = "2.3";
  data = serializer.serialize(jsonData);
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth " + m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "2.0");  
  request.setRawHeader("If-Match", "*");

  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  
  nam->put(request, data);
}

void EventUpdateJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error()) {
    qDebug() << reply->readAll();
    setError(reply->error());
    setErrorText(reply->errorString());
    emitResult();
    return;
  }
  
  /* After first request Google will redirect us to a new URL,
   * so resend the request to the new address */
  if (!reply->header(QNetworkRequest::LocationHeader).toString().isEmpty() &&
      !reply->request().url().hasQueryItem("gsessionid"))
  {
    requestData(reply->header(QNetworkRequest::LocationHeader).toUrl());
    return;
  }

  QJson::Parser parser;
  bool ok;
  QVariantMap data = parser.parse(reply->readAll(), &ok).toMap();
  
  if (!ok) {
    setError(1);
    setErrorText("Failed to parse server reply");
    emitResult();
    return;
  }
  
  m_newEvent = new Event::Event();
  m_newEvent->fromJSON(data["data"].toMap());
  emitResult();
}
