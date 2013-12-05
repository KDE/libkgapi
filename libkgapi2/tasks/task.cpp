/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "task.h"

using namespace KGAPI2;


class Task::Private
{

  public:
    Private();
    Private (const Private&);

    bool deleted;
};

Task::Private::Private():
    deleted(false)
{
}

Task::Private::Private(const Private &other):
    deleted(other.deleted)
{
}

Task::Task():
    Object(),
    KCalCore::Todo(),
    d(new Private)
{
}

Task::Task(const Task& other):
    Object(other),
    KCalCore::Todo(other),
    d(new Private(*(other.d)))
{
}

Task::Task(const KCalCore::Todo &other):
    Object(),
    KCalCore::Todo(other),
    d(new Private)
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
