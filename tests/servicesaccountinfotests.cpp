/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "servicesaccountinfotests.h"

#include <services/accountinfo.h>
#include <objects/accountinfo.h>

void ServicesAccountInfoTests::testJSONParser_data()
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
    QTest::addColumn< QByteArray >("jsonData");

    QTest::newRow("test1")
        << "1234568909876543210"
        << "john.doe@gmail.com"
        << "John Doe"
        << "John"
        << "Doe"
        << "1991-05-20"
        << "male"
        << "https://plus.google.com/12345678909876543210"
        << "en"
        << "Europe/Prague"
        << true
        << "https://lh3.googleusercontent.com/--ABCD/XXXX/XXXXy/1234-9876/photo.jpg"
        << QByteArray("{\n"
           "    \"id\": \"1234568909876543210\",\n"
           "    \"email\": \"john.doe@gmail.com\",\n"
           "    \"verified_email\": true,\n"
           "    \"name\": \"John Doe\",\n"
           "    \"given_name\": \"John\",\n"
           "    \"family_name\": \"Doe\",\n"
           "    \"birthday\": \"1991-05-20\",\n"
           "    \"link\": \"https://plus.google.com/12345678909876543210\",\n"
           "    \"picture\": \"https://lh3.googleusercontent.com/--ABCD/XXXX/XXXXy/1234-9876/photo.jpg\",\n"
           "    \"gender\": \"male\",\n"
           "    \"timezone\": \"Europe/Prague\",\n"
           "    \"locale\": \"en\"\n"
           "}");
}

void ServicesAccountInfoTests::testJSONParser()
{
    KGAPI::Objects::AccountInfo *accInfo;
    KGAPI::Services::AccountInfo *service;

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
    QFETCH(QByteArray, jsonData);

    service = new KGAPI::Services::AccountInfo();

    accInfo = dynamic_cast< KGAPI::Objects::AccountInfo* >(service->JSONToObject(jsonData));

    QCOMPARE(accInfo->id(), id);
    QCOMPARE(accInfo->email(), email);
    QCOMPARE(accInfo->name(), name);
    QCOMPARE(accInfo->givenName(), givenName);
    QCOMPARE(accInfo->familyName(), familyName);
    QCOMPARE(accInfo->birthday(), birthday);
    QCOMPARE(accInfo->gender(), gender);
    QCOMPARE(accInfo->link(), link);
    QCOMPARE(accInfo->locale(), locale);
    QCOMPARE(accInfo->timezone(), timezone);
    QCOMPARE(accInfo->verifiedEmail(), verifiedEmail);
    QCOMPARE(accInfo->photoUrl(), photoUrl);

    delete accInfo;
    delete service;
}

QTEST_KDEMAIN(ServicesAccountInfoTests, NoGUI);
