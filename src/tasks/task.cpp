/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "task.h"
#include "../debug.h"

using namespace KGAPI2;


class Q_DECL_HIDDEN Task::Private
{

  public:
    Private() = default;
    Private(const Private &other) = default;

    bool deleted = false;
};


Task::Task():
    Object(),
    KCalendarCore::Todo(),
    d(new Private)
{
}

Task::Task(const Task& other):
    Object(other),
    KCalendarCore::Todo(other),
    d(new Private(*(other.d.get())))
{
}

Task::Task(const KCalendarCore::Todo &other):
    Object(),
    KCalendarCore::Todo(other),
    d(new Private)
{
}

Task::~Task() = default;

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
