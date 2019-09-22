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


#include "reminder.h"
#include "../debug.h"

using namespace KGAPI2;
using namespace KCalendarCore;

class Q_DECL_HIDDEN Reminder::Private
{
public:
    Alarm::Type type = Alarm::Invalid;
    Duration offset;
};

Reminder::Reminder():
    d(new Private)
{
}

Reminder::Reminder(const Alarm::Type &type, const Duration& startOffset):
    d(new Private)
{
    d->type = type;
    d->offset = startOffset;
}

Reminder::Reminder(const Reminder& other):
    d(new Private(*(other.d)))
{
}

Reminder::~Reminder() = default;

bool Reminder::operator==(const Reminder &other) const
{
    if (d->type != other.d->type) {
        qCDebug(KGAPIDebug) << "Types don't match";
        return false;
    }
    if (d->offset != other.d->offset) {
        qCDebug(KGAPIDebug) << "Offsets don't match";
        return false;
    }
    return true;
}

void Reminder::setType(Alarm::Type type)
{
    d->type = type;
}

Alarm::Type Reminder::type() const
{
    return d->type;
}

void Reminder::setStartOffset(const Duration &startOffset)
{
    d->offset = startOffset;
}

Duration Reminder::startOffset() const
{
    return d->offset;
}

// In LibKGAPI1 we return AlarmPtr
Alarm *Reminder::toAlarm(Incidence *incidence) const
{
    auto alarm = new Alarm(incidence);
    alarm->setType(d->type);
    alarm->setStartOffset(d->offset);
    return alarm;
}

