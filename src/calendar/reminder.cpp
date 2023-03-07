/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "reminder.h"
#include "debug.h"

using namespace KGAPI2;
using namespace KCalendarCore;

class Q_DECL_HIDDEN Reminder::Private
{
public:
    Alarm::Type type = Alarm::Invalid;
    Duration offset;
};

Reminder::Reminder()
    : d(new Private)
{
}

Reminder::Reminder(Alarm::Type type, const Duration &startOffset)
    : d(new Private)
{
    d->type = type;
    d->offset = startOffset;
}

Reminder::Reminder(const Reminder &other)
    : d(new Private(*(other.d)))
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
