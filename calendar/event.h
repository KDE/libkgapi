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


#ifndef EVENT_H
#define EVENT_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QMetaType>


#include <KDE/KCalCore/Event>

namespace Event {

class EventPrivate;

/**
 * @brief An Event represenation.
 */
class Event : public KCalCore::Event
{
  public:
    typedef QList<Event> List;
    typedef QSharedPointer<Event> Ptr;
    
    /**
     * @brief Creates a new Event object
     */
    Event();
    
    /**
     * @brief Copy constructor
     */
    Event(const Event& other);
    
    
    Event(const KCalCore::Event *event);
    
    /**
     * @brief Destructor
     */
    ~Event();
    
    /**
     * @brief Sets the event as deleted.
     */
    void setDeleted(const bool deleted);
    
    /**
     * @brief Returns whter the event is deleted.
     * 
     * @return TRUE when event is deleted 
     */
    bool deleted();
    
    /**
     * @brief Set event ID.
     * 
     * @param id Unique ID of event on Google servers
     */
    void setId(const QString &id);
    
    /**
     * @brief Returns event ID.
     * 
     * @return ID of an event in Google Calendar
     */
    QString id();
    
    /**
     * @brief Sets event etag.
     * 
     * @param etag etag of an event.
     */
    void setETag(const QString &etag);
    
    /**
     * @brief Returns etag of an event.
     * 
     * @return Etag of an event.
     */
    QString etag();
    
    /**
     * @brief Parses input GData JSON data and fills the event 
     * object.
     */
    void fromJSON(QVariantMap jsonData);
    
    /**
     * @brief Returns content of the event encoding as JSON
     * data compatible with GData protocol.
     */
    QVariantMap toJSON();

    Event& operator=( const Event& other );
    
  private:
    QSharedDataPointer<EventPrivate> d;
  
};

} /* Namespace Event */
  
Q_DECLARE_METATYPE(Event::Event::Ptr)
Q_DECLARE_METATYPE(Event::Event::List)

#endif // EVENT_H
