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

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <qjson/parser.h>

#include "calendarlistjob.h"

CalendarListJob::CalendarListJob(const QString& accessToken):
  m_accessToken(accessToken)
{
}

void CalendarListJob::start()
{
  requestData(QUrl("https://www.google.com/calendar/feeds/default/allcalendars/full?alt=jsonc"));
}

void CalendarListJob::requestData(const QUrl& url)
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  request.setUrl(url);
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "2.0");
  
  nam->get(request);
}


void CalendarListJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error()) {
    setError(1);
    setErrorText("Failed to fetch calendars. Server replied: " + reply->readAll());
    emitResult();
    return;
  }
  
  /* Calendar API sends 301 Redirect first so we need to send request again
   * on the URL we received */
  if (!reply->header(QNetworkRequest::LocationHeader).toString().isEmpty()) {
    requestData(reply->header(QNetworkRequest::LocationHeader).toUrl());
    return;
  }
  
  QJson::Parser parser;
  bool ok;
 
  QVariantMap data = parser.parse(reply->readAll(), &ok).toMap()["data"].toMap();

  foreach (QVariant cal, data["items"].toList()) {
    QString id = cal.toMap()["id"].toString();
    id.remove("http://www.google.com/calendar/feeds/default/calendars/"); // Remove the URL prefix
    m_calendars.insert(id, cal.toMap()["title"].toString());
  }
  
  emitResult();
}


