/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "change.h"
#include "file_p.h"
#include "utils_p.h"

#include <QJsonDocument>
#include <QVariantMap>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN Change::Private
{
public:
    Private();
    Private(const Private &other);

    qlonglong id = -1;
    QString fileId;
    QUrl selfLink;
    bool deleted = false;
    FilePtr file;

    static ChangePtr fromJSON(const QVariantMap &map);
};

Change::Private::Private()
{
}

Change::Private::Private(const Private &other)
    : id(other.id)
    , fileId(other.fileId)
    , selfLink(other.selfLink)
    , deleted(other.deleted)
    , file(other.file)
{
}

ChangePtr Change::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1String("drive#change")) {
        return ChangePtr();
    }

    ChangePtr change(new Change);
    change->d->id = map[QStringLiteral("id")].toLongLong();
    change->d->fileId = map[QStringLiteral("fileId")].toString();
    change->d->selfLink = map[QStringLiteral("selfLink")].toUrl();
    change->d->deleted = map[QStringLiteral("deleted")].toBool();
    change->d->file = File::Private::fromJSON(map[QStringLiteral("file")].toMap());

    return change;
}

Change::Change()
    : KGAPI2::Object()
    , d(new Private)
{
}

Change::Change(const Change &other)
    : KGAPI2::Object(other)
    , d(new Private(*(other.d)))
{
}

Change::~Change()
{
    delete d;
}

bool Change::operator==(const Change &other) const
{
    if (!Object::operator==(other)) {
        return false;
    }

    GAPI_COMPARE(id)
    GAPI_COMPARE(fileId)
    GAPI_COMPARE(selfLink)
    GAPI_COMPARE(deleted)
    GAPI_COMPARE_SHAREDPTRS(file)
    return true;
}

qlonglong Change::id() const
{
    return d->id;
}

QString Change::fileId() const
{
    return d->fileId;
}

QUrl Change::selfLink() const
{
    return d->selfLink;
}

bool Change::deleted() const
{
    return d->deleted;
}

FilePtr Change::file() const
{
    return d->file;
}

ChangePtr Change::fromJSON(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ChangePtr();
    }

    const QVariant data = document.toVariant();
    return Private::fromJSON(data.toMap());
}

ChangesList Change::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return ChangesList();
    }

    const QVariant data = document.toVariant();
    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1StringView("kind")) || map[QStringLiteral("kind")].toString() != QLatin1String("drive#changeList")) {
        return ChangesList();
    }

    if (map.contains(QLatin1StringView("nextLink"))) {
        feedData.nextPageUrl = map[QStringLiteral("nextLink")].toUrl();
    }

    ChangesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant &item : items) {
        const ChangePtr change = Private::fromJSON(item.toMap());

        if (!change.isNull()) {
            list << change;
        }
    }

    return list;
}
