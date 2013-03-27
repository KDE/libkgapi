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

#include "change.h"
#include "file_p.h"

#include <QtCore/QVariantMap>

#include <qjson/parser.h>

using namespace KGAPI2;

class DriveChange::Private
{
  public:
    Private();
    Private(const Private &other);

    qlonglong id;
    QString fileId;
    QUrl selfLink;
    bool deleted;
    DriveFilePtr file;

    static DriveChangePtr fromJSON(const  QVariantMap &map);
};

DriveChange::Private::Private():
    id(-1),
    deleted(false)
{
}

DriveChange::Private::Private(const Private &other):
    id(other.id),
    fileId(other.fileId),
    selfLink(other.selfLink),
    deleted(other.deleted),
    file(other.file)
{
}

DriveChangePtr DriveChange::Private::fromJSON(const QVariantMap &map)
{
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#change"))
    {
        return DriveChangePtr();
    }

    DriveChangePtr change(new DriveChange);
    change->d->id = map[QLatin1String("id")].toLongLong();
    change->d->fileId = map[QLatin1String("fileId")].toString();
    change->d->selfLink = map[QLatin1String("selfLink")].toUrl();
    change->d->deleted = map[QLatin1String("deleted")].toBool();
    change->d->file = DriveFile::Private::fromJSON(map[QLatin1String("file")].toMap());

    return change;
}

DriveChange::DriveChange():
    KGAPI2::Object(),
    d(new Private)
{
}

DriveChange::DriveChange(const DriveChange &other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{
}

DriveChange::~DriveChange()
{
    delete d;
}

qlonglong DriveChange::id() const
{
    return d->id;
}

QString DriveChange::fileId() const
{
    return d->fileId;
}

QUrl DriveChange::selfLink() const
{
    return d->selfLink;
}

bool DriveChange::deleted() const
{
    return d->deleted;
}

DriveFilePtr DriveChange::file() const
{
    return d->file;
}

DriveChangePtr DriveChange::fromJSON(const QByteArray &jsonData)
{
    QJson::Parser parser;
    bool ok;

    const QVariant data = parser.parse(jsonData, &ok);
    if (!ok) {
        return DriveChangePtr();
    }

    return Private::fromJSON(data.toMap());
}

DriveChangesList DriveChange::fromJSONFeed(const QByteArray &jsonData, FeedData &feedData)
{
    QJson::Parser parser;
    bool ok;

    const QVariant data = parser.parse(jsonData, &ok);
    if (!ok) {
        return DriveChangesList();
    }

    const QVariantMap map = data.toMap();
    if (!map.contains(QLatin1String("kind")) ||
        map[QLatin1String("kind")].toString() != QLatin1String("drive#changeList"))
    {
        return DriveChangesList();
    }

    if (map.contains(QLatin1String("nextLink"))) {
        feedData.nextPageUrl = map[QLatin1String("nextLink")].toUrl();
    }

    DriveChangesList list;
    const QVariantList items = map[QLatin1String("items")].toList();
    Q_FOREACH (const QVariant &item, items) {
        const DriveChangePtr change = Private::fromJSON(item.toMap());

        if (!change.isNull()) {
            list << change;
        }
    }

    return list;
}
