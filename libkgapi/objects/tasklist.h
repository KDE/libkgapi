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

#ifndef LIBKGAPI_OBJECTS_TASKLIST_H
#define LIBKGAPI_OBJECTS_TASKLIST_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class LIBKGAPI_EXPORT_DEPRECATED TaskList: public KGAPI::Object
{
  public:
    typedef QSharedPointer< TaskList > Ptr;
    typedef QList< TaskList > List;

    TaskList();
    TaskList (const TaskList& other);

    virtual ~TaskList();

    void setUid(const QString &uid);
    QString uid() const;

    void setTitle(const QString &title);
    QString title() const;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::TaskList::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::TaskList*)
Q_DECLARE_METATYPE(KGAPI::Objects::TaskList::List)

#endif // LIBKGAPI_OBJECTS_TASKLIST_H
