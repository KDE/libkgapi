/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "childreference.h"

#include <QVariantMap>
#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChildReference::Private
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
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#childReference")) {
        return ChildReferencePtr();
    }

    ChildReferencePtr reference(new ChildReference(map[QStringLiteral("id")].toString()));
    reference->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    reference->d->childLink = map[QStringLiteral("childLink")].toUrl();

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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ChildReferencePtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

ChildReferencesList ChildReference::fromJSONFeed(const QByteArray &jsonData,
                                                 FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ChildReferencesList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#childList")) {
        return ChildReferencesList();
    }

    ChildReferencesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant & item : items) {
        ChildReferencePtr reference = Private::fromJSON(item.toMap());

        if (!reference.isNull()) {
            list << reference;
        }
    }

    if (map.contains(QStringLiteral("nextLink"))) {
        feedData.nextPageUrl = map[QStringLiteral("nextLink")].toUrl();
    }

    return list;
}

QByteArray ChildReference::toJSON(const ChildReferencePtr &reference)
{
    QVariantMap map;

    map[QStringLiteral("id")] = reference->id();

    QJsonDocument document = QJsonDocument::fromVariant(map);
    return document.toJson(QJsonDocument::Compact);
}
