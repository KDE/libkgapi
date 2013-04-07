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


#include "reminder.h"

using namespace KGAPI2;

#include "common/reminder.inc.cpp"

// In LibKGAPI1 we return AlarmPtr
Alarm* Reminder::toAlarm (Incidence* incidence) const
{
    Alarm* alarm = new Alarm(incidence);

    alarm->setType(d->type);
    alarm->setStartOffset(d->offset);

    return alarm;
}

