/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "childreference.h"
#include "utils_p.h"

#include <QJsonDocument>
#include <QVariantMap>

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

ChildReference::Private::Private(const Private &other)
    : id(other.id)
    , selfLink(other.selfLink)
    , childLink(other.childLink)
{
}

ChildReferencePtr ChildReference::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#childReference")) {
        return ChildReferencePtr();
    }

    ChildReferencePtr reference(new ChildReference(map[QStringLiteral("id")].toString()));
    reference->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    reference->d->childLink = map[QStringLiteral("childLink")].toUrl();

    return reference;
}

ChildReference::ChildReference(const QString &id)
    : KGAPI2::Object()
    , d(new Private)
{
    d->id = id;
}

ChildReference::ChildReference(const ChildReference &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

ChildReference::~ChildReference()
{
    delete d;
}

bool ChildReference::operator==(const ChildReference &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }
    GAPI_COMPARE(id)
    GAPI_COMPARE(selfLink)
    GAPI_COMPARE(childLink)
    return true;
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

ChildReferencesList ChildReference::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ChildReferencesList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#childList")) {
        return ChildReferencesList();
    }

    ChildReferencesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : items) {
        ChildReferencePtr reference = Private::fromJSON(item.toMap());

        if (!reference.isNull()) {
            list << reference;
        }
    }

    if (map.contains(QLatin1StringView("nextLink"))) {
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
