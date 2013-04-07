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

using namespace KCalCore;

class Reminder::Private
{
  public:
    Private();
    Private(const Private &other);
    ~Private() {};

    Alarm::Type type;
    Duration offset;
};

Reminder::Private::Private():
    type(Alarm::Invalid)
{
}

Reminder::Private::Private(const Private &other):
    type(other.type),
    offset(other.offset)
{
}

Reminder::Reminder():
    d(new Private)
{
}

Reminder::Reminder (const Alarm::Type &type, const Duration& startOffset):
    d(new Private)
{
    d->type = type;
    d->offset = startOffset;
}

Reminder::Reminder (const Reminder& other):
    d(new Private(*(other.d)))
{
}

Reminder::~Reminder()
{
    delete d;
}

void Reminder::setType (const Alarm::Type& type)
{
    d->type = type;
}

Alarm::Type Reminder::type() const
{
    return d->type;
}

void Reminder::setStartOffset (const Duration& startOffset)
{
    d->offset = startOffset;
}

Duration Reminder::startOffset() const
{
    return d->offset;
}
