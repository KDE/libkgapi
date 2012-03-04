/*
    Akonadi Google - Contacts Test
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "kgoogleobject.h"
#include "objects/contact.h"
#include "services/addressbook.h"

#include <kdatetime.h>
#include <qdebug.h>
#include <qtest.h>
#include <qmetatype.h>

#include <kabc/phonenumber.h>

using namespace KGoogle;
using namespace KGoogle::Object;
using namespace KGoogle::Service;
using namespace KABC;

Q_DECLARE_METATYPE(KABC::PhoneNumber::List)
Q_DECLARE_METATYPE(KABC::Address::List)

class TestContacts : public QObject
{
    Q_OBJECT

private slots:
    void fromJSON_data();
    void fromJSON();

    void toXML_data();
    void toXML();

    /* TODO: toJSON() test */
    /* TODO: fromXML() test */
};


void TestContacts::fromJSON_data()
{
    PhoneNumber::List phoneNumbers;
    Address::List addresses;

    QTest::addColumn<QString>("jsonData");
    QTest::addColumn<QString>("id");
    QTest::addColumn<QString>("etag");
    QTest::addColumn<bool>("deleted");
    QTest::addColumn<QUrl>("photoUrl");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("notes");
    QTest::addColumn<QString>("job");
    QTest::addColumn<QString>("jobTitle");
    QTest::addColumn<QDateTime>("birthday");
    //QTest::addColumn<KDateTime>("created");
    QTest::addColumn<KDateTime>("updated");
    QTest::addColumn<QStringList>("emails");
    //QTest::addColumn<IM::List>("ims");
    QTest::addColumn<PhoneNumber::List>("phoneNumbers");
    QTest::addColumn<Address::List>("addresses");

    phoneNumbers << PhoneNumber("+001234567890", PhoneNumber::Cell)
                 << PhoneNumber("+098765432100", PhoneNumber::Work)
                 << PhoneNumber("+098123765567", PhoneNumber::Home)
                 << PhoneNumber("+098765432100", PhoneNumber::Pager);

    Address address(KABC::Address::Home);
    address.setStreet("Some Street");
    address.setPostOfficeBox("His POBOX");
    address.setLocality("A Random City");
    address.setRegion("Some state");
    address.setPostalCode("41000");
    address.setCountry("Czech Republic");
    addresses << address;

    address.clear();
    address.setType(KABC::Address::Work);
    address.setLabel("Another Street 29\r\nAnother POBOX\r\nAnother City, Some another State 55500\r\nGermany");
    addresses << address;

    QTest::newRow("Contact 1")
            << "{\"xmlns\":\"http://www.w3.org/2005/Atom\",\"xmlns$gContact\":\"http://schemas.google.com/contact/2008\",\"xmlns$batch\":\"http://schemas.google.com/gdata/batch\""
            ",\"xmlns$gd\":\"http://schemas.google.com/g/2005\",\"gd$etag\":\"\\\"Q3w_fzVSLit7I2A9WhdSFU4DQwM.\\\"\",\"id\":{\"$t\":\"http://www.google.com/m8/feeds/contacts/some.user%40gmail.com/base/21e062b78964abc6\"},"
            "\"updated\":{\"$t\":\"2011-07-24T20:31:12.247Z\"},\"app$edited\":{\"xmlns$app\":\"http://www.w3.org/2007/app\",\"$t\":\"2011-07-24T20:31:12.247Z\"},"
            "\"category\":[{\"scheme\":\"http://schemas.google.com/g/2005#kind\",\"term\":\"http://schemas.google.com/contact/2008#contact\"}],"
            "\"title\":{\"$t\":\"Mr. FirstName MiddleName Surename CSc.\"},\"content\":{\"$t\":\"Some notes about this nice man.\"},\"link\":[{\"rel\":"
            "\"http://schemas.google.com/contacts/2008/rel#photo\",\"type\":\"image/*\",\"href\":\"https://www.google.com/m8/feeds/photos/media/some.user%40gmail.com/21e062b78964abc6\"},"
            "{\"rel\":\"self\",\"type\":\"application/atom+xml\",\"href\":\"https://www.google.com/m8/feeds/contacts/some.user%40gmail.com/full/21e062b78964abc6\"},{\"rel\":\"edit\","
            "\"type\":\"application/atom+xml\",\"href\":\"https://www.google.com/m8/feeds/contacts/some.user%40gmail.com/full/21e062b78964abc6\"}],\"gd$name\":{\"gd$fullName\":"
            "{\"$t\":\"Mr. FirstName MiddleName Surename CSc.\"},\"gd$namePrefix\":{\"$t\":\"Mr.\"},\"gd$givenName\":{\"$t\":\"FirstName\"},\"gd$additionalName\":{\"$t\":\"MiddleName\"},"
            "\"gd$familyName\":{\"$t\":\"Surename\"},\"gd$nameSuffix\":{\"$t\":\"CSc.\"}},\"gContact$nickname\":{\"$t\":\"Johhnie\"},\"gContact$birthday\":{\"when\":\"1991-05-20\"},"
            "\"gd$organization\":[{\"rel\":\"http://schemas.google.com/g/2005#other\",\"gd$orgName\":{\"$t\":\"Good Software Inc.\"},\"gd$orgTitle\":{\"$t\":\"Software Engineer\"}}],"
            "\"gd$email\":[{\"rel\":\"http://schemas.google.com/g/2005#home\",\"address\":\"email1@server.com\",\"primary\":\"true\"},{\"rel\":\"http://schemas.google.com/g/2005#work\","
            "\"address\":\"email2@server.com\"}],\"gd$im\":[{\"address\":\"jabber@server.com\",\"protocol\":\"http://schemas.google.com/g/2005#JABBER\",\"rel\":"
            "\"http://schemas.google.com/g/2005#other\"},{\"address\":\"123456789\",\"protocol\":\"http://schemas.google.com/g/2005#ICQ\",\"rel\":\"http://schemas.google.com/g/2005#other\"},"
            "{\"address\":\"talkie@google.com\",\"protocol\":\"http://schemas.google.com/g/2005#GOOGLE_TALK\",\"rel\":\"http://schemas.google.com/g/2005#other\"},{\"address\":\"aim@address.com\","
            "\"protocol\":\"http://schemas.google.com/g/2005#AIM\",\"rel\":\"http://schemas.google.com/g/2005#other\"},{\"address\":\"callme\",\"protocol\":"
            "\"http://schemas.google.com/g/2005#SKYPE\",\"rel\":\"http://schemas.google.com/g/2005#other\"}],\"gd$phoneNumber\":[{\"rel\":\"http://schemas.google.com/g/2005#mobile\","
            "\"$t\":\"+001234567890\"},{\"rel\":\"http://schemas.google.com/g/2005#work\",\"$t\":\"+098765432100\"},{\"rel\":\"http://schemas.google.com/g/2005#home\",\"$t\":\"+098123765567\"},"
            "{\"rel\":\"http://schemas.google.com/g/2005#pager\",\"$t\":\"+098765432100\"}],\"gd$structuredPostalAddress\":[{\"rel\":\"http://schemas.google.com/g/2005#home\","
            "\"gd$formattedAddress\":{\"$t\":\"Some Street\nHis POBOX\nA Random City, Some state 41000\nCzech Republic\"},\"gd$street\":{\"$t\":\"Some Street\"},\"gd$pobox\":{\"$t\":\"His POBOX\"},"
            "\"gd$postcode\":{\"$t\":\"41000\"},\"gd$city\":{\"$t\":\"A Random City\"},\"gd$region\":{\"$t\":\"Some state\"},\"gd$country\":{\"$t\":\"Czech Republic\"}},{\"rel\":"
            "\"http://schemas.google.com/g/2005#work\",\"gd$formattedAddress\":{\"$t\":\"Another Street 29\r\nAnother POBOX\r\nAnother City, Some another State 55500\r\nGermany\"},"
            "\"gd$street\":{\"$t\":\"Another Street 29\r\nAnother POBOX\r\nAnother City, Some another State 55500\r\nGermany\"}}],\"gContact$groupMembershipInfo\":[{\"deleted\":\"false\","
            "\"href\":\"http://www.google.com/m8/feeds/groups/some.user%40gmail.com/base/6\"}]}"
            << "21e062b78964abc6"
            << "\"Q3w_fzVSLit7I2A9WhdSFU4DQwM.\""
            << false
            << QUrl("https://www.google.com/m8/feeds/photos/media/some.user%40gmail.com/21e062b78964abc6")
            << "Mr. FirstName MiddleName Surename CSc."
            << "Some notes about this nice man."
            << "Good Software Inc."
            << "Software Engineer"
            << QDateTime::fromString("1991-05-20", "%Y-%m-%d")
            //<< KDateTime()
            << KDateTime::fromString("2011-07-24T20:31:12.247Z", KDateTime::RFC3339Date)
            << (QStringList() << "email1@server.com" << "email2@server.com")
            << phoneNumbers
            << addresses;
}


