/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
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

#include "searchquery.h"

#include <QString>
#include <QDateTime>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN SearchQuery::Private : public QSharedData
{
public:
    Private() = default;
    Private(const Private &other) = default;
    ~Private() = default;

    static QString compareOperatorToString(CompareOperator op);
    static QString logicOperatorToString(LogicOperator op);

    QList<SearchQuery> subqueries;
    QString field;
    QString value;
    CompareOperator compareOp;
    LogicOperator logicOp;
};

QString SearchQuery::Private::compareOperatorToString(CompareOperator op)
{
    switch (op) {
    case Contains:
        return QStringLiteral(" contains ");
    case Equals:
        return QStringLiteral(" = ");
    case NotEquals:
        return QStringLiteral(" != ");
    case Less:
        return QStringLiteral(" < ");
    case LessOrEqual:
        return QStringLiteral(" <= ");
    case Greater:
        return QStringLiteral(" > ");
    case GreaterOrEqual:
        return QStringLiteral(" >= ");
    case In:
        return QStringLiteral(" in ");
    case Has:
        return QStringLiteral(" has ");
    }

    Q_ASSERT(false);
    return QString();
}

QString SearchQuery::Private::logicOperatorToString(SearchQuery::LogicOperator op)
{
    switch (op) {
    case And:
        return QStringLiteral(" and ");
    case Or:
        return QStringLiteral(" or ");
    }

    Q_ASSERT(false);
    return QString();
}

SearchQuery::SearchQuery(SearchQuery::LogicOperator op)
    : d(new Private)
{
    d->logicOp = op;
}

SearchQuery::SearchQuery(const SearchQuery &other) = default;

SearchQuery::~SearchQuery() = default;

SearchQuery &SearchQuery::operator=(const SearchQuery &other) = default;

void SearchQuery::addQuery(const QString &field, SearchQuery::CompareOperator op, const QString &value)
{
    SearchQuery query;
    query.d->field = field;
    query.d->compareOp = op;
    query.d->value = value;
    d->subqueries.append(query);
}

void SearchQuery::addQuery(const SearchQuery &query)
{
    d->subqueries.append(query);
}

bool SearchQuery::isEmpty() const
{
    return d->value.isNull() && d->subqueries.isEmpty();
}

QString SearchQuery::serialize() const
{
    if (isEmpty()) {
        return QString();
    }

    QString r;
    r = QLatin1Char('(');
    if (d->subqueries.isEmpty()) {
        if (d->compareOp == In) {
            r += QStringLiteral("%1 in %2").arg(d->value, d->field);
        } else {
            r += d->field % Private::compareOperatorToString(d->compareOp) % d->value;
        }
    } else {
        QList<SearchQuery>::ConstIterator iter, end;
        for (iter = d->subqueries.constBegin(), end = d->subqueries.constEnd(); iter != end; ++iter) {
            if (iter != d->subqueries.constBegin()) {
                r += Private::logicOperatorToString(d->logicOp);
            }
            r += (*iter).serialize();
        }
    }
    r += QLatin1Char(')');

    return r;
}


