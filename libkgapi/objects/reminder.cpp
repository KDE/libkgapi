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

#include "reminder.h"
#include "reminder_p.h"

using namespace KGAPI::Objects;

ReminderPrivate::ReminderPrivate():
    type(Alarm::Invalid)
{ }

ReminderPrivate::ReminderPrivate(const ReminderPrivate &other):
    type(other.type),
    offset(other.offset)
{ }

Reminder::Reminder():
    d(new ReminderPrivate)
{ }

Reminder::Reminder (const Alarm::Type &type, const Duration& startOffset):
    d(new ReminderPrivate)
{
    d->type = type;
    d->offset = startOffset;
}

Reminder::Reminder (const Reminder& other):
    d(new ReminderPrivate(*(other.d)))
{ }

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

AlarmPtr Reminder::toAlarm (Incidence* incidence) const
{
    AlarmPtr alarm(new Alarm(incidence));

    alarm->setType(d->type);
    alarm->setStartOffset(d->offset);

    return alarm;
}
