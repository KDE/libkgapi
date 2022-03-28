/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "accountinfo/accountinfo.h"
#include "accountinfo/accountinfofetchjob.h"
#include "types.h"

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
            {scenarioFromFile(QFINDTESTDATA("data/accountinfo_fetch_request.txt"), QFINDTESTDATA("data/accountinfo_fetch_response.txt"))});

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
