/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QList>

#include "fakeaccountstorage.h"
#include "fakeauthbrowser.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "fakenetworkaccessmanager.h"
#include "fakenetworkreply.h"
#include "accountmanager.h"
#include "accountstorage_p.h"
#include "account.h"
#include "testutils.h"

using namespace KGAPI2;



namespace {

const static auto ApiKey1 = QStringLiteral("Key1");
const static auto SecretKey1 = QStringLiteral("Secret1");
const static auto Account1 = QStringLiteral("jonh.doe@fakegmail.invalid");

}

class TestableAccountManager : public AccountManager
{
    Q_OBJECT
public:
    explicit TestableAccountManager(QObject *parent = nullptr)
        : AccountManager(parent)
    {
        sInstance = this;
    }

    ~TestableAccountManager() override
    {
        sInstance = nullptr;
    }

    FakeAccountStorage *fakeStore() const
    {
        return mStorageFactory.fakeStore();
    }

private:
    FakeAccountStorageFactory mStorageFactory;
};

class AccountManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testGetNewAccount()
    {
        FakeAuthBrowser authBrowser;
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            { scenarioFromFile(QFINDTESTDATA("data/accountmanager_part1_request.txt"),
                               QFINDTESTDATA("data/accountmanager_part1_response.txt"),
                               false),
              scenarioFromFile(QFINDTESTDATA("data/accountinfo_fetch_request.txt"),
                               QFINDTESTDATA("data/accountinfo_fetch_response.txt")) });

        TestableAccountManager accountManager;
        const auto promise = accountManager.getAccount(ApiKey1, SecretKey1, Account1, { Account::contactsScopeUrl() });

        QCOMPARE(promise->account(), {});
        QSignalSpy spy(promise, &AccountPromise::finished);
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        const auto account = promise->account();
        QVERIFY(account);
        QCOMPARE(account->accountName(), Account1);
        const QList<QUrl> expectedScopes = { Account::contactsScopeUrl(), Account::accountInfoEmailScopeUrl() };
        QCOMPARE(account->scopes(), expectedScopes);
        QVERIFY(!account->accessToken().isEmpty());
        QVERIFY(!account->refreshToken().isEmpty());
        QVERIFY(account->expireDateTime().isValid());

        const auto storeAccount = accountManager.fakeStore()->mStore.value(ApiKey1 + Account1);
        QVERIFY(storeAccount);
        QCOMPARE(*storeAccount, *account);
    }

    void testGetExistingAccount()
    {
        TestableAccountManager accountManager;

        const auto insertedAccount = AccountPtr::create(*accountManager.fakeStore()->generateAccount(ApiKey1, Account1, { Account::contactsScopeUrl() }));
        QVERIFY(insertedAccount);

        const auto promise = accountManager.getAccount(ApiKey1, SecretKey1, Account1, { Account::contactsScopeUrl() });
        QCOMPARE(promise->account(), {});
        QSignalSpy spy(promise, &AccountPromise::finished);
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QVERIFY(promise->account());

        QCOMPARE(*promise->account(), *insertedAccount);
    }

    void testMergeAccountScopes()
    {
        FakeAuthBrowser authBrowser;
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            { scenarioFromFile(QFINDTESTDATA("data/accountmanager_part1_request.txt"),
                               QFINDTESTDATA("data/accountmanager_part1_response.txt"),
                               false),
              scenarioFromFile(QFINDTESTDATA("data/accountinfo_fetch_request.txt"),
                               QFINDTESTDATA("data/accountinfo_fetch_response.txt")) });

        TestableAccountManager accountManager;
        const auto insertedAccount = accountManager.fakeStore()->generateAccount(ApiKey1, Account1, { Account::contactsScopeUrl() });
        QVERIFY(insertedAccount);
        auto expectedAccount = AccountPtr::create(*insertedAccount);
        expectedAccount->setScopes({ Account::contactsScopeUrl(),
                                     Account::calendarScopeUrl(),
                                     Account::accountInfoEmailScopeUrl() });

        const auto promise = accountManager.getAccount(ApiKey1, SecretKey1, Account1, { Account::calendarScopeUrl() });
        QCOMPARE(promise->account(), {});
        QSignalSpy spy(promise, &AccountPromise::finished);
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        const auto account = promise->account();
        QVERIFY(account);
        QCOMPARE(account->accountName(), Account1);
        QVERIFY(!account->accessToken().isEmpty());
        QVERIFY(!account->refreshToken().isEmpty());
        QVERIFY(account->expireDateTime().isValid());
        QCOMPARE(account->scopes(), expectedAccount->scopes());

        const auto storeAccount = accountManager.fakeStore()->mStore.value(ApiKey1 + Account1);
        QVERIFY(storeAccount);
        QCOMPARE(storeAccount->accountName(), Account1);
        QVERIFY(!storeAccount->accessToken().isEmpty());
        QVERIFY(!storeAccount->refreshToken().isEmpty());
        QVERIFY(storeAccount->expireDateTime().isValid());
        QCOMPARE(storeAccount->scopes(), expectedAccount->scopes());
    }

    void testRemoveAccountScopes()
    {
        TestableAccountManager accountManager;
        accountManager.fakeStore()->generateAccount(ApiKey1, Account1,
                { Account::contactsScopeUrl(), Account::calendarScopeUrl() });

        accountManager.removeScopes(ApiKey1, Account1, { Account::contactsScopeUrl() });

        const auto storeAccount = accountManager.fakeStore()->mStore.value(ApiKey1 + Account1);
        QVERIFY(storeAccount);
        QCOMPARE(storeAccount->accountName(), Account1);
        QVERIFY(storeAccount->accessToken().isEmpty());
        QVERIFY(storeAccount->refreshToken().isEmpty());
        QVERIFY(storeAccount->expireDateTime().isNull());
        QCOMPARE(storeAccount->scopes(), QList<QUrl>{ Account::calendarScopeUrl() });
    }

    void testRemoveAllScopes()
    {
        TestableAccountManager accountManager;
        accountManager.fakeStore()->generateAccount(ApiKey1, Account1,
                { Account::contactsScopeUrl(), Account::calendarScopeUrl() });

        accountManager.removeScopes(ApiKey1, Account1, { Account::contactsScopeUrl(), Account::calendarScopeUrl() });

        QVERIFY(!accountManager.fakeStore()->mStore.contains(ApiKey1 + Account1));
    }

    void testFindInvalidAccount()
    {
        TestableAccountManager accountManager;

        const auto promise = accountManager.findAccount(ApiKey1, Account1);
        QCOMPARE(promise->account(), {});
        QSignalSpy spy(promise, &AccountPromise::finished);
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(promise->account(), AccountPtr());
    }

    void testFindValidAccount()
    {
        TestableAccountManager accountManager;

        const auto insertedAccount = accountManager.fakeStore()->generateAccount(ApiKey1, Account1, { Account::calendarScopeUrl() });

        const auto promise = accountManager.findAccount(ApiKey1, Account1);
        QCOMPARE(promise->account(), {});
        QSignalSpy spy(promise, &AccountPromise::finished);
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QVERIFY(promise->account());
        QCOMPARE(*promise->account(), *insertedAccount);
    }

    void testRefreshTokens()
    {
        FakeAuthBrowser authBrowser;
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            { scenarioFromFile(QFINDTESTDATA("data/accountmanager_refresh_request.txt"),
                               QFINDTESTDATA("data/accountmanager_refresh_response.txt"),
                               false)
            });

        TestableAccountManager accountManager;

        auto insertedAccount = accountManager.fakeStore()->generateAccount(ApiKey1, Account1, { Account::calendarScopeUrl() });
        insertedAccount->setRefreshToken(QStringLiteral("FakeRefreshToken"));

        const auto promise = accountManager.refreshTokens(ApiKey1, SecretKey1, Account1);
        QCOMPARE(promise->account(), {});
        QSignalSpy spy(promise, &AccountPromise::finished);
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QVERIFY(promise->account());
        QCOMPARE(promise->account()->accountName(), insertedAccount->accountName());
        QCOMPARE(promise->account()->accessToken(), QStringLiteral("NewAccessToken"));
    }
};

QTEST_MAIN(AccountManagerTest)

#include "accountmanagertest.moc"
