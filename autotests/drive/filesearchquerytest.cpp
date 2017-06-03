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

#include <QObject>
#include <QTest>

#include "filesearchquery.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

Q_DECLARE_METATYPE(KGAPI2::Drive::FileSearchQuery)

class FileSearchQueryTest: public QObject
{
    Q_OBJECT
public:
    explicit FileSearchQueryTest()
    {
    }

    ~FileSearchQueryTest()
    {
    }

private Q_SLOTS:
    void testFileSearchQuery_data()
    {
        QTest::addColumn<FileSearchQuery>("query");
        QTest::addColumn<QString>("expected");

        {
            FileSearchQuery query;
            query.addQuery(FileSearchQuery::Title, FileSearchQuery::Contains, QLatin1String("Some String"));
            QTest::newRow("simple") << query
                                    << "((title contains 'Some String'))";
        }

        {
            FileSearchQuery query;
            query.addQuery(FileSearchQuery::Title, FileSearchQuery::Equals, QLatin1String("Title"));
            query.addQuery(FileSearchQuery::MimeType, FileSearchQuery::Contains, QLatin1String("text/plain"));
            QTest::newRow("A and B") << query
                                     << "((title = 'Title') and (mimeType contains 'text/plain'))";
        }

        {
            FileSearchQuery query(FileSearchQuery::Or);
            query.addQuery(FileSearchQuery::Trashed, FileSearchQuery::Equals, true);
            query.addQuery(FileSearchQuery::Writers, FileSearchQuery::In, QLatin1String("user@gmail.com"));
            QTest::newRow("A or B") << query
                                    << "((trashed = true) or ('user@gmail.com' in writers))";
        }

        {
            FileSearchQuery query(FileSearchQuery::Or);
            query.addQuery(FileSearchQuery::ModifiedDate, FileSearchQuery::Equals, QDateTime(QDate(2014, 07, 28), QTime(17, 05, 30), Qt::UTC));
            FileSearchQuery subquery;
            subquery.addQuery(FileSearchQuery::Title, FileSearchQuery::Equals, QLatin1String("Test"));
            subquery.addQuery(FileSearchQuery::Title, FileSearchQuery::NotEquals, QLatin1String("File"));
            query.addQuery(subquery);
            QTest::newRow("A or (B and C)") << query
                                            << "((modifiedDate = '2014-07-28T17:05:30') or ((title = 'Test') and (title != 'File')))";
        }
    }

    void testFileSearchQuery()
    {
        QFETCH(FileSearchQuery, query);
        QFETCH(QString, expected);

        const QString serialized = query.serialize();
        QCOMPARE(serialized, expected);
    }
};

QTEST_GUILESS_MAIN(FileSearchQueryTest)

#include "filesearchquerytest.moc"
