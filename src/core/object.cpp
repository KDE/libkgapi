/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "object.h"
#include "../debug.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN Object::Private
{
  public:
    Private();
    Private(const Private& other);

    QString etag;
};

Object::Private::Private()
{
}

Object::Private::Private(const Private& other):
    etag(other.etag)
{
}

Object::Object():
    d(new Private())
{
}

Object::Object(const Object& other):
    d(new Private(*(other.d)))
{
}

Object::~Object()
{
    delete d;
}

bool Object::operator==(const Object &other) const
{
    if (d->etag != other.d->etag) {
        qCDebug(KGAPIDebug) << "ETags don't match";
        return false;
    }
    return true;
}


void Object::setEtag(const QString& etag)
{
    d->etag = etag;
}

QString Object::etag() const
{
    return d->etag;
}
