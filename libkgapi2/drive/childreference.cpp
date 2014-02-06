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

#include "childreference.h"

#include <QtCore/QVariantMap>

#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class ChildReference::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QUrl selfLink;
    QUrl childLink;

    static ChildReferencePtr fromJSON(const QVariantMap &map);
};

ChildReference::Private::Private()
{
}

ChildReference::Private::Private(const Private &other):
    id(other.id),
    selfLink(other.selfLink),
    childLink(other.childLink)
{
}

ChildReferencePtr ChildReference::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("drive#childReference")) {
        return ChildReferencePtr();
    }

    ChildReferencePtr reference(new ChildReference(map[QLatin1String("id")].toString()));
    reference->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    reference->d->childLink = map[QLatin1String("childLink")].toUrl();

    return reference;
}

ChildReference::ChildReference(const QString &id):
    KGAPI2::Object(),
    d(new Private)
{
    d->id = id;
}

ChildReference::ChildReference(const ChildReference &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

ChildReference::~ChildReference()
{
    delete d;
}

QString ChildReference::id() const
{
    return d->id;
}

QUrl ChildReference::selfLink() const
{
    return d->selfLink;
}

QUrl ChildReference::childLink() const
{
    return d->childLink;
}

ChildReferencePtr ChildReference::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return ChildReferencePtr();
    }

    return Private::fromJSON(data.toMap());
}

ChildReferencesList ChildReference::fromJSONFeed(const QByteArray &jsonData,
                                                 FeedData &feedData)
{
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse(jsonData, &ok);

    if (!ok) {
        return ChildReferencesList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
            map[QLatin1String("kind")].toString() != QLatin1String("drive#childList")) {
        return ChildReferencesList();
    }

    ChildReferencesList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH(const QVariant & item, items) {
        ChildReferencePtr reference = Private::fromJSON(item.toMap());

        if (!reference.isNull()) {
            list << reference;
        }
    }

    if (map.contains(QLatin1String("nextLink"))) {
        feedData.nextPageUrl = map[QLatin1String("nextLink")].toUrl();
    }

    return list;
}

QByteArray ChildReference::toJSON(const ChildReferencePtr &reference)
{
    QVariantMap map;

    map[QLatin1String("id")] = reference->id();

    QJson::Serializer serializer;
    return serializer.serialize(map);
}
