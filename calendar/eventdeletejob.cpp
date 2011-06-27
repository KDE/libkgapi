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

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QDebug>

#include "eventdeletejob.h"

EventDeleteJob::EventDeleteJob(const QString& eventId, const QString& calendarId, const QString& accessToken):
  m_eventId(eventId),
  m_calendarId(calendarId),
  m_accessToken(accessToken)
{ }

void EventDeleteJob::start()
{
  requestDelete(QUrl("https://https://www.google.com/calendar/feeds/" + m_calendarId + "/private/full/" + m_eventId));
}

void EventDeleteJob::requestDelete(const QUrl& url)
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  QUrl requestUrl = url.toString().replace("%40", "@");
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authentication", "OAuth " + m_accessToken.toLatin1());
  request.setRawHeader("If-Match", "*");
  
  nam->deleteResource(request);
}

void EventDeleteJob::requestFinished(QNetworkReply* reply)
{

  qDebug() << reply->readAll();  
  
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
    requestDelete(reply->header(QNetworkRequest::LocationHeader).toUrl());
    return;
  }
  


  setError(0);
  emitResult();
}


