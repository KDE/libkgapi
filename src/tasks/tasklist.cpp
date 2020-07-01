/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "tasklist.h"
#include "../debug.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskList::Private
{
  public:
    Private() = default;
    Private(const Private &other) = default;

    QString title;
    QString selfLink;
    QString updated;
    QString uid;
};

TaskList::TaskList():
    Object(),
    d(new Private)
{
}

TaskList::TaskList (const TaskList& other):
    Object(other),
    d(new Private(*(other.d.get())))
{
}

TaskList::~TaskList() = default;

bool TaskList::operator==(const TaskList &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }

    if (d->uid != other.d->uid) {
        qCDebug(KGAPIDebug) << "UIDs don't match";
        return false;
    }

    if (d->title != other.d->title) {
        qCDebug(KGAPIDebug) << "Titles don't match";
        return false;
    }

    return true;
}

void TaskList::setUid(const QString &uid)
{
    d->uid = uid;
}

QString TaskList::uid() const
{
    return d->uid;
}

void TaskList::setTitle(const QString& title)
{
    d->title = title;
}

QString TaskList::title() const
{
    return d->title;
}

void TaskList::setSelfLink(const QString& selfLink)
{
    d->selfLink = selfLink;
}

QString TaskList::selfLink() const
{
    return d->selfLink;
}

void TaskList::setUpdated(const QString& updated)
{
    d->updated = updated;
}

QString TaskList::updated() const
{
    return d->updated;
}
