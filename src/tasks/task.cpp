/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "task.h"
#include "../debug.h"

using namespace KGAPI2;


class Q_DECL_HIDDEN Task::Private
{

  public:
    Private();
    Private (const Private&);

    bool deleted = false;
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
    KCalendarCore::Todo(),
    d(new Private)
{
}

Task::Task(const Task& other):
    Object(other),
    KCalendarCore::Todo(other),
    d(new Private(*(other.d)))
{
}

Task::Task(const KCalendarCore::Todo &other):
    Object(),
    KCalendarCore::Todo(other),
    d(new Private)
{
}

Task::~Task()
{
    delete d;
}

bool Task::operator==(const Task &other) const
{
    if (!Object::operator==(other)) {
        qCDebug(KGAPIDebug) << "Objects don't match";
        return false;
    }
    if (!Todo::operator==(other)) {
        qCDebug(KGAPIDebug) << "Todos don't match";
        return false;
    }
    if (d->deleted != other.d->deleted) {
        qCDebug(KGAPIDebug) << "Deleted does not match";
        return false;
    }

    return true;
}


void Task::setDeleted(const bool deleted)
{
    d->deleted = deleted;
}

bool Task::deleted() const
{
    return d->deleted;
}
