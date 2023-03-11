/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "kgapidrive_export.h"

#include <QSharedDataPointer>
#include <QVariant>

namespace KGAPI2
{
namespace Drive
{

/**
 * SearchQuery class should be used as a base class for building file/team search queries.
 */
class KGAPIDRIVE_EXPORT SearchQuery
{
public:
    enum CompareOperator { Contains, Equals, NotEquals, Less, LessOrEqual, Greater, GreaterOrEqual, In, Has };

    enum LogicOperator { And, Or };

    SearchQuery(LogicOperator op = And);
    SearchQuery(const SearchQuery &other);
    ~SearchQuery();

    SearchQuery &operator=(const SearchQuery &other);

    void addQuery(const QString &field, CompareOperator op, const QString &value);
    void addQuery(const SearchQuery &query);

    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT QString serialize() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};
}
}
