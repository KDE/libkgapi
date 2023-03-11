/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
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
 * FileSearchQuery class allows simply building even complex file search queries
 * for FileFetchJob.
 *
 * See https://developers.google.com/drive/web/search-parameters for allowed
 * combinations of fields, compare operators, and value types.
 *
 * @since 2.3
 */
class KGAPIDRIVE_EXPORT FileSearchQuery : public SearchQuery
{
public:
    enum Field {
        Title,
        FullText,
        MimeType,
        ModifiedDate,
        LastViewedByMeDate,
        Trashed,
        Starred,
        Parents,
        Owners,
        Writers,
        Readers,
        SharedWithMe,
        /*Properties            FIXME: Not supported atm */
    };

    using SearchQuery::SearchQuery;

    using SearchQuery::addQuery;
    void addQuery(Field field, CompareOperator op, const QVariant &value);

private:
    Q_REQUIRED_RESULT QString fieldToString(Field field);
    Q_REQUIRED_RESULT QString valueToString(FileSearchQuery::Field field, const QVariant &var);
};
}
}
