/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactsgroup.h"
#include "debug.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactsGroup::Private
{
public:
    Private();

    ~Private();

    QString id;
    QString title;
    QString content;
    QDateTime updated;
    bool isSystemGroup = false;
};

ContactsGroup::Private::Private()
{
}

ContactsGroup::Private::~Private()
{
}

ContactsGroup::ContactsGroup()
    : Object()
    , d(new Private)
{
}

ContactsGroup::ContactsGroup(const ContactsGroup &other)
    : Object(other)
    , d(new Private(*(other.d)))
{
}

ContactsGroup::~ContactsGroup()
{
    delete d;
}

bool ContactsGroup::operator==(const ContactsGroup &other) const
{
    if (d->id != other.d->id) {
        qCDebug(KGAPIDebug) << "IDs don't match";
        return false;
    }
    if (d->title != other.d->title) {
        qCDebug(KGAPIDebug) << "Titles don't match";
        return false;
    }
    if (d->content != other.d->content) {
        qCDebug(KGAPIDebug) << "Contents don't match";
        return false;
    }
    if (d->updated != other.d->updated) {
        qCDebug(KGAPIDebug) << "Updated does not match";
        return false;
    }
    if (d->isSystemGroup != other.d->isSystemGroup) {
        qCDebug(KGAPIDebug) << "SystemGroup does not match";
        return false;
    }

    return true;
}

void ContactsGroup::setId(const QString &id)
{
    d->id = id;
}

QString ContactsGroup::id() const
{
    return d->id;
}

void ContactsGroup::setTitle(const QString &title)
{
    d->title = title;
}

QString ContactsGroup::title() const
{
    return d->title;
}

void ContactsGroup::setContent(const QString &content)
{
    d->content = content;
}

QString ContactsGroup::content() const
{
    return d->content;
}

void ContactsGroup::setUpdated(const QDateTime &updated)
{
    d->updated = updated;
}

QDateTime ContactsGroup::updated() const
{
    return d->updated;
}

void ContactsGroup::setIsSystemGroup(const bool isSystemGroup)
{
    d->isSystemGroup = isSystemGroup;
}

bool ContactsGroup::isSystemGroup() const
{
    return d->isSystemGroup;
}
