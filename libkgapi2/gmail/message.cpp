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

#include "message.h"
#include "attachment.h"

#include <QVariant>

#include <qjson/serializer.h>
#include <qjson/parser.h>

using namespace KGAPI2;
using namespace KGAPI2::Gmail;

class Message::Private
{
public:
    Private();
    ~Private();

    static QVariant toJSON(const MessagePtr &message);
    static MessagePtr fromJSON(const QVariant &data);

    QString id;
    QString threadId;
    QStringList labelIds;
    QString snippet;
    ulong historyId;
    QString partId;
    QString mimeType;
    QString filename;
    QMap<QString, QString> headers;
    AttachmentPtr body;
    QList<QByteArray> messageParts;
    int size;
    QByteArray raw;
};

Message::Private::Private()
    : historyId(0)
    , size(0)
{
}

Message::Private::~Private()
{
}

Message::Message()
    : Object()
    , d(new Private)
{
}

Message::~Message()
{
    delete d;
}

QString Message::id() const
{
    return d->id;
}

void Message::setId(const QString &id)
{
    d->id = id;
}

QString Message::threadId() const
{
    return d->threadId;
}

void Message::setThreadId(const QString &threadId)
{
    d->threadId = threadId;
}

QStringList Message::labelIds() const
{
    return d->labelIds;
}

void Message::setLabelIds(const QStringList &labelIds)
{
    d->labelIds = labelIds;
}

void Message::addLabelId(const QString &labelId)
{
    d->labelIds.append(labelId);
}

QString Message::snippet() const
{
    return d->snippet;
}

void Message::setSnippet(const QString &snippet)
{
    d->snippet = snippet;
}

ulong Message::historyId() const
{
    return d->historyId;
}

void Message::setHistoryId(ulong historyId)
{
    return d->historyId;
}

QString Message::partId() const
{
    return d->partId;
}

void Message::setPartId(const QString &partId) const
{
    d->partId = partId;
}

QString Message::mimeType() const
{
    return d->mimeType;
}

void Message::setMimeType(const QString &mimeType)
{
    d->mimeType = mimeType;
}

QString Message::filename() const
{
    return d->filename;
}

void Message::setFilename(const QString &filename)
{
    d->filename = filename;
}

QMap<QString, QString> Message::headers() const
{
    return d->headers;
}

void Message::setHeaders(const QMap<QString, QString> &headers)
{
    d->headers = headers;
}

void Message::addHeader(const QString &header, const QString &value)
{
    d->headers[header] = value;
}

AttachmentPtr Message::body() const
{
    return d->body;
}

void Message::setBody(const AttachmentPtr &body)
{
    d->body = body;
}

QList<QByteArray> Message::messageParts() const
{
    return d->messageParts;
}

void Message::setMessageParts(const QList<QByteArray> &messageParts)
{
    d->messageParts = messageParts;
}

void Message::addMessagePart(const QByteArray &messagePart)
{
    d->messageParts.append(messagePart);
}

int Message::size() const
{
    return d->size;
}

void Message::setSize(int size)
{
    d->size = size;
}

QByteArray Message::raw() const
{
    return d->raw;
}

void Message::setRaw(const QByteArray &raw)
{
    d->raw = raw;
}

MessagePtr Message::Private::fromJSON(const QVariant &data)
{
    MessagePtr message(new Message);
    const QVariantMap map = data.toMap();

    message->d->id = map[QLatin1String("id")].toString();
    message->d->threadId = map[QLatin1String("threadId")].toString();
    message->d->labelIds = map[QLatin1String("labelIds")].toStringList();
    message->d->snippet = map[QLatin1String("snippet")].toString();
    message->d->historyId = map[QLatin1String("historyId")].toULongLong();
    const QVariantMap payload = map[QLatin1String("payload")].toMap();
    message->d->partId = payload[QLatin1String("partId")].toString();
    message->d->mimeType = payload[QLatin1String("mimeType")].toString();
    message->d->filename = payload[QLatin1String("filename")].toString();
    const QVariantList headers = payload[QLatin1String("headers")].toList();
    Q_FOREACH (const QVariant &val,  headers) {
        const QVariantMap header = val.toMap();
        message->d->headers[header[QLatin1String("name")].toString()] = header[QLatin1String("value")].toString();
    }
    message->d->body = Attachment::fromJSON(payload[QLatin1String("body")]);
    const QVariantList parts = payload[QLatin1String("parts")].toList();
    Q_FOREACH (const QVariant &part,  parts) {
        message->d->messageParts.append(part.toByteArray());
    }
    message->d->size = map[QLatin1String("sizeEstimate")].toInt();
    message->d->raw = map[QLatin1String("raw")].toByteArray();

    return message;
}

QVariant Message::Private::toJSON(const MessagePtr &message)
{
    QVariantMap map;

    // TODO: Convert all,  or just the few fields that matter?
}

MessagePtr Message::fromJSON(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap map = parser.parse(rawData,  &ok).toMap();
    if (!ok || map.isEmpty()) {
        return MessagePtr();
    }

    return Private::fromJSON(map);
}

ObjectsList Message::fromJSONFeed(const QByteArray &rawData, FeedData &feedData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap map = parser.parse(rawData, &ok).toMap();
    if (!ok || map.isEmpty()) {
        return ObjectsList();
    }

    ObjectsList msgs;
    feedData.totalResults = map[QLatin1String("resultSizeEstimate")].toInt();
    if (feedData.requestUrl.isValid()) {
        feedData.nextPageUrl = feedData.requestUrl;
        feedData.nextPageUrl.removeQueryItem(QLatin1String("pageToken"));
        feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"),  map[QLatin1String("nextPageToken")].toString());
    }
    Q_FOREACH (const QVariant msg, map[QLatin1String("messages")].toList()) {
        msgs <<  Private::fromJSON(msg);
    }

    return msgs;
}

QByteArray Message::toJSON(const MessagePtr &message)
{
    const QVariant var = Private::toJSON(message);
    QJson::Serializer serializer;
    return serializer.serialize(var);
}
