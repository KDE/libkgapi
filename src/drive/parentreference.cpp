/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "parentreference.h"
#include "parentreference_p.h"
#include "utils_p.h"

#include <QJsonDocument>
#include <QVariantMap>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

ParentReference::Private::Private()
    : isRoot(false)
{
}

ParentReference::Private::Private(const Private &other)
    : id(other.id)
    , selfLink(other.selfLink)
    , parentLink(other.parentLink)
    , isRoot(other.isRoot)
{
}

ParentReferencePtr ParentReference::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#parentReference")) {
        return ParentReferencePtr();
    }

    ParentReferencePtr reference(new ParentReference(map[QStringLiteral("id")].toString()));
    reference->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    reference->d->parentLink = map[QStringLiteral("parentLink")].toUrl();
    reference->d->isRoot = map[QStringLiteral("isRoot")].toBool();

    return reference;
}

QVariantMap ParentReference::Private::toJSON(const ParentReferencePtr &reference)
{
    QVariantMap map;
    if (!reference->d->id.isEmpty()) {
        map[QStringLiteral("id")] = reference->id();
    }
    if (!reference->d->selfLink.isEmpty()) {
        map[QStringLiteral("selfLink")] = reference->d->selfLink;
    }
    if (!reference->d->parentLink.isEmpty()) {
        map[QStringLiteral("parentLink")] = reference->d->parentLink;
    }
    if (reference->d->isRoot) { // default is false
        map[QStringLiteral("isRoot")] = reference->d->isRoot;
    }

    return map;
}

ParentReference::ParentReference(const QString &id)
    : KGAPI2::Object()
    , d(new Private)
{
    d->id = id;
}

ParentReference::ParentReference(const ParentReference &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

ParentReference::~ParentReference()
{
    delete d;
}

bool ParentReference::operator==(const ParentReference &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }
    GAPI_COMPARE(id)
    GAPI_COMPARE(selfLink)
    GAPI_COMPARE(parentLink)
    GAPI_COMPARE(isRoot)
    return true;
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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ParentReferencePtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

ParentReferencesList ParentReference::fromJSONFeed(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ParentReferencesList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1StringView("drive#parentList")) {
        return ParentReferencesList();
    }

    ParentReferencesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : items) {
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

    QJsonDocument document = QJsonDocument::fromVariant(map);
    return document.toJson(QJsonDocument::Compact);
}