void TestContacts::fromJSON()
{
    QFETCH(QString, jsonData);
    QFETCH(QString, id);
    QFETCH(QString, etag);
    QFETCH(bool, deleted);
    QFETCH(QUrl, photoUrl);
    QFETCH(QString, name);
    QFETCH(QString, notes);
    QFETCH(QString, job);
    QFETCH(QString, jobTitle);
    QFETCH(QDateTime, birthday);
    //QFETCH(KDateTime, created);
    QFETCH(KDateTime, updated);
    QFETCH(QStringList, emails);
    QFETCH(PhoneNumber::List, phoneNumbers);
    QFETCH(Address::List, addresses);

    Addressbook *book = new Addressbook();
    Contact *contact = static_cast<Contact*>(book->JSONToObject(jsonData.toLatin1()));

    QCOMPARE(contact->id(), id);
    QCOMPARE(contact->etag(), etag);
    QCOMPARE(contact->deleted(), deleted);
    QCOMPARE(contact->photoUrl(), photoUrl);
    QCOMPARE(contact->name(), name);
    QCOMPARE(contact->note(), notes);
    QCOMPARE(contact->organization(), job);
    QCOMPARE(contact->role(), jobTitle);
    QCOMPARE(contact->birthday(), birthday);
    //QCOMPARE(contact->created().toString(KDateTime::RFC3339Date), created.toString(KDateTime::RFC3339Date));
    QCOMPARE(contact->updated().toString(KDateTime::RFC3339Date), updated.toString(KDateTime::RFC3339Date));
    QCOMPARE(contact->emails(), emails);
    QCOMPARE(contact->phoneNumbers(), phoneNumbers);
    QCOMPARE(contact->addresses(), addresses);

    delete book;
    delete contact;
}

