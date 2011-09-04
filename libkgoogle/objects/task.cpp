/*
    libKGoogle - Task
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


#include "task.h"

using namespace KGoogle;
using namespace Object;

Task::Task():
  m_deleted(false)
{ }

Task::Task(const Task& other):
  KGoogleObject(other),
#ifdef WITH_KCAL
  KCal::Todo(other),
#else
  KCalCore::Todo(other),
#endif
  m_deleted(other.m_deleted)
{ }

#ifdef WITH_KCAL
Task::Task(const KCal::Todo &other): 
  KCal::Todo(other),
#else
Task::Task(const KCalCore::Todo &other):
  KCalCore::Todo(other),
#endif
  m_deleted(false)
{ }

Task::~Task()
{ }

void Task::setDeleted(const bool deleted)
{
  m_deleted = deleted;
}

bool Task::deleted()
{
  return m_deleted;
}

void Task::setId(const QString& id)
{
  setUid(id);
}

QString Task::id()
{
  return uid();
}

