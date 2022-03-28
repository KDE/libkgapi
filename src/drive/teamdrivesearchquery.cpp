/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "teamdrivesearchquery.h"

#include <QDateTime>
#include <QString>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

QString TeamdriveSearchQuery::fieldToString(Field field)
{
    switch (field) {
    case Name:
        return QStringLiteral("name");
    case CreatedTime:
        return QStringLiteral("createdTime");
    case MemberCount:
        return QStringLiteral("memberCount");
    case OrganizerCount:
        return QStringLiteral("organizerCount");
    }

    Q_ASSERT(false);
    return QString();
}

QString TeamdriveSearchQuery::valueToString(TeamdriveSearchQuery::Field field, const QVariant &var)
{
    switch (field) {
    case Name:
        return QStringLiteral("'%1'").arg(var.toString().replace(QLatin1Char('\''), QLatin1String("\\\'")));
    case MemberCount:
    case OrganizerCount:
        return var.toString();
    case CreatedTime:
        return QStringLiteral("'%1'").arg(var.toDateTime().toUTC().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss")));
    }

    Q_ASSERT(false);
    return QString();
}

void TeamdriveSearchQuery::addQuery(TeamdriveSearchQuery::Field field, TeamdriveSearchQuery::CompareOperator op, const QVariant &value)
{
    switch (field) {
    case Name:
        Q_ASSERT(op == Contains || op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<QString>());
        break;
    case MemberCount:
    case OrganizerCount:
        Q_ASSERT(op == LessOrEqual || op == Less || op == Equals || op == NotEquals || op == Greater || op == GreaterOrEqual);
        Q_ASSERT(value.canConvert<int>());
        break;
    case CreatedTime:
        Q_ASSERT(op == LessOrEqual || op == Less || op == Equals || op == NotEquals || op == Greater || op == GreaterOrEqual);
        Q_ASSERT(value.canConvert<QDateTime>());
        break;
    }

    SearchQuery::addQuery(fieldToString(field), op, valueToString(field, value));
}
