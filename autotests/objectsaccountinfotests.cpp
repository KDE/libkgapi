/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "objectsaccountinfotests.h"

#include <objects/accountinfo.h>

using namespace KGAPI;

void ObjectsAccountInfoTests::testAccountInfo_data()
{
    QTest::addColumn< QString >("id");
    QTest::addColumn< QString >("email");
    QTest::addColumn< QString >("name");
    QTest::addColumn< QString >("givenName");
    QTest::addColumn< QString >("familyName");
    QTest::addColumn< QString >("birthday");
    QTest::addColumn< QString >("gender");
    QTest::addColumn< QString >("link");
    QTest::addColumn< QString >("locale");
    QTest::addColumn< QString >("timezone");
    QTest::addColumn< bool >("verifiedEmail");
    QTest::addColumn< QString >("photoUrl");

    QTest::newRow("accountInfo1_czech")
        << "1234"
        << "user@gmail.com"
        << "Franta Vomáčka"
        << "Franta"
        << "Vomáčka"
        << "1991-05-20"
        << "male"
        << "https://plus.google.com/1234"
        << "en"
        << "Europe/Prague"
        << true
        << "https://lh3.googleusercontent.com/--XXXXX/XXXXX/abc-edf/photo.jpg";
}

void ObjectsAccountInfoTests::testAccountInfo()
{
    KGAPI::Objects::AccountInfo accountInfo;

    QFETCH(QString, id);
    QFETCH(QString, email);
    QFETCH(QString, name);
    QFETCH(QString, givenName);
    QFETCH(QString, familyName);
    QFETCH(QString, birthday);
    QFETCH(QString, gender);
    QFETCH(QString, link);
    QFETCH(QString, locale);
    QFETCH(QString, timezone);
    QFETCH(bool, verifiedEmail);
    QFETCH(QString, photoUrl);

    accountInfo.setId(id);
    accountInfo.setEmail(email);
    accountInfo.setName(name);
    accountInfo.setGivenName(givenName);
    accountInfo.setFamilyName(familyName);
    accountInfo.setBirthday(birthday);
    accountInfo.setGender(gender);
    accountInfo.setLink(link);
    accountInfo.setLocale(locale);
    accountInfo.setTimezone(timezone);
    accountInfo.setVerifiedEmail(verifiedEmail);
    accountInfo.setPhotoUrl(photoUrl);

    QCOMPARE(accountInfo.id(), id);
    QCOMPARE(accountInfo.email(), email);
    QCOMPARE(accountInfo.name(), name);
    QCOMPARE(accountInfo.givenName(), givenName);
    QCOMPARE(accountInfo.familyName(), familyName);
    QCOMPARE(accountInfo.birthday(), birthday);
    QCOMPARE(accountInfo.gender(), gender);
    QCOMPARE(accountInfo.link(), link);
    QCOMPARE(accountInfo.locale(), locale);
    QCOMPARE(accountInfo.timezone(), timezone);
    QCOMPARE(accountInfo.verifiedEmail(), verifiedEmail);
    QCOMPARE(accountInfo.photoUrl(), photoUrl);
}

QTEST_KDEMAIN(ObjectsAccountInfoTests, NoGUI);
