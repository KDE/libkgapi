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


#ifndef INCIDENCELISTJOB_H
#define INCIDENCELISTJOB_H

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <KDE/KJob>

#include "event.h"

class EventListJob : public KJob
{

  Q_OBJECT
  public:
    EventListJob(const QString &accessToken,
		 const QString &lastSync,
		 const QString &calendarId);
    
    ~EventListJob();
    
    void start();
    
    Event::Event::List events() { return m_events; }
    
  private Q_SLOTS:
    void requestData(const QUrl &url);
    void requestFinished(QNetworkReply *reply);
    
    
  private:
    QString m_accessToken;
    QString m_lastSync;
    QString m_calendarId;

    QNetworkAccessManager *m_nam;   
    
    Event::Event::List m_events;
    
};

#endif // INCIDENCELISTJOB_H
