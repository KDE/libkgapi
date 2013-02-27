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

class Object::Private
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

void Object::setEtag(const QString& etag)
{
    d->etag = etag;
}

QString Object::etag() const
{
    return d->etag;
}
