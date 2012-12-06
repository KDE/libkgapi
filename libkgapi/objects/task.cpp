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

#include "task.h"
#include "task_p.h"

using namespace KGAPI::Objects;

TaskPrivate::TaskPrivate():
    deleted(false)
{

}

TaskPrivate::TaskPrivate(const TaskPrivate &other):
    deleted(other.deleted)
{

}

Task::Task():
    d(new TaskPrivate)
{

}

Task::Task(const Task& other):
    KGAPI::Object(other),
#ifdef WITH_KCAL
    KCal::Todo(other),
#else
    KCalCore::Todo(other),
#endif
    d(new TaskPrivate(*(other.d)))
{

}

#ifdef WITH_KCAL
Task::Task(const KCal::Todo &other):
    KCal::Todo(other),
#else
Task::Task(const KCalCore::Todo &other):
    KCalCore::Todo(other),
#endif
    d(new TaskPrivate)
{

}

Task::~Task()
{
    delete d;
}

void Task::setDeleted(const bool deleted)
{
    d->deleted = deleted;
}

bool Task::deleted() const
{
    return d->deleted;
}
