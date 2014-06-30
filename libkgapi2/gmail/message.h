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

#ifndef KGAPI2_GMAIL_MESSAGE_H
#define KGAPI2_GMAIL_MESSAGE_H

#include <libkgapi2/object.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QStringList>

namespace KGAPI2
{
namespace Gmail
{

class KGAPI2_EXPORT Message : public Object
{

public:
    explicit Message();
    virtual ~Message();

    QString id() const;
    void setId(const QString &id);

    QString threadId() const;
    void setThreadId(const QString &threadId);

    QStringList labelIds() const;
    void setLabelIds(const QStringList &labelIds);
    void addLabelId(const QString &labelId);

    QString snippet() const;
    void setSnippet(const QString &snippet);

    ulong historyId() const;
    void setHistoryId(ulong historyId);

    QString partId() const;
    void setPartId(const QString &partId) const;

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    QString filename() const;
    void setFilename(const QString &filename);

    QMap<QString, QString> headers() const;
    void setHeaders(const QMap<QString, QString> &headers);
    void addHeader(const QString &header, const QString &value);

    AttachmentPtr body() const;
    void setBody(const AttachmentPtr &body);

    QList<QByteArray> messageParts() const;
    void setMessageParts(const QList<QByteArray> &messageParts);
    void addMessagePart(const QByteArray &messagePart);

    int size() const;
    void setSize(int size);

    QByteArray raw() const;
    void setRaw(const QByteArray &raw);

    static QByteArray toJSON(const MessagePtr &message);
    static MessagePtr fromJSON(const QByteArray &rawData);
    static ObjectsList fromJSONFeed(const QByteArray &rawData, FeedData &feedData);

  private:
    Q_DISABLE_COPY(Message)

    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Gmail
} // namespace KGAPI2

#endif