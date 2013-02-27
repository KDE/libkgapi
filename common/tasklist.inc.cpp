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

class TaskList::Private
{
  public:
    Private();
    Private(const Private &other);

    QString title;
    QString uid;
};


TaskList::Private::Private()
{
}

TaskList::Private::Private (const Private& other):
    title(other.title),
    uid(other.uid)
{
}

TaskList::TaskList():
    Object(),
    d(new Private)
{
}

TaskList::TaskList (const TaskList& other):
    Object(other),
    d(new Private(*(other.d)))
{
}

TaskList::~TaskList()
{
    delete d;
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
