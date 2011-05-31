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


#ifndef CALENDARLISTJOB_H
#define CALENDARLISTJOB_H

#include <KDE/KJob>

#include <QtCore/QMap>
#include <QtNetwork/QNetworkReply>

/**
 * @brief KJob for fetching list of calendars.
 * 
 * The calendars are available via \calendars() function
 * as list of pair calendarId - calendar name.
 */
class CalendarListJob : public KJob
{
  Q_OBJECT
  public:
    /**
     * @brief Create a new job.
     * 
     * @param accessToken Access token for authorizing requests
     */
    CalendarListJob(const QString &accessToken);
    
    virtual ~CalendarListJob() {};

    /**
     * @brief Starts the job
     */
    void start();
    
    /**
     * @brief Returns list of pairs calendarId - calendar name fetched
     * 	      from Google.
     * 
     * @return List of pairs calendarId - calendar name
     */
    QMap<QString,QString> calendars() { return m_calendars; };
    
  private Q_SLOTS:
    void requestData(const QUrl &url);
    void requestFinished(QNetworkReply*);
    
  private:
    QString m_accessToken;
    QMap<QString,QString> m_calendars;

};

#endif // CALENDARLISTJOB_H
