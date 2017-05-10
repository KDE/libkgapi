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

#include "change.h"
#include "file_p.h"

#include <QVariantMap>
#include <QJsonDocument>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN Change::Private
{
  public:
    Private();
    Private(const Private &other);

    qlonglong id;
    QString fileId;
    QUrl selfLink;
    bool deleted;
    FilePtr file;

    static ChangePtr fromJSON(const  QVariantMap &map);
};

Change::Private::Private():
    id(-1),
    deleted(false)
{
}

Change::Private::Private(const Private &other):
    id(other.id),
    fileId(other.fileId),
    selfLink(other.selfLink),
    deleted(other.deleted),
    file(other.file)
{
}

ChangePtr Change::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#change")) {
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

Change::Change():
    KGAPI2::Object(),
    d(new Private)
{
}

Change::Change(const Change &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

Change::~Change()
{
    delete d;
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
    if (!map.contains(QStringLiteral("kind")) ||
            map[QStringLiteral("kind")].toString() != QLatin1String("drive#changeList")) {
        return ChangesList();
    }

    if (map.contains(QStringLiteral("nextLink"))) {
        feedData.nextPageUrl = map[QStringLiteral("nextLink")].toUrl();
    }

    ChangesList list;
    const QVariantList items = map[QStringLiteral("items")].toList();
    for (const QVariant & item : items) {
        const ChangePtr change = Private::fromJSON(item.toMap());

        if (!change.isNull()) {
            list << change;
        }
    }

    return list;
}
