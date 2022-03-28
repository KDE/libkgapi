/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "drivessearchquery.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

Q_DECLARE_METATYPE(KGAPI2::Drive::DrivesSearchQuery)

class DrivesSearchQueryTest : public QObject
{
    Q_OBJECT
public:
    explicit DrivesSearchQueryTest()
    {
    }

    ~DrivesSearchQueryTest() override
    {
    }

private Q_SLOTS:
    void testDrivesSearchQuery_data()
    {
        QTest::addColumn<DrivesSearchQuery>("query");
        QTest::addColumn<QString>("expected");

        {
            DrivesSearchQuery query;
            query.addQuery(DrivesSearchQuery::CreatedDate, DrivesSearchQuery::Equals, QDateTime(QDate(2019, 6, 16), QTime(6, 36, 0), Qt::UTC));
            QTest::newRow("serialize time") << query << "((createdDate = '2019-06-16T06:36:00'))";
        }

        {
            DrivesSearchQuery query;
            query.addQuery(DrivesSearchQuery::Name, DrivesSearchQuery::Contains, QLatin1String("Some String"));
            QTest::newRow("contains") << query << "((name contains 'Some String'))";
        }

        {
            DrivesSearchQuery query;
            query.addQuery(DrivesSearchQuery::Hidden, DrivesSearchQuery::Equals, true);
            QTest::newRow("contains") << query << "((hidden = true))";
        }

        {
            DrivesSearchQuery query;
            query.addQuery(DrivesSearchQuery::Name, DrivesSearchQuery::Contains, QLatin1String("Name"));
            query.addQuery(DrivesSearchQuery::MemberCount, DrivesSearchQuery::Equals, 5);
            QTest::newRow("A and B") << query << "((name contains 'Name') and (memberCount = 5))";
        }

        {
            DrivesSearchQuery query(DrivesSearchQuery::Or);
            query.addQuery(DrivesSearchQuery::Name, DrivesSearchQuery::Contains, QLatin1String("Name"));
            query.addQuery(DrivesSearchQuery::OrganizerCount, DrivesSearchQuery::Equals, 5);
            QTest::newRow("A or B") << query << "((name contains 'Name') or (organizerCount = 5))";
        }

        {
            DrivesSearchQuery query(DrivesSearchQuery::Or);
            query.addQuery(DrivesSearchQuery::CreatedDate, DrivesSearchQuery::Equals, QDateTime(QDate(2019, 3, 5), QTime(6, 36, 0), Qt::UTC));
            DrivesSearchQuery subquery;
            subquery.addQuery(DrivesSearchQuery::Name, DrivesSearchQuery::Equals, QLatin1String("Test"));
            subquery.addQuery(DrivesSearchQuery::MemberCount, DrivesSearchQuery::LessOrEqual, 10);
            query.addQuery(subquery);
            QTest::newRow("A or (B and C)") << query << "((createdDate = '2019-03-05T06:36:00') or ((name = 'Test') and (memberCount <= 10)))";
        }
    }

    void testDrivesSearchQuery()
    {
        QFETCH(DrivesSearchQuery, query);
        QFETCH(QString, expected);

        const QString serialized = query.serialize();
        QCOMPARE(serialized, expected);
    }
};

QTEST_GUILESS_MAIN(DrivesSearchQueryTest)

#include "drivessearchquerytest.moc"
