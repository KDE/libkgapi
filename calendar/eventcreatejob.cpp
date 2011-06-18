/*
    <one line to give the program's name and a brief idea of what it does.>
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

#include "eventcreatejob.h"
#include "eventjob.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QDebug>

#include <qjson/serializer.h>
#include <qjson/parser.h>

EventCreateJob::EventCreateJob(KCalCore::Event* event, const QString &calendarId, const QString& accessToken):
  m_accessToken(accessToken),
  m_calendarId(calendarId),
  m_event(event)
{
}

void EventCreateJob::start()
{
  requestData(QUrl("https://www.google.com/calendar/feeds/"+m_calendarId+"/private/full"));
}

void EventCreateJob::requestData(const QUrl& url)
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
  
  QJson::Serializer serializer;
  QByteArray data;
  QVariantMap jsonData;
  jsonData["data"] = EventJob::KCalToJSON(*m_event);
  data = serializer.serialize(jsonData);
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth " + m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "2.0");  
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  
  nam->post(request, data);
}

void EventCreateJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error()) {
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
  
  m_newEvent = EventJob::JSONToKCal(data);
  emitResult();
}
