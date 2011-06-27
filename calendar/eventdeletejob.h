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


#ifndef EVENTDELETEJOB_H
#define EVENTDELETEJOB_H

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include <KDE/KJob>

class EventDeleteJob : public KJob
{

  Q_OBJECT
  public:
    EventDeleteJob(const QString &eventId,
		   const QString &calendarId,
		   const QString &accessToken);
    void start();
    
  private Q_SLOTS:
    void requestDelete(const QUrl &url);
    void requestFinished(QNetworkReply *reply);
    
  private:
    QString m_eventId;
    QString m_calendarId;
    QString m_accessToken;

};

#endif // EVENTDELETEJOB_H
