/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "drivessearchquery.h"
#include "utils.h"

#include <QDateTime>
#include <QString>

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
        return Utils::bool2Str(var.toBool());
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
