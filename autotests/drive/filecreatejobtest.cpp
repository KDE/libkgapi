/*
 * Createright (C) 2018  Daniel Vrátil <dvratil@kde.org>
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
 * You should have received a create of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "drivetestutils.h"

#include "types.h"
#include "filecreatejob.h"
#include "file.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::FilePtr)

class FileCreateJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testCreate_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<Drive::FilePtr>("sourceFile");
        QTest::addColumn<QString>("uploadFilePath");
        QTest::addColumn<Drive::FilePtr>("expectedResult");

        QTest::newRow("metadata only")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/file1_create_request.txt"),
                                     QFINDTESTDATA("data/file1_create_response.txt"))
                }
            << fileFromFile(QFINDTESTDATA("data/file1.json"))
            << QString()
            << fileFromFile(QFINDTESTDATA("data/file1.json"));

        // NOTE: The scenarios are reversed due use of QMap, which orders the files
        // by ID
        QTest::newRow("upload")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/file2_create_request.txt"),
                                     QFINDTESTDATA("data/file2_create_response.txt"))
                }
            << fileFromFile(QFINDTESTDATA("data/file2.json"))
            << QFINDTESTDATA("data/DSC_1287.JPG")
            << fileFromFile(QFINDTESTDATA("data/file2.json"));
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::FilePtr, sourceFile);
        QFETCH(QString, uploadFilePath);
        QFETCH(Drive::FilePtr, expectedResult);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        Drive::FileCreateJob *job = nullptr;
        if (uploadFilePath.isNull()) {
            job = new Drive::FileCreateJob(sourceFile, account);
        } else {
            job = new Drive::FileCreateJob(uploadFilePath, sourceFile, account);
        }

        QVERIFY(execJob(job));
        const auto items = job->files();
        QCOMPARE(items.count(), 1);
        QVERIFY(*items.cbegin());
        QCOMPARE(**items.cbegin(), *expectedResult);
    }
};

QTEST_GUILESS_MAIN(FileCreateJobTest)

#include "filecreatejobtest.moc"





