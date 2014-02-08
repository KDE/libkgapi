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
using namespace KGAPI2::Drive;

ParentReference::Private::Private():
    isRoot(false)
{
}

ParentReference::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    parentLink(other.parentLink),
    isRoot(other.isRoot)
{
}

ParentReferencePtr ParentReference::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("drive#parentReference")) {
        return ParentReferencePtr();
    }

    ParentReferencePtr reference(new ParentReference(map[QLatin1String("id")].toString()));
    reference->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    reference->d->parentLink = map[QLatin1String("parentLink")].toUrl();
    reference->d->isRoot = map[QLatin1String("isRoot")].toBool();

    return reference;
}

QVariantMap ParentReference::Private::toJSON(const ParentReferencePtr &reference)
{
    QVariantMap map;
    map[QLatin1String("id")] = reference->id();
    map[QLatin1String("selfLink")] = reference->d->selfLink;
    map[QLatin1String("parentLink")] = reference->d->parentLink;
    map[QLatin1String("isRoot")] = reference->d->isRoot;

    return map;
}

ParentReference::ParentReference(const QString &id):
    KGAPI2::Object(),
    d(new Private)
{
    d->id = id;
}

ParentReference::ParentReference(const ParentReference &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

ParentReference::~ParentReference()
{
    delete d;
}

QString ParentReference::id() const
{
    return d->id;
}

QUrl ParentReference::selfLink() const
{
    return d->selfLink;
}

QUrl ParentReference::parentLink() const
{
    return d->parentLink;
}

bool ParentReference::isRoot() const
{
    return d->isRoot;
}

ParentReferencePtr ParentReference::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return ParentReferencePtr();
    }

    return Private::fromJSON(data.toMap());
}

ParentReferencesList ParentReference::fromJSONFeed(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return ParentReferencesList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("kind#parentList")) {
        return ParentReferencesList();
    }

    ParentReferencesList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH(const QVariant & item, items) {
        const ParentReferencePtr reference = Private::fromJSON(item.toMap());

        if (!reference.isNull()) {
            list << reference;
        }
    }

    return list;
}

QByteArray ParentReference::toJSON(const ParentReferencePtr &reference)
{
    const QVariantMap map = Private::toJSON(reference);

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
