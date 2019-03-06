/*
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
