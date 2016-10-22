/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filesearchquery.h"

#include <QString>
#include <QDateTime>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileSearchQuery::Private : public QSharedData
{
public:
    Private();
    Private(const Private &other);
    ~Private();

    static QString fieldToString(Field field);
    static QString compareOperatorToString(CompareOperator op);
    static QString logicOperatorToString(LogicOperator op);
    static QString valueToString(Field field, const QVariant &var);

    QList<FileSearchQuery> subqueries;
    QVariant value;
    Field field;
    CompareOperator compareOp;
    LogicOperator logicOp;
};

FileSearchQuery::Private::Private()
    : QSharedData()
{
}

FileSearchQuery::Private::Private(const Private &other)
    : QSharedData(other)
    , subqueries(other.subqueries)
    , value(other.value)
    , field(other.field)
    , compareOp(other.compareOp)
    , logicOp(other.logicOp)
{
}

FileSearchQuery::Private::~Private()
{
}


QString FileSearchQuery::Private::fieldToString(Field field)
{
    switch (field) {
    case Title:
        return QStringLiteral("title");
    case FullText:
        return QStringLiteral("fullText");
    case MimeType:
        return QStringLiteral("mimeType");
    case ModifiedDate:
        return QStringLiteral("modifiedDate");
    case LastViewedByMeDate:
        return QStringLiteral("lastViewedByMeDate");
    case Trashed:
        return QStringLiteral("trashed");
    case Starred:
        return QStringLiteral("starred");
    case Parents:
        return QStringLiteral("parents");
    case Owners:
        return QStringLiteral("owners");
    case Writers:
        return QStringLiteral("writers");
    case Readers:
        return QStringLiteral("readers");
    case SharedWithMe:
        return QStringLiteral("sharedWithMe");
    }

    Q_ASSERT(false);
    return QString();
}

QString FileSearchQuery::Private::compareOperatorToString(CompareOperator op)
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

QString FileSearchQuery::Private::logicOperatorToString(FileSearchQuery::LogicOperator op)
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

QString FileSearchQuery::Private::valueToString(FileSearchQuery::Field field, const QVariant &var)
{
    switch (field) {
    case Title:
    case FullText:
    case MimeType:
    case Parents:
    case Owners:
    case Writers:
    case Readers:
        return QStringLiteral("'%1'").arg(var.toString().replace(QLatin1Char('\''), QLatin1String("\\\'")));
    case ModifiedDate:
    case LastViewedByMeDate:
        return QStringLiteral("'%1'").arg(var.toDateTime().toUTC().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss")));
    case Trashed:
    case Starred:
    case SharedWithMe:
        return (var.toBool() == true ? QStringLiteral("true") : QStringLiteral("false"));
    }

    Q_ASSERT(false);
    return QString();
}

FileSearchQuery::FileSearchQuery(FileSearchQuery::LogicOperator op)
    : d(new Private)
{
    d->logicOp = op;
}

FileSearchQuery::FileSearchQuery(const FileSearchQuery &other)
    : d(other.d)
{
}

FileSearchQuery::~FileSearchQuery()
{
}

FileSearchQuery &FileSearchQuery::operator=(const FileSearchQuery &other)
{
    d = other.d;
    return *this;
}


void FileSearchQuery::addQuery(FileSearchQuery::Field field, FileSearchQuery::CompareOperator op, const QVariant &value)
{
    switch (field) {
    case Title:
    case MimeType:
        Q_ASSERT(op == Contains || op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<QString>());
        break;
    case FullText:
        Q_ASSERT(op == Contains);
        Q_ASSERT(value.canConvert<QString>());
        break;
    case ModifiedDate:
    case LastViewedByMeDate:
        Q_ASSERT(op == LessOrEqual || op == Less || op == Equals || op == NotEquals || op == Greater || op == GreaterOrEqual);
        Q_ASSERT(value.canConvert<QDateTime>());
        break;
    case Trashed:
    case Starred:
    case SharedWithMe:
        Q_ASSERT(op == Equals || op == NotEquals);
        Q_ASSERT(value.canConvert<bool>());
        break;
    case Parents:
    case Owners:
    case Writers:
    case Readers:
        Q_ASSERT(op == In);
        Q_ASSERT(value.canConvert<QString>());
        break;
    }

    FileSearchQuery query;
    query.d->field = field;
    query.d->compareOp = op;
    query.d->value = value;
    d->subqueries.append(query);
}

void FileSearchQuery::addQuery(const FileSearchQuery &query)
{
    d->subqueries.append(query);
}

bool FileSearchQuery::isEmpty() const
{
    return d->value.isNull() && d->subqueries.isEmpty();
}

QString FileSearchQuery::serialize() const
{
    if (isEmpty()) {
        return QString();
    }

    QString r;
    r = QLatin1Char('(');
    if (d->subqueries.isEmpty()) {
        if (d->compareOp == In) {
            r += QStringLiteral("%1 in %2").arg(Private::valueToString(d->field, d->value),
                                                     Private::fieldToString(d->field));
        } else {
            r += Private::fieldToString(d->field) % Private::compareOperatorToString(d->compareOp) % Private::valueToString(d->field, d->value);
        }
    } else {
        QList<FileSearchQuery>::ConstIterator iter, end;
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


