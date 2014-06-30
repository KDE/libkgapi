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

#ifndef KGAPI2_GMAIL_ATTACHMENT_H
#define KGAPI2_GMAIL_ATTACHMENT_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QStringList>

namespace KGAPI2
{
namespace Gmail
{

class Message;

class KGAPI2_EXPORT Attachment : public Object
{
public:
    explicit Attachment();
    virtual ~Attachment();

    QString id() const;
    void setId(const QString &setId);

    int size() const;
    void setSize(int size);

    QByteArray data() const;
    void setData(const QByteArray &data);

    static AttachmentPtr fromJSON(const QByteArray &rawData);

  private:
    Q_DISABLE_COPY(Attachment)

    class Private;
    Private * const d;
    friend class Private;

};

} // namespace Gmail
} // namespace KGAPI2