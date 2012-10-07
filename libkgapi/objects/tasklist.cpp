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

#include "tasklist.h"
#include "tasklist_p.h"

using namespace KGAPI::Objects;

TaskListPrivate::TaskListPrivate()
{ }

TaskListPrivate::TaskListPrivate (const TaskListPrivate& other):
    title(other.title),
    uid(other.uid)
{ }

TaskList::TaskList():
    d(new TaskListPrivate)
{ }

TaskList::TaskList (const TaskList& other):
    Object(other),
    d(new TaskListPrivate(*(other.d)))
{ }


TaskList::~TaskList()
{
    delete d;
}

void TaskList::setUid(const QString &uid)
{
    d->uid = uid;
}

QString TaskList::uid() const
{
    return d->uid;
}

void TaskList::setTitle(const QString& title)
{
    d->title = title;
}

QString TaskList::title() const
{
    return d->title;
}

