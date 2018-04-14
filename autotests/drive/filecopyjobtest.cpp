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
#include "drivetestutils.h"

#include "types.h"
#include "filecopyjob.h"
#include "file.h"
#include "account.h"

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

        QTest::newRow("simple file")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/file1_copy_request.txt"),
                                     QFINDTESTDATA("data/file1_copy_response.txt"))
                }
            << Drive::FilesList{ fileFromFile(QFINDTESTDATA("data/file1.json")) }
            << Drive::FilesList{ fileFromFile(QFINDTESTDATA("data/file1_copy.json")) }
            << Drive::FilesList{ fileFromFile(QFINDTESTDATA("data/file1_copy.json")) };

        // NOTE: The scenarios are reversed due use of QMap, which orders the files
        // by ID
        QTest::newRow("batch copy")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/file2_copy_request.txt"),
                                     QFINDTESTDATA("data/file2_copy_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/file1_copy_request.txt"),
                                     QFINDTESTDATA("data/file1_copy_response.txt"))
                }
            << Drive::FilesList{ fileFromFile(QFINDTESTDATA("data/file1.json")),
                                 fileFromFile(QFINDTESTDATA("data/file2.json")) }
            << Drive::FilesList{ fileFromFile(QFINDTESTDATA("data/file1_copy.json")),
                                 fileFromFile(QFINDTESTDATA("data/file2_copy.json")) }
            << Drive::FilesList{ fileFromFile(QFINDTESTDATA("data/file2_copy.json")),
                                 fileFromFile(QFINDTESTDATA("data/file1_copy.json")) };
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




