/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "kgapidrive_export.h"

#include "searchquery.h"

#include <QSharedDataPointer>
#include <QVariant>

namespace KGAPI2
{
namespace Drive
{

/**
 * DrivesSearchQuery class allows simply building even complex shared drive search queries
 * for DrivesFetchJob.
 *
 * See https://developers.google.com/drive/api/v2/search-shareddrives for allowed
 * combinations of fields, compare operators, and value types.
 */
class KGAPIDRIVE_EXPORT DrivesSearchQuery : public SearchQuery
{
public:
    enum Field { Name, Hidden, CreatedDate, MemberCount, OrganizerCount };

    using SearchQuery::SearchQuery;

    using SearchQuery::addQuery;
    void addQuery(Field field, CompareOperator op, const QVariant &value);

private:
    QString fieldToString(Field field);
    QString valueToString(DrivesSearchQuery::Field field, const QVariant &var);
};
}
}
