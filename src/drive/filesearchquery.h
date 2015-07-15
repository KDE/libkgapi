/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef KGAPI2_DRIVE_FILESEARCHQUERY_H
#define KGAPI2_DRIVE_FILESEARCHQUERY_H

#include "kgapidrive_export.h"

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
class KGAPIDRIVE_EXPORT FileSearchQuery
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

    enum CompareOperator {
        Contains,
        Equals,
        NotEquals,
        Less,
        LessOrEqual,
        Greater,
        GreaterOrEqual,
        In,
        Has
    };

    enum LogicOperator {
        And,
        Or
    };

    explicit FileSearchQuery(LogicOperator op = And);
    FileSearchQuery(const FileSearchQuery &other);
    FileSearchQuery &operator=(const FileSearchQuery &other);
    ~FileSearchQuery();

    void addQuery(Field field, CompareOperator op, const QVariant &value);
    void addQuery(const FileSearchQuery &query);

    bool isEmpty() const;

    QString serialize() const;

  private:
    class Private;
    QSharedDataPointer<Private> d;
};
}
}

#endif // KGAPI2_DRIVE_FILESEARCHQUERY_H
