/*
 * Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
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

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "types.h"
#include "accountinfo/accountinfofetchjob.h"
#include "accountinfo/accountinfo.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)

class AccountInfoFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetch()
    {
        QFile f(QFINDTESTDATA("data/accountinfo.json"));
        QVERIFY(f.open(QIODevice::ReadOnly));
        auto accountInfo = AccountInfo::fromJSON(f.readAll());
        QVERIFY(accountInfo);

        FakeNetworkAccessManagerFactory::get()->setScenarios(
            { scenarioFromFile(QFINDTESTDATA("data/accountinfo_fetch_request.txt"),
                               QFINDTESTDATA("data/accountinfo_fetch_response.txt")) });

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new AccountInfoFetchJob(account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        QCOMPARE(*items.at(0).dynamicCast<AccountInfo>(), *accountInfo);
    }
};

QTEST_GUILESS_MAIN(AccountInfoFetchJobTest)

#include "accountinfofetchjobtest.moc"



