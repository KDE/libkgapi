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

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include "contactdeletejob.h"

ContactDeleteJob::ContactDeleteJob(const QString& uid, const QString& accessToken):
  m_uid(uid),
  m_accessToken(accessToken)
{}

ContactDeleteJob::~ContactDeleteJob()
{
}

void ContactDeleteJob::start()
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;

  connect (nam, SIGNAL(finished(QNetworkReply*)),
	   this, SLOT(requestFinished(QNetworkReply*)));
  connect (nam, SIGNAL(finished(QNetworkReply*)),
	   nam, SLOT(deleteLater()));
  request.setUrl("https://www.google.com/m8/feeds/contacts/default/full/"+m_uid);
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("If-Match", "*");

  nam->deleteResource(request);
}

void ContactDeleteJob::requestFinished(QNetworkReply* reply)
{
  if (reply->error()) {
    setError(1); 
    setErrorText("Failed to remove contact. Server replied:"+reply->errorString());
  }
  
  emitResult();
}



