/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVE_FILESEARCHQUERY_H
#define KGAPI2_DRIVE_FILESEARCHQUERY_H

#include "kgapidrive_export.h"

#include "searchquery.h"

#include <QVariant>
#include <QSharedDataPointer>

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
    QString fieldToString(Field field);
    QString valueToString(FileSearchQuery::Field field, const QVariant &var);
};
}
}

#endif // KGAPI2_DRIVE_FILESEARCHQUERY_H
