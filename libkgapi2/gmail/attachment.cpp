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

#include "attachment.h"

#include <qjson/parser.h>

#include <QVariant>

using namespace KGAPI2;
using namespace KGAPI2::Gmail;

class Attachment::Private
{
public:
    Private();
    ~Private();

    int id;
    int size;
    QByteArray data;
};

Attachment::Private::Private()
    : id(-1)
    , size(0)
{
}

Attachment::Private::~Private()
{
}

Attachment::Attachment()
    : Object()
    , d(new Private)
{
}

Attachment::~Attachment()
{
    delete d;
}

QString Attachment::id() const
{
    return d->id;
}

void Attachment::setId(const QString &setId)
{
    d->id = id;
}

int Attachment::size() const
{
    return d->size;
}

void Attachment::setSize(int size)
{
    d->size = size;
}

QByteArray Attachment::data() const
{
    return d->data;
}

void Attachment::setData(const QByteArray &data)
{
    d->data = data;
}

AttachmentPtr Attachment::fromJSON(const QByteArray &rawData)
{
    QJson::Parser parser;
    bool ok = false;
    const QVariantMap map = parser.parse(rawData, &ok).toMap;
    if (!ok || map.isEmpty()) {
        return AttachmentPtr();
    }

    AttachmentPtr atmnt(new Attachment);
    atmnt->d->id = map[QLatin1String("attachmentId")].toInt();
    atmnt->d->size = map[QLatin1String("size")].toInt();
    atmnt->d->data = map[QLatin1String("data")].toByteArray();

    return atmnt;
}
