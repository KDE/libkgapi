/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "teamdrivesearchquery.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

Q_DECLARE_METATYPE(KGAPI2::Drive::TeamdriveSearchQuery)

class TeamdriveSearchQueryTest: public QObject
{
    Q_OBJECT
public:
    explicit TeamdriveSearchQueryTest() {}

    ~TeamdriveSearchQueryTest() {}

private Q_SLOTS:
    void testTeamdriveSearchQuery_data()
    {
        QTest::addColumn<TeamdriveSearchQuery>("query");
        QTest::addColumn<QString>("expected");

        {
            TeamdriveSearchQuery query;
            query.addQuery(TeamdriveSearchQuery::CreatedTime, TeamdriveSearchQuery::Equals, QDateTime(QDate(2019, 3, 5), QTime(6, 36, 0), Qt::UTC));
            QTest::newRow("serialize time") << query
                                    << "((createdTime = '2019-03-05T06:36:00'))";
        }

        {
            TeamdriveSearchQuery query;
            query.addQuery(TeamdriveSearchQuery::Name, TeamdriveSearchQuery::Contains, QLatin1String("Some String"));
            QTest::newRow("contains") << query
                                    << "((name contains 'Some String'))";
        }

        {
            TeamdriveSearchQuery query;
            query.addQuery(TeamdriveSearchQuery::Name, TeamdriveSearchQuery::Contains, QLatin1String("Name"));
            query.addQuery(TeamdriveSearchQuery::MemberCount, TeamdriveSearchQuery::Equals, 5);
            QTest::newRow("A and B") << query
                                     << "((name contains 'Name') and (memberCount = 5))";
        }

        {
            TeamdriveSearchQuery query(TeamdriveSearchQuery::Or);
            query.addQuery(TeamdriveSearchQuery::Name, TeamdriveSearchQuery::Contains, QLatin1String("Name"));
            query.addQuery(TeamdriveSearchQuery::OrganizerCount, TeamdriveSearchQuery::Equals, 5);
            QTest::newRow("A or B") << query
                                    << "((name contains 'Name') or (organizerCount = 5))";
        }

        {
            TeamdriveSearchQuery query(TeamdriveSearchQuery::Or);
            query.addQuery(TeamdriveSearchQuery::CreatedTime, TeamdriveSearchQuery::Equals, QDateTime(QDate(2019, 3, 5), QTime(6, 36, 0), Qt::UTC));
            TeamdriveSearchQuery subquery;
            subquery.addQuery(TeamdriveSearchQuery::Name, TeamdriveSearchQuery::Equals, QLatin1String("Test"));
            subquery.addQuery(TeamdriveSearchQuery::MemberCount, TeamdriveSearchQuery::LessOrEqual, 10);
            query.addQuery(subquery);
            QTest::newRow("A or (B and C)") << query
                                            << "((createdTime = '2019-03-05T06:36:00') or ((name = 'Test') and (memberCount <= 10)))";
        }
    }

    void testTeamdriveSearchQuery()
    {
        QFETCH(TeamdriveSearchQuery, query);
        QFETCH(QString, expected);

        const QString serialized = query.serialize();
        QCOMPARE(serialized, expected);
    }
};

QTEST_GUILESS_MAIN(TeamdriveSearchQueryTest)

#include "teamdrivesearchquerytest.moc"
