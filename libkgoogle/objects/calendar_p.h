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

#include <qshareddata.h>
#include <qstring.h>

#include <kdatetime.h>

#ifdef WITH_KCAL
#include <kcal/alarm.h>
using namspace KCal;
#else
#include <kcalcore/alarm.h>
using namespace KCalCore;
#endif

namespace KGoogle
{

  namespace Objects
  {

    class CalendarData: public QSharedData
    {
      public:
        CalendarData() {};
        CalendarData (const CalendarData &other);
        ~CalendarData() {};

        QString uid;
        QString title;
        QString details;
        QString timezone;
        QString location;
        bool editable;

        Alarm::List alarms;
    };

  } // namespace Objects

} // namespace KGoogle
