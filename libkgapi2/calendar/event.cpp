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

#include "event.h"

using namespace KGAPI2;

class Event::Private
{
  public:
    explicit Private();
    Private(const Private &other);

    bool deleted;
    bool useDefaultReminders;
};

Event::Private::Private():
    deleted(false),
    useDefaultReminders(false)
{
}

Event::Private::Private(const Private &other):
    deleted(other.deleted),
    useDefaultReminders(other.useDefaultReminders)
{
}

Event::Event():
    Object(),
    KCalCore::Event(),
    d(new Private)
{
}

Event::Event(const Event &other):
    Object(other),
    KCalCore::Event(other),
    d(new Private(*(other.d)))
{
}

Event::Event(const KCalCore::Event &other):
    Object(),
    KCalCore::Event(other),
    d(new Private)
{
}

Event::~Event()
{
    delete d;
}

bool Event::deleted() const
{
    return d->deleted;
}

bool Event::useDefaultReminders() const
{
    return d->useDefaultReminders;
}

void Event::setDeleted(bool deleted)
{
    d->deleted = deleted;
}

void Event::setUseDefaultReminders(bool useDefault)
{
    d->useDefaultReminders = useDefault;
}
