/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
