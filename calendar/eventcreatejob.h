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


#ifndef EVENTCREATEJOB_H
#define EVENTCREATEJOB_H

#include <KJob>

#include <QtNetwork/QNetworkReply>

#include "event.h"

class EventCreateJob : public KJob
{

  Q_OBJECT
  public:
    EventCreateJob(Event::Event *event,
		   const QString &calendarId,
		   const QString &accessToken);
  
    void start();
    
    Event::Event* newEvent() { return m_newEvent; }
    
  protected Q_SLOTS:
    void requestData(const QUrl &url);
    void requestFinished(QNetworkReply *reply);
    
  protected:
    QString m_accessToken;
    QString m_calendarId;
    Event::Event *m_event;
    Event::Event *m_newEvent;
};

#endif // EVENTCREATEJOB_H