void TestContacts::toXML_data()
{
    QTest::addColumn<Contact::Ptr>("contact");
    QTest::addColumn<QString>("xml");

    Contact::Ptr contact = (Contact::Ptr)new Contact();
    contact->setId("21e062b78964abc6");
    contact->setEtag("\"Q3w_fzVSLit7I2A9WhdSFU4DQwM.\"");
    contact->setDeleted(false);
    contact->setPhotoUrl(QUrl("https://www.google.com/m8/feeds/photos/media/some.user%40gmail.com/21e062b78964abc6"));
    contact->setName("Mr. FirstName MiddleName Surename CSc.");
    contact->setNote("Some notes about this nice man.");
    contact->setOrganization("Good Software Inc.");
    contact->setRole("Software Engineer");
    contact->setBirthday(QDateTime::fromString("1991-05-20", "%Y-%m-%d"));
    contact->insertEmail("email1@server.com");
    contact->insertEmail("email2@server.com");
    contact->insertPhoneNumber(PhoneNumber("+001234567890", PhoneNumber::Cell));
    contact->insertPhoneNumber(PhoneNumber("+098765432100", PhoneNumber::Work));
    contact->insertPhoneNumber(PhoneNumber("+098123765567", PhoneNumber::Home));
    contact->insertPhoneNumber(PhoneNumber("+098765432100", PhoneNumber::Pager));

    Address address;
    address.setType(KABC::Address::Home);
    address.setStreet("Some Street");
    address.setPostOfficeBox("His POBOX");
    address.setLocality("A Random City");
    address.setRegion("Some state");
    address.setPostalCode("41000");
    address.setCountry("Czech Republic");
    contact->insertAddress(address);

    address.clear();
    address.setType(KABC::Address::Work);
    address.setLabel("Another Street 29\r\nAnother POBOX\r\nAnother City, Some another State 55500\r\nGermany");
    contact->insertAddress(address);

    QString xmlData;

    QTest::newRow("Contact 1")
            << contact
            << "<gd:name><gd:givenName>FirstName</gd:givenName><gd:familyName>CSc.</gd:familyName><gd:fullName>Mr. FirstName MiddleName Surename CSc.</gd:fullName></gd:name><atom:content type='text'>"
            "Some notes about this nice man.</atom:content><gd:email rel='http://schemas.google.com/g/2005#home' address='email1@server.com' />"
            "<gd:email rel='http://schemas.google.com/g/2005#home' address='email2@server.com' /><gd:phoneNumber rel='http://schemas.google.com/g/2005#mobile'>+001234567890</gd:phoneNumber>"
            "<gd:phoneNumber rel='http://schemas.google.com/g/2005#work'>+098765432100</gd:phoneNumber><gd:phoneNumber rel='http://schemas.google.com/g/2005#home'>+098123765567</gd:phoneNumber>"
            "<gd:phoneNumber rel='http://schemas.google.com/g/2005#pager'>+098765432100</gd:phoneNumber><gd:structuredPostalAddress rel='http://schemas.google.com/g/2005#home'>"
            "<gd:city>A Random City</gd:city><gd:street>Some Street</gd:street><gd:region>Some state</gd:region><gd:postcode>41000</gd:postcode><gd:country>Czech Republic</gd:country>"
            "<gd:formattedAddress>Some Street\nPO BOX His POBOX\nA Random City,  Some state 41000\n\nCZECH REPUBLIC</gd:formattedAddress></gd:structuredPostalAddress>"
            "<gd:structuredPostalAddress rel='http://schemas.google.com/g/2005#work'></gd:structuredPostalAddress><gContact:birthday when='1991-05-20'/>";

}

void TestContacts::toXML()
{
    QFETCH(Contact::Ptr, contact);
    QFETCH(QString, xml);

    Addressbook *book = new Addressbook();
    KGoogleObject *obj = dynamic_cast<KGoogleObject*>(contact.data());
    QByteArray outputXml = book->objectToXML(obj);

    QCOMPARE(QString(outputXml), xml);
}


QTEST_MAIN(TestContacts)
#include "testcontacts.moc"
