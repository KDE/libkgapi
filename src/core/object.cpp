/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "object.h"
#include "debug.h"

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
