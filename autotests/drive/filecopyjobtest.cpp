/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "drivetestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "file.h"
#include "filecopyjob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::FilesList)

class FileCopyJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testCopy_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<Drive::FilesList>("sourceFiles");
        QTest::addColumn<Drive::FilesList>("destinationFiles");
        QTest::addColumn<Drive::FilesList>("expectedCopies");

        QTest::newRow("simple file") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/file1_copy_request.txt"),
                                                                                                   QFINDTESTDATA("data/file1_copy_response.txt"))}
                                     << Drive::FilesList{fileFromFile(QFINDTESTDATA("data/file1.json"))}
                                     << Drive::FilesList{fileFromFile(QFINDTESTDATA("data/file1_copy.json"))}
                                     << Drive::FilesList{fileFromFile(QFINDTESTDATA("data/file1_copy.json"))};

        // NOTE: The scenarios are reversed due use of QMap, which orders the files
        // by ID
        QTest::newRow("batch copy") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/file2_copy_request.txt"),
                                                                                                  QFINDTESTDATA("data/file2_copy_response.txt")),
                                                                                 scenarioFromFile(QFINDTESTDATA("data/file1_copy_request.txt"),
                                                                                                  QFINDTESTDATA("data/file1_copy_response.txt"))}
                                    << Drive::FilesList{fileFromFile(QFINDTESTDATA("data/file1.json")), fileFromFile(QFINDTESTDATA("data/file2.json"))}
                                    << Drive::FilesList{fileFromFile(QFINDTESTDATA("data/file1_copy.json")),
                                                        fileFromFile(QFINDTESTDATA("data/file2_copy.json"))}
                                    << Drive::FilesList{fileFromFile(QFINDTESTDATA("data/file2_copy.json")),
                                                        fileFromFile(QFINDTESTDATA("data/file1_copy.json"))};
    }

    void testCopy()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::FilesList, sourceFiles);
        QFETCH(Drive::FilesList, destinationFiles);
        QFETCH(Drive::FilesList, expectedCopies);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        Drive::FileCopyJob *job = nullptr;
        if (sourceFiles.count() == 1) {
            job = new Drive::FileCopyJob(sourceFiles.at(0), destinationFiles.at(0), account);
        } else {
            QMap<Drive::FilePtr, Drive::FilePtr> map;
            for (auto it1 = sourceFiles.cbegin(), it2 = destinationFiles.cbegin(), end = sourceFiles.cend(); it1 != end; ++it1, ++it2) {
                map.insert(*it1, *it2);
            }
            job = new Drive::FileCopyJob(map, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->files();
        QCOMPARE(items.count(), expectedCopies.count());
        for (int i = 0; i < expectedCopies.count(); ++i) {
            QVERIFY(items.at(i));
            QCOMPARE(*items.at(i), *expectedCopies.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(FileCopyJobTest)

#include "filecopyjobtest.moc"
