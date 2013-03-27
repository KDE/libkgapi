/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "parentreference.h"
#include "parentreference_p.h"

#include <QtCore/QVariantMap>

#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGAPI2;

DriveParentReference::Private::Private():
    isRoot(false)
{
}

DriveParentReference::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    parentLink(other.parentLink),
    isRoot(other.isRoot)
{
}

DriveParentReferencePtr DriveParentReference::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#parentReference"))
    {
        return DriveParentReferencePtr();
    }

    DriveParentReferencePtr reference(new DriveParentReference(map[QLatin1String("id")].toString()));
    reference->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    reference->d->parentLink = map[QLatin1String("parentLink")].toUrl();
    reference->d->isRoot = map[QLatin1String("isRoot")].toBool();

    return reference;
}

QVariantMap DriveParentReference::Private::toJSON(const DriveParentReferencePtr &reference)
{
    QVariantMap map;
    map[QLatin1String("id")] = reference->id();

    return map;
}

DriveParentReference::DriveParentReference(const QString &id):
    KGAPI2::Object(),
    d(new Private)
{
    d->id = id;
}

DriveParentReference::DriveParentReference(const DriveParentReference &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

DriveParentReference::~DriveParentReference()
{
    delete d;
}

QString DriveParentReference::id() const
{
    return d->id;
}

QUrl DriveParentReference::selfLink() const
{
    return d->selfLink;
}

QUrl DriveParentReference::parentLink() const
{
    return d->parentLink;
}

bool DriveParentReference::isRoot() const
{
    return d->isRoot;
}

DriveParentReferencePtr DriveParentReference::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return DriveParentReferencePtr();
    }

    return Private::fromJSON(data.toMap());
}

DriveParentReferencesList DriveParentReference::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return DriveParentReferencesList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("kind#parentList"))
    {
        return DriveParentReferencesList();
    }

    DriveParentReferencesList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const DriveParentReferencePtr reference = Private::fromJSON(item.toMap());

        if (!reference.isNull()) {
            list << reference;
        }
    }

    return list;
}

QByteArray DriveParentReference::toJSON(const DriveParentReferencePtr &reference)
{
    const QVariantMap map = Private::toJSON(reference);

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
