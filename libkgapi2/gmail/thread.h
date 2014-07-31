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

#ifndef KGAPI2_GMAIL_THREAD_H
#define KGAPI2_GMAIL_THREAD_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QStringList>

namespace KGAPI2
{

namespace Gmail
{

class KGAPI2_EXPORT Thread : public Object
{

public:
    explicit Thread();
    virtual ~Thread();

    QString id() const;
    void setId(const QString &id);

    QString snippet() const;
    void setSnippet(const QString &snippet);

    ulong historyId() const;
    void setHistoryId(ulong historyId);

    MessagesList messages() const;
    void setMessages(const MessagesList &messages);

private:
    class Private;
    Private * const d;
    friend class Private;
};

}

}

#endif