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

#include "contactsgroup.h"

using namespace KGAPI2;


class ContactsGroup::Private
{
  public:
    Private();

    ~Private();

    QString id;
    QString title;
    QString content;
    QDateTime updated;
    bool isSystemGroup;

};

ContactsGroup::Private::Private():
    isSystemGroup(false)
{
}

ContactsGroup::Private::~Private()
{
}

ContactsGroup::ContactsGroup():
    Object(),
    d(new Private)
{
}

ContactsGroup::ContactsGroup(const ContactsGroup &other):
    Object(other),
    d(new Private(*(other.d)))
{
}

ContactsGroup::~ContactsGroup()
{
    delete d;
}

void ContactsGroup::setId(const QString &id)
{
    d->id = id;
}

QString ContactsGroup::id() const
{
    return d->id;
}

void ContactsGroup::setTitle(const QString &title)
{
    d->title = title;
}

QString ContactsGroup::title() const
{
    return d->title;
}

void ContactsGroup::setContent(const QString &content)
{
    d->content = content;
}

QString ContactsGroup::content() const
{
    return d->content;
}

void ContactsGroup::setUpdated(const QDateTime &updated)
{
    d->updated = updated;
}

QDateTime ContactsGroup::updated() const
{
    return d->updated;
}

void ContactsGroup::setIsSystemGroup(const bool isSystemGroup)
{
    d->isSystemGroup = isSystemGroup;
}

bool ContactsGroup::isSystemGroup() const
{
    return d->isSystemGroup;
}
