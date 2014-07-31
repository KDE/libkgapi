/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "thread.h"

using namespace KGAPI2;
using namespace KGAPI2::Gmail;

class Thread::Private
{
public:
    Private();
    ~Private();

    QString id;
    QString snippet;
    ulong historyId;
    MessagesList messages;
};

Thread::Private::Private()
    : historyId(0)
{
}

Thread::Private::~Private()
{
}

Thread::Thread()
    : Object()
    , d(new Private)
{
}

Thread::~Thread()
{
    delete d;
}

QString Thread::id() const
{
    return d->id;
}

void Thread::setId(const QString &id)
{
    d->id = id;
}

QString Thread::snippet() const
{
    return d->snippet;
}

void Thread::setSnippet(const QString &snippet)
{
    d->snippet = snippet;
}

ulong Thread::historyId() const
{
    return d->historyId;
}

void Thread::setHistoryId(ulong historyId)
{
    d->historyId = historyId;
}

MessagesList Thread::messages() const
{
    return d->messages;
}

void Thread::setMessages(const MessagesList &messages)
{
    d->messages = messages;
}

