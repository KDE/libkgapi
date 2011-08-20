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


#include "photojob.h"

#include <QtDebug>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QFile>

PhotoJob::PhotoJob(const QString &accessToken, const QUrl &photoUrl):
  m_accessToken(accessToken),
  m_photoUrl(photoUrl)
{}

void PhotoJob::start()
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  QString photoId = m_photoUrl.toString();
  photoId = photoId.mid(photoId.lastIndexOf("/")+1);
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(photoRetrieved(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  /* Google doesn't like user@gmail.com in the URL, but works with 'default' */
  request.setUrl(QUrl("https://www.google.com/m8/feeds/photos/media/default/"+photoId));
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "3.0");  

  nam->get(request);
}

void PhotoJob::photoRetrieved(QNetworkReply* reply)
{
  if (reply->error() != QNetworkReply::NoError) {
    setError(reply->error());
    setErrorText(reply->errorString());
    emitResult();
    return;
  }
   
  QByteArray data = reply->readAll();
  m_photo.loadFromData(data, (const char*)reply->header(QNetworkRequest::ContentTypeHeader).data());
  
  emitResult();
}

