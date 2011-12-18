/*
    libKGoogle - Objects - Calendar
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


#ifndef LIBKGOOGLE_OBJECTS_CALENDAR_H
#define LIBKGOOGLE_OBJECTS_CALENDAR_H

#include <libkgoogle/object.h>
#include <libkgoogle/libkgoogle_export.h>

#include <qsharedpointer.h>
#include <qmetatype.h>

#include <kdatetime.h>

namespace KGoogle {

  namespace Objects {

    class CalendarData;

    /**
     * Represents a calendar.
     */
    class LIBKGOOGLE_EXPORT Calendar: public KGoogle::Object
    {

      public:
	typedef QSharedPointer<Calendar> Ptr;
	typedef QList<Calendar> List;

	Calendar();

	Calendar(const Calendar &other);

	~Calendar();

	/**
	 * Returns calendar title (name).
	 */
	QString title();

	/**
	 * Sets a calendar title (name).
	 */
	void setTitle(const QString &title);

	/**
	 * Returns color of the remote calendar. 
	 */
	QString color();

	/**
	 * Sets color of the remote calendar.
	 */
	void setColor(const QString &color);

	/**
	 * Returns detailed description of the calendar.
	 */
	QString details();

	/**
	 * Sets detailed description of a calenar.
	 */
	void setDetails(const QString &details);

	/**
	 * Returns timezone of the calendar.
	 */
	QString timezone();

	/**
	 * Sets timezone of the calendar.
	 */
	void setTimezone(const QString &timezone);

	/**
	 * Returns wheter calendar is editable or read-only.
	 */
	bool editable();

	/**
	 * Sets calendar to read-only or editable.
	 */
	void setEditable(const bool editable);

	/**
	 * Returns date and time when the calendar was created
	 * on the remote server.
	 */
	KDateTime created();

	/**
	 * Sets date and time when the calendar was created on
	 * the remote server.
	 */
	void setCreated(const KDateTime &created);

	/**
	 * Returns date and time when the calendar was last
	 * updated on the remote server.
	 * 
	 * This value does not reflect last update of calendar content,
	 * but just it's properties.
	 */
	KDateTime updated();

	/**
	 * Sets date and time when the calendar properties were last
	 * updated on the remote server. This does not include updated
	 * of calendar content.
	 */
	void setUpdated(const KDateTime &updated);

      protected:
	QExplicitlySharedDataPointer<CalendarData> d;

    };

  } // namespace Objects

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Objects::Calendar)
Q_DECLARE_METATYPE(KGoogle::Objects::Calendar*)
Q_DECLARE_METATYPE(KGoogle::Objects::Calendar::Ptr)
Q_DECLARE_METATYPE(KGoogle::Objects::Calendar::List)

#endif // LIBKGOOGLE_OBJECTS_CALENDAR_H
