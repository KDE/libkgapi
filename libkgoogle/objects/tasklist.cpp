/*
    libKGoogle - Objects - Task List
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


#include "tasklist.h"

using namespace KGoogle::Objects;

TaskList::TaskList()
{

}

TaskList::~TaskList()
{

}

void TaskList::setUid(const QString &uid)
{
    m_uid = uid;
}

QString TaskList::uid() const
{
    return m_uid;
}

void TaskList::setTitle(const QString& title)
{
    m_title = title;
}

QString TaskList::title() const
{
    return m_title;
}

