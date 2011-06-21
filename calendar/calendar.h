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


#ifndef CALENDAR_H
#define CALENDAR_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMetaType>

/**
 * @brief A Calendar class provides simple interface for storing
 * 	  all important informations about calendars.
 * 
 * Calendar is registered as a Qt MetaType so that it can be passed
 * as QVariant value for example.
 */
class Calendar: public QObject
{
  Q_OBJECT
  Q_ENUMS(CalendarRoles)
  public:
    /**
     * @brief Custom Qt::ItemDataRole extension
     */
    enum { CalendarIdRole = 1000, /* << Calendar ID string */
	   CalendarRole = 1001 	  /* << Calendar object */
    } CalendarRoles;
    
    /**
     * @brief Create new calendar object
     * 
     * @param title Title (name) of the calendar
     * @param id ID of the calendar
     * @param color Color of the calendar
     */
    Calendar(const QString &title = QString(), 
	     const QString &id = QString(), 
	     const QString &color = QString());

    Q_DISABLE_COPY(Calendar);   
    
    ~Calendar() {};
   
    /**
     * @brief Duplicate current calendar
     * 
     * @return Copy of current calendar
     */
    Calendar* clone() { return new Calendar (m_title, m_id, m_color); }
    
    /**
     * @brief Get title of the calendar
     * 
     * @return Title of calendar
     */
    QString title() { return m_title.isEmpty() ? "Google Calendar" : m_title; };
    
    /**
     * @brief Set title of calendar
     */
    void setTitle(const QString &title) { m_title = title; }
    
    /**
     * @brief Get ID of the calendar.
     * 
     * @return Id of calendar
     */
    QString id() { return m_id.isEmpty() ? "google_calendar" : m_id; }
    
    /**
     * @brief Set calendar ID
     */
    void setId(const QString &id) { m_id = id; }

    
    /**
     * @brief Get color of the calendar
     * 
     * @return Color of calendar 
     */
    QString color() { return m_color.isEmpty() ? "#000000" : m_color; };
    
    /**
     * @brief Set color of the calendar
     */
    void setColor(const QString &color) { m_color = color; }
    
  private:
    QString m_title;
    QString m_id;
    QString m_color;
};

Q_DECLARE_METATYPE(Calendar)
Q_DECLARE_METATYPE(Calendar*)

#endif // CALENDAR_H
