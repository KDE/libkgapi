/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI_OBJECTS_TASK_H
#define LIBKGAPI_OBJECTS_TASK_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

#ifdef WITH_KCAL
#include <KCal/Todo>
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KCal::Todo> TodoPtr;
#else
#include <KCalCore/Todo>
typedef KCalCore::Todo::Ptr TodoPtr;
#endif

namespace KGAPI
{

namespace Objects
{

class TaskData;

#ifdef WITH_KCAL
class LIBKGAPI_EXPORT Task: public KGAPI::Object, public KCal::Todo
#else
class LIBKGAPI_EXPORT Task: public KGAPI::Object, public KCalCore::Todo
#endif
{
  public:
    typedef QList<Task> List;
    typedef QSharedPointer< Task > Ptr;

    Task();
    Task(const Task& other);
#ifdef WITH_KCAL
    Task(const KCal::Todo &other);
#else
    Task(const KCalCore::Todo &other);
#endif
    virtual ~Task();

    void setDeleted(const bool deleted);
    bool deleted() const;

  private:
    QSharedDataPointer< TaskData > d;

};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::Task::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::Task*)
Q_DECLARE_METATYPE(KGAPI::Objects::Task::List)

#endif // LIBKGAPI_OBJECTS_TASK_H
