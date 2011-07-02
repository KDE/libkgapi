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


#ifndef INCIDENCEJOB_H
#define INCIDENCEJOB_H

#include <KDE/KJob>

#include <QtNetwork/QNetworkReply>

#include "event.h"

class EventJob : public KJob
{

  Q_OBJECT
  public:
    EventJob(const QString &accessToken,
	     const QString &calendarId,
	     const QString &eventId);
    
    void start();
    
    Event::Event* getEvent() { return m_event; }
    
  private Q_SLOTS:
    void requestData(const QUrl &url);
    void requestFinished(QNetworkReply*);
    
  private:
    QString m_accessToken;
    QString m_calendarId;
    QString m_eventId;
    
    Event::Event* m_event;
};

#endif // INCIDENCEJOB_H