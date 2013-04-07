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



#include "objectscontacttests.h"

#include <KDateTime>
#include <KUrl>

#include <libkgapi/objects/contact.h>

using namespace KGAPI::Objects;

void ObjectsContactTests::testIMTypeConversion()
{
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::AIM),
             QString("AIM"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::GoogleTalk),
             QString("GOOGLE_TALK"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::ICQ),
             QString("ICQ"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::Jabber),
             QString("JABBER"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::MSN),
             QString("MSN"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::Other),
             QString("Other"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::QQ),
             QString("QQ"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::Skype),
             QString("SKYPE"));
    QCOMPARE(Contact::IMProtocolToScheme(KGAPI::Objects::Contact::Yahoo),
             QString("YAHOO"));

    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#AIM"),
             QString("aim"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#GOOGLE_TALK"),
             QString("google_talk"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#ICQ"),
             QString("icq"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#JABBER"),
             QString("jabber"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#MSN"),
             QString("msn"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#OTHER"),
             QString("other"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#QQ"),
             QString("qq"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#SKYPE"),
             QString("skype"));
    QCOMPARE(Contact::IMSchemeToProtocolName("http://schemas.google.com/g/2005#YAHOO"),
             QString("yahoo"));

    QCOMPARE(Contact::IMProtocolNameToScheme("AIM"),
             QString("http://schemas.google.com/g/2005#AIM"));
    QCOMPARE(Contact::IMProtocolNameToScheme("GOOGLE_TALK"),
             QString("http://schemas.google.com/g/2005#GOOGLE_TALK"));
    QCOMPARE(Contact::IMProtocolNameToScheme("ICQ"),
             QString("http://schemas.google.com/g/2005#ICQ"));
    QCOMPARE(Contact::IMProtocolNameToScheme("XMPP"),
             QString("http://schemas.google.com/g/2005#JABBER"));
    QCOMPARE(Contact::IMProtocolNameToScheme("MSN"),
             QString("http://schemas.google.com/g/2005#MSN"));
    QCOMPARE(Contact::IMProtocolNameToScheme("QQ"),
             QString("http://schemas.google.com/g/2005#QQ"));
    QCOMPARE(Contact::IMProtocolNameToScheme("SKYPE"),
             QString("http://schemas.google.com/g/2005#SKYPE"));
    QCOMPARE(Contact::IMProtocolNameToScheme("YAHOO"),
             QString("http://schemas.google.com/g/2005#YAHOO"));
    QCOMPARE(Contact::IMProtocolNameToScheme("RandomProtocol"),
             QString("RandomProtocol"));

    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#AIM"),
             KGAPI::Objects::Contact::AIM);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#GOOGLE_TALK"),
             KGAPI::Objects::Contact::GoogleTalk);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#ICQ"),
             KGAPI::Objects::Contact::ICQ);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#XMPP"),
             KGAPI::Objects::Contact::Jabber);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#JABBER"),
             KGAPI::Objects::Contact::Jabber);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#MSN"),
             KGAPI::Objects::Contact::MSN);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#OTHER"),
             KGAPI::Objects::Contact::Other);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#QQ"),
             KGAPI::Objects::Contact::QQ);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#SKYPE"),
             KGAPI::Objects::Contact::Skype);
    QCOMPARE(Contact::IMSchemeToProtocol("http://schemas.google.com/g/2005#YAHOO"),
             KGAPI::Objects::Contact::Yahoo);
}

void ObjectsContactTests::testAddressTypeConversion()
{
    bool primary;

    primary = true;
    QCOMPARE(Contact::addressTypeToScheme(KABC::Address::Work, &primary),
             QString("http://schemas.google.com/g/2005#work"));
    QCOMPARE(primary, false);

    primary = true;
    QCOMPARE(Contact::addressTypeToScheme(KABC::Address::Home, &primary),
             QString("http://schemas.google.com/g/2005#home"));
    QCOMPARE(primary, false);

    primary = false;
    QCOMPARE(Contact::addressTypeToScheme(KABC::Address::Work |
                                          KABC::Address::Pref, &primary),
             QString("http://schemas.google.com/g/2005#work"));
    QCOMPARE(primary, true);

    QCOMPARE(Contact::addressSchemeToType("http://schemas.google.com/g/2005#work", false),
             KABC::Address::Work);
    QCOMPARE(Contact::addressSchemeToType("http://schemas.google.com/g/2005#home", false),
             KABC::Address::Home);
    QCOMPARE(Contact::addressSchemeToType("http://schemas.google.com/g/2005#home", true),
             KABC::Address::Home | KABC::Address::Pref);
}

