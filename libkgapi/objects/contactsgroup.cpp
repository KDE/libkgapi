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

#include "contactsgroup.h"
#include "contactsgroup_p.h"

using namespace KGAPI::Objects;

ContactsGroupPrivate::ContactsGroupPrivate():
    isSystemGroup(false)
{ }

ContactsGroupPrivate::ContactsGroupPrivate(const ContactsGroupPrivate &other):
    QSharedData(other),
    id(other.id),
    title(other.title),
    content(other.content),
    updated(other.updated),
    isSystemGroup(other.isSystemGroup)
{ }

ContactsGroupPrivate::~ContactsGroupPrivate()
{ }

ContactsGroup::ContactsGroup():
    KGAPI::Object(),
    d(new ContactsGroupPrivate)
{ }

ContactsGroup::~ContactsGroup()
{ }


ContactsGroup::ContactsGroup(const ContactsGroup &other):
    KGAPI::Object(other),
    d(other.d)
{ }

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

