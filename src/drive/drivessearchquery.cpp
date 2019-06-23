/*
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

#include "drivessearchquery.h"

#include <QString>
#include <QDateTime>

using namespace KGAPI2;
using namespace KGAPI2::Drive;


QString DrivesSearchQuery::fieldToString(Field field)
{
    switch (field) {
    case Name:
        return QStringLiteral("name");
    case Hidden:
        return QStringLiteral("hidden");
    case CreatedDate:
        return QStringLiteral("createdDate");
    case MemberCount:
        return QStringLiteral("memberCount");
    case OrganizerCount:
        return QStringLiteral("organizerCount");
    }

    Q_ASSERT(false);
    return QString();
}

QString DrivesSearchQuery::valueToString(DrivesSearchQuery::Field field, const QVariant &var)
{
    switch (field) {
    case Name:
        return QStringLiteral("'%1'").arg(var.toString().replace(QLatin1Char('\''), QLatin1String("\\\'")));
    case Hidden:
        return (var.toBool() == true ? QStringLiteral("true") : QStringLiteral("false"));
    case MemberCount:
    case OrganizerCount:
        return var.toString();
    case CreatedDate:
        return QStringLiteral("'%1'").arg(var.toDateTime().toUTC().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss")));
    }

    Q_ASSERT(false);
    return QString();
}

void DrivesSearchQuery::addQuery(DrivesSearchQuery::Field field, DrivesSearchQuery::CompareOperator op, const QVariant &value)
{
    switch (field) {
    case Name:
        Q_ASSERT(op == Contains || op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<QString>());
        break;
    case Hidden:
        Q_ASSERT(op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<bool>());
        break;
    case MemberCount:
    case OrganizerCount:
        Q_ASSERT(op == LessOrEqual || op == Less || op == Equals || op == NotEquals || op == Greater || op == GreaterOrEqual);
        Q_ASSERT(value.canConvert<int>());
        break;
    case CreatedDate:
        Q_ASSERT(op == LessOrEqual || op == Less || op == Equals || op == NotEquals || op == Greater || op == GreaterOrEqual);
        Q_ASSERT(value.canConvert<QDateTime>());
        break;
    }

    SearchQuery::addQuery(fieldToString(field), op, valueToString(field, value));
}