void ObjectsContactTests::testPhoneTypeConversion()
{
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#car"),
             KABC::PhoneNumber::Car);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#fax"),
             KABC::PhoneNumber::Fax);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#isdn"),
             KABC::PhoneNumber::Isdn);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#mobile"),
             KABC::PhoneNumber::Cell);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#pager"),
             KABC::PhoneNumber::Pager);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#main"),
             KABC::PhoneNumber::Pref);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#home_fax"),
             KABC::PhoneNumber::Home | KABC::PhoneNumber::Fax);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#work"),
             KABC::PhoneNumber::Work);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#work_fax"),
             KABC::PhoneNumber::Work | KABC::PhoneNumber::Fax);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#work_mobile"),
             KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#work_pager"),
             KABC::PhoneNumber::Work | KABC::PhoneNumber::Pager);
    QCOMPARE(Contact::phoneSchemeToType("http://schemas.google.com/g/2005#company_main"),
             KABC::PhoneNumber::Work | KABC::PhoneNumber::Pref);

    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell),
             QString("http://schemas.google.com/g/2005#work_mobile"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Work | KABC::PhoneNumber::Fax),
             QString("http://schemas.google.com/g/2005#work_fax"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Work | KABC::PhoneNumber::Pager),
             QString("http://schemas.google.com/g/2005#work_pager"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Work | KABC::PhoneNumber::Pref),
             QString("http://schemas.google.com/g/2005#company_main"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Work),
             QString("http://schemas.google.com/g/2005#work"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Home | KABC::PhoneNumber::Fax),
             QString("http://schemas.google.com/g/2005#home_fax"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Home),
             QString("http://schemas.google.com/g/2005#home"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Pref),
             QString("http://schemas.google.com/g/2005#main"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Car),
             QString("http://schemas.google.com/g/2005#car"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Isdn),
             QString("http://schemas.google.com/g/2005#isdn"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Fax),
             QString("http://schemas.google.com/g/2005#fax"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Cell),
             QString("http://schemas.google.com/g/2005#mobile"));
    QCOMPARE(Contact::phoneTypeToScheme(KABC::PhoneNumber::Pager),
             QString("http://schemas.google.com/g/2005#pager"));
}




void ObjectsContactTests::testContact_data()
{
    QTest::addColumn< bool >("deleted");
    QTest::addColumn< QUrl >("photoUrl");
    QTest::addColumn< KDateTime >("created");
    QTest::addColumn< KDateTime >("updated");
    QTest::addColumn< QString >("spouseName");
    QTest::addColumn< QString >("managerName");
    QTest::addColumn< QString >("assistantName");
    QTest::addColumn< QString >("profession");
    QTest::addColumn< QString >("office");
    QTest::addColumn< QString >("anniversary");
    QTest::addColumn< QString >("blogFeed");
    QTest::addColumn< QStringList >("groups");

    QStringList groups1;
    groups1 << "http://www.google.com/feeds/contacts/groups/user%40gmail.com/base/1234a"
            << "http://www.google.com/feeds/contacts/groups/user%40gmail.com/base/1234b";

    QTest::newRow("contact1_czech")
            << true
            << QUrl("https://www.google.com/m8/feeds/photos/media/user@gmail.com/1234abcd/dcba4321")
            << KDateTime::fromString("2011-12-11T16:55:55.183Z", KDateTime::RFC3339Date)
            << KDateTime::fromString("2011-12-12T09:32:04.351Z", KDateTime::RFC3339Date)
            << "Jane Spouse"
            << "Franta Vomáčka"
            << "Honza Novák"
            << "programátor"
            << "3. patro, 2. dveře v pravo od výtahu"
            << "2012-12-12"
            << "http://pepa.z.depa.cz"
            << groups1;
}

void ObjectsContactTests::testContact()
{
    KGAPI::Objects::Contact contact;

    QFETCH(bool, deleted);
    QFETCH(QUrl, photoUrl);
    QFETCH(KDateTime, created);
    QFETCH(KDateTime, updated);
    QFETCH(QString, spouseName);
    QFETCH(QString, managerName);
    QFETCH(QString, assistantName);
    QFETCH(QString, profession);
    QFETCH(QString, office);
    QFETCH(QString, anniversary);
    QFETCH(QString, blogFeed);
    QFETCH(QStringList, groups);

    contact.setDeleted(deleted);
    contact.setPhotoUrl(photoUrl);
    contact.setCreated(created);
    contact.setUpdated(updated);
    contact.setSpousesName(spouseName);
    contact.setManagersName(managerName);
    contact.setAssistantsName(assistantName);
    contact.setProfession(profession);
    contact.setOffice(office);
    contact.setAnniversary(anniversary);
    contact.setBlogFeed(blogFeed);
    contact.setGroups(groups);

    QCOMPARE(contact.deleted(), deleted);
    QCOMPARE(contact.photoUrl(), photoUrl);
    QCOMPARE(contact.created(), created);
    QCOMPARE(contact.updated(), updated);
    QCOMPARE(contact.spousesName(), spouseName);
    QCOMPARE(contact.managersName(), managerName);
    QCOMPARE(contact.assistantsName(), assistantName);
    QCOMPARE(contact.profession(), profession);
    QCOMPARE(contact.office(), office);
    QCOMPARE(contact.anniversary(), anniversary);
    QCOMPARE(contact.groups(), groups);
}


QTEST_KDEMAIN(ObjectsContactTests, NoGUI);
