/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "event.h"
#include "debug.h"

using namespace KGAPI2;

namespace
{
static constexpr const char *EventIdProperty = "EventId";
static constexpr const char *EventHangoutLinkProperty = "EventHangoutLink";
}

class Q_DECL_HIDDEN Event::Private
{
public:
    QString id;
    bool deleted = false;
    bool useDefaultReminders = false;
};

Event::Event()
    : Object()
    , KCalendarCore::Event()
    , d(new Private)
{
}

Event::Event(const Event &other)
    : Object(other)
    , KCalendarCore::Event(other)
    , d(new Private(*(other.d)))
{
}

Event::Event(const KCalendarCore::Event &other)
    : Object()
    , KCalendarCore::Event(other)
    , d(new Private)
{
}

Event::~Event() = default;

bool Event::operator==(const Event &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }
    if (!KCalendarCore::Event::operator==(other)) {
        return false;
    }
    if (d->deleted != other.d->deleted) {
        qCDebug(KGAPIDebug) << "Deleted does not match";
        return false;
    }
    if (d->useDefaultReminders != other.d->useDefaultReminders) {
        qCDebug(KGAPIDebug) << "UseDefaultReminders does not match";
        return false;
    }

    return true;
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

QString Event::id() const
{
    const QString val = customProperty("LIBKGAPI", EventIdProperty);
    if (val.isEmpty()) {
        // Backwards compatibility: prior to introducing "id", UID was used for
        // remote identification: use it
        return KCalendarCore::Incidence::uid();
    }
    return val;
}

void Event::setId(const QString &id)
{
    setCustomProperty("LIBKGAPI", EventIdProperty, id);
}

QString Event::hangoutLink() const
{
    return customProperty("LIBKGAPI", EventHangoutLinkProperty);
}

void Event::setHangoutLink(const QString &hangoutLink)
{
    setCustomProperty("LIBKGAPI", EventHangoutLinkProperty, hangoutLink);
}