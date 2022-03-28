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
    enum Field { Name, CreatedTime, MemberCount, OrganizerCount };

    using SearchQuery::SearchQuery;

    using SearchQuery::addQuery;
    void addQuery(Field field, CompareOperator op, const QVariant &value);

private:
    QString fieldToString(Field field);
    QString valueToString(TeamdriveSearchQuery::Field field, const QVariant &var);
};
}
}
