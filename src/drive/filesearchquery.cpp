/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 * Copyright (C) 2019  David Barchiesi <david@barchie.si>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "filesearchquery.h"
#include "utils.h"

#include <QString>
#include <QDateTime>

using namespace KGAPI2;
using namespace KGAPI2::Drive;


QString FileSearchQuery::fieldToString(Field field)
{
    switch (field) {
    case Title:
        return QStringLiteral("title");
    case FullText:
        return QStringLiteral("fullText");
    case MimeType:
        return QStringLiteral("mimeType");
    case ModifiedDate:
        return QStringLiteral("modifiedDate");
    case LastViewedByMeDate:
        return QStringLiteral("lastViewedByMeDate");
    case Trashed:
        return QStringLiteral("trashed");
    case Starred:
        return QStringLiteral("starred");
    case Parents:
        return QStringLiteral("parents");
    case Owners:
        return QStringLiteral("owners");
    case Writers:
        return QStringLiteral("writers");
    case Readers:
        return QStringLiteral("readers");
    case SharedWithMe:
        return QStringLiteral("sharedWithMe");
    }

    Q_ASSERT(false);
    return QString();
}

QString FileSearchQuery::valueToString(FileSearchQuery::Field field, const QVariant &var)
{
    switch (field) {
    case Title:
    case FullText:
    case MimeType:
    case Parents:
    case Owners:
    case Writers:
    case Readers:
        return QStringLiteral("'%1'").arg(var.toString().replace(QLatin1Char('\''), QLatin1String("\\\'")));
    case ModifiedDate:
    case LastViewedByMeDate:
        return QStringLiteral("'%1'").arg(var.toDateTime().toUTC().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss")));
    case Trashed:
    case Starred:
    case SharedWithMe:
        return Utils::bool2Str(var.toBool());
    }

    Q_ASSERT(false);
    return QString();
}

void FileSearchQuery::addQuery(FileSearchQuery::Field field, FileSearchQuery::CompareOperator op, const QVariant &value)
{
    switch (field) {
    case Title:
    case MimeType:
        Q_ASSERT(op == Contains || op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<QString>());
        break;
    case FullText:
        Q_ASSERT(op == Contains);
        Q_ASSERT(value.canConvert<QString>());
        break;
    case ModifiedDate:
    case LastViewedByMeDate:
        Q_ASSERT(op == LessOrEqual || op == Less || op == Equals || op == NotEquals || op == Greater || op == GreaterOrEqual);
        Q_ASSERT(value.canConvert<QDateTime>());
        break;
    case Trashed:
    case Starred:
    case SharedWithMe:
        Q_ASSERT(op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<bool>());
        break;
    case Parents:
    case Owners:
    case Writers:
    case Readers:
        Q_ASSERT(op == In);
        Q_ASSERT(value.canConvert<QString>());
        break;
    }

    SearchQuery::addQuery(fieldToString(field), op, valueToString(field, value));
}
