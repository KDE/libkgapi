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

#ifndef KGAPI2_DRIVE_TEAMDRIVESEARCHQUERY_H
#define KGAPI2_DRIVE_TEAMDRIVESEARCHQUERY_H

#include "kgapidrive_export.h"

#include "searchquery.h"

#include <QVariant>
#include <QSharedDataPointer>

namespace KGAPI2
{
namespace Drive
{

/**
 * TeamdriveSearchQuery class allows simply building even complex team drive search queries
 * for TeamdriveFetchJob.
 *
 * See https://developers.google.com/drive/web/search-parameters for allowed
 * combinations of fields, compare operators, and value types.
 *
 * @since 2.3
 */
class KGAPIDRIVE_EXPORT TeamdriveSearchQuery : public SearchQuery
{
public:
    enum Field {
        Name,
        CreatedTime,
        MemberCount,
        OrganizerCount
    };

    using SearchQuery::SearchQuery;

    using SearchQuery::addQuery;
    void addQuery(Field field, CompareOperator op, const QVariant &value);

private:
    QString fieldToString(Field field);
    QString valueToString(TeamdriveSearchQuery::Field field, const QVariant &var);
};
}
}

#endif // KGAPI2_DRIVE_TEAMDRIVESEARCHQUERY_H