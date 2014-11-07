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

#include "servicescontactstests.h"

#include "services/contacts.h"
#include "objects/contactsgroup.h"
#include "objects/contact.h"

#include <KContacts/PhoneNumber>
#include <KContacts/Address>

#include <QFile>
#include <QStringList>

class IM {
  public:
      explicit IM() { }
      IM(const QString &address, const KGAPI::Objects::Contact::IMProtocol &protocol):
        address(address),
        protocol(protocol) { }
      QString address;
      KGAPI::Objects::Contact::IMProtocol protocol;
};

Q_DECLARE_METATYPE(IM)
Q_DECLARE_METATYPE(QList< IM >)
Q_DECLARE_METATYPE(KContacts::PhoneNumber::List)
Q_DECLARE_METATYPE(KContacts::Address::List)
Q_DECLARE_METATYPE(KContacts::PhoneNumber)
Q_DECLARE_METATYPE(KContacts::Address)


void ServicesContactsTests::compareGroups(const KGAPI::Objects::ContactsGroup *group, const QSettings &src, bool fromSerializer)
{
    QVERIFY(group != 0);

    if (!fromSerializer) {
        QCOMPARE(group->id(), src.value("id").toString());
        QCOMPARE(group->updated(), QDateTime::fromString(src.value("updated").toString(), Qt::ISODate).dateTime());
        QCOMPARE(group->isSystemGroup(), src.value("isSystem").toBool());
    }
    QCOMPARE(group->title(), src.value("title").toString());
    QCOMPARE(group->content(), src.value("content").toString());
}

void ServicesContactsTests::testContactsGroupParsers()
{
    KGAPI::Services::Contacts service;

    QSettings src("../../tests/data/contacts_groups.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Objects::ContactsGroup *contactGroup;

        src.beginGroup(group);

        QFile jsonSrc("../../tests/data/json/contacts_group_" + group + ".json");
        if (jsonSrc.exists()) {
            qDebug() << "Running JSON parser test" << group;
            jsonSrc.open(QIODevice::ReadOnly);
            contactGroup = static_cast< KGAPI::Objects::ContactsGroup* >(service.JSONToObject(jsonSrc.readAll()));
            compareGroups(contactGroup, src);
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping JSON parser test" << group << "(missing source)";
        }

        QFile xmlSrc("../../tests/data/xml/contacts_group_" + group + ".xml");
        if (xmlSrc.exists()) {
            qDebug() << "Running XML parser test" << group;
            xmlSrc.open(QIODevice::ReadOnly);
            contactGroup = static_cast< KGAPI::Objects::ContactsGroup* >(service.XMLToObject(xmlSrc.readAll()));
            compareGroups(contactGroup, src);
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping XML parser test" << group << "(missing source)";
        }

        src.endGroup();
    }
}


void ServicesContactsTests::compareContacts(const KGAPI::Objects::Contact *contact, const QSettings &src, bool fromSerializer)
{
    QVERIFY(contact != 0);

    if (!fromSerializer) {
        QCOMPARE(contact->uid(), src.value("id").toString());
        QCOMPARE(contact->updated(), QDateTime::fromString(src.value("updated").toString(), Qt::ISODate));
        QCOMPARE(contact->name(), src.value("name").toString());
        QCOMPARE(contact->photoUrl(), src.value("photoUrl").toUrl());
        QCOMPARE(contact->formattedName(), src.value("formattedName").toString());
    }
    QCOMPARE(contact->givenName(), src.value("givenName").toString());
    QCOMPARE(contact->familyName(), src.value("familyName").toString());
    QCOMPARE(contact->nickName(), src.value("nickname").toString());
    QCOMPARE(contact->emails(), src.value("emails").toStringList());

    /* FIXME Test IMs */

    QStringList phoneNumbers = src.value("phoneNumbers").toStringList();
    QList< QVariant > phoneTypes = src.value("phoneTypes").toList();
    QCOMPARE(contact->phoneNumbers().count(), phoneNumbers.count());
    for (int i = 0; i < contact->phoneNumbers().count(); i++) {
        QCOMPARE(contact->phoneNumbers().at(i).number(),
                    phoneNumbers.at(i));
        QCOMPARE((quint32) contact->phoneNumbers().at(i).type(),
                    phoneTypes.at(i).toUInt());
    }

    QStringList streets = src.value("streets").toStringList();
    QStringList extendedAddresses = src.value("postalExtendedAddresses").toStringList();
    QStringList postalCodes = src.value("postalCodes").toStringList();
    QStringList postalLocalities = src.value("postalLocalities").toStringList();
    QStringList postalCountries = src.value("postalCountries").toStringList();
    QStringList postalLabels = src.value("postalLabels").toStringList();
    QStringList postalPOBoxes = src.value("postalPOBoxes").toStringList();
    QStringList postalRegions = src.value("postalRegions").toStringList();
    QList< QVariant > postalTypes = src.value("postalTypes").toList();
    QCOMPARE(contact->addresses().count(), streets.count());
    for (int i = 0; i < contact->addresses().count(); i++) {
        KContacts::Address address = contact->addresses().at(i);

        QCOMPARE(address.country(), postalCountries.at(i));
        QCOMPARE(address.extended(), extendedAddresses.at(i));
        QCOMPARE(address.locality(), postalLocalities.at(i));
        QCOMPARE(address.postalCode(), postalCodes.at(i));
        QCOMPARE(address.postOfficeBox(), postalPOBoxes.at(i));
        QCOMPARE(address.region(), postalRegions.at(i));
        QCOMPARE(address.street(), streets.at(i));
        QCOMPARE((quint32) address.type(), postalTypes.at(i).toUInt());
    }

    QCOMPARE(contact->spousesName(), src.value("spouse").toString());
    QCOMPARE(contact->managersName(), src.value("manager").toString());
    QCOMPARE(contact->assistantsName(), src.value("assistant").toString());
    QCOMPARE(contact->profession(), src.value("profession").toString());
    QCOMPARE(contact->organization(), src.value("organization").toString());
    QCOMPARE(contact->department(), src.value("department").toString());
    QCOMPARE(contact->title(), src.value("title").toString());
    QCOMPARE(contact->office(), src.value("office").toString());
    QCOMPARE(contact->anniversary(), src.value("anniversary").toString());
    QCOMPARE(contact->blogFeed(), src.value("blog").toString());
    QCOMPARE(contact->url(), KUrl(src.value("homepage").toUrl()));
    QCOMPARE(contact->groups(), src.value("groups").toStringList());
}

void ServicesContactsTests::testContactsParsers()
{
    KGAPI::Services::Contacts service;

    QSettings src("../../tests/data/contacts.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Objects::Contact *contact;

        src.beginGroup(group);

        QFile jsonSrc("../../tests/data/json/contact_" + group + ".json");
        if (jsonSrc.exists()) {
            qDebug() << "Running JSON parser test" << group;
            jsonSrc.open(QIODevice::ReadOnly);
            contact = static_cast< KGAPI::Objects::Contact* >(service.JSONToObject(jsonSrc.readAll()));
            compareContacts(contact, src);
            jsonSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping JSON parser test" << group << "(missing source)";
        }

        QFile xmlSrc("../../tests/data/xml/contacts_" + group + ".xml");
        if (xmlSrc.exists()) {
            qDebug() << "Running XML parser test" << group;
            xmlSrc.open(QIODevice::ReadOnly);
            contact = static_cast< KGAPI::Objects::Contact* >(service.XMLToObject(jsonSrc.readAll()));
            compareContacts(contact, src);
            xmlSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping XML parser test" << group << "(missing source)";
        }

        src.endGroup();
    }
}

void ServicesContactsTests::testContactsGroupSerializer()
{
    KGAPI::Services::Contacts service;

    QSettings src("../../tests/data/contacts_groups.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Object *contactGroup;

        src.beginGroup(group);

        /* JSON serializer is not implemetented - we don't need it :) */

        QFile xmlSrc("../../tests/data/xml/contacts_group_" + group + ".xml");
        if (xmlSrc.exists()) {
            qDebug() << "Running XML serializer test" << group;
            xmlSrc.open(QIODevice::ReadOnly);
            contactGroup = service.XMLToObject(xmlSrc.readAll());
            QByteArray rawXml = service.objectToXML(contactGroup);
            rawXml.prepend("<entry xmlns=\"http://www.w3.org/2005/Atom\" xmlns:gContact=\"http://schemas.google.com/contact/2008\" xmlns:batch=\"http://schemas.google.com/gdata/batch\" xmlns:gd=\"http://schemas.google.com/g/2005\">");
            rawXml.append("</entry>");
            contactGroup = service.XMLToObject(rawXml);
            compareGroups(static_cast< KGAPI::Objects::ContactsGroup* >(contactGroup), src, true);
            xmlSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping XML serializer test" << group << "(missing source)";
        }

        src.endGroup();
    }
}

void ServicesContactsTests::testContactsSerializer()
{
    KGAPI::Services::Contacts service;

    QSettings src("../../tests/data/contacts.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Object *contact;

        src.beginGroup(group);

        /* JSON serializer is not implemetented - we don't need it :) */

        QFile xmlSrc("../../tests/data/xml/contact_" + group + ".xml");
        if (xmlSrc.exists()) {
            qDebug() << "Running XML serializer test" << group;
            xmlSrc.open(QIODevice::ReadOnly);
            contact = service.XMLToObject(xmlSrc.readAll());
            QByteArray rawXml = service.objectToXML(contact);
            rawXml.prepend("<entry xmlns=\"http://www.w3.org/2005/Atom\" xmlns:gContact=\"http://schemas.google.com/contact/2008\" xmlns:batch=\"http://schemas.google.com/gdata/batch\" xmlns:gd=\"http://schemas.google.com/g/2005\">");
            rawXml.append("</entry>");
            contact = service.XMLToObject(rawXml);
            compareContacts(static_cast< KGAPI::Objects::Contact* >(contact), src, true);
            xmlSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping XML serializer test" << group << "(missing source)";
        }

        src.endGroup();
    }
}

void ServicesContactsTests::testUrls()
{
    QCOMPARE(KGAPI::Services::Contacts::fetchAllContactsUrl("xyz%40no.where", true),
             QUrl("https://www.google.com/m8/feeds/contacts/xyz%40no.where/full?alt=json&showdeleted=true"));
    QCOMPARE(KGAPI::Services::Contacts::fetchContactUrl("xyz%40no.where", "5678efgh"),
             QUrl("https://www.google.com/m8/feeds/contacts/xyz%40no.where/full/5678efgh?alt=json"));
    QCOMPARE(KGAPI::Services::Contacts::createContactUrl("xyz%40no.where"),
             QUrl("https://www.google.com/m8/feeds/contacts/xyz%40no.where/full"));
    QCOMPARE(KGAPI::Services::Contacts::updateContactUrl("xyz%40no.where", "5678efgh"),
             QUrl("https://www.google.com/m8/feeds/contacts/xyz%40no.where/full/5678efgh"));
    QCOMPARE(KGAPI::Services::Contacts::removeContactUrl("xyz%40no.where", "5678efgh"),
             QUrl("https://www.google.com/m8/feeds/contacts/xyz%40no.where/full/5678efgh"));

    QCOMPARE(KGAPI::Services::Contacts::fetchAllGroupsUrl("xyz%40no.where"),
             QUrl("https://www.google.com/m8/feeds/groups/xyz%40no.where/full?alt=json"));
    QCOMPARE(KGAPI::Services::Contacts::fetchGroupUrl("xyz%40no.where", "1234abcd"),
             QUrl("https://www.google.com/m8/feeds/groups/xyz%40no.where/base/1234abcd?alt=json"));
    QCOMPARE(KGAPI::Services::Contacts::createGroupUrl("xyz%40no.where"),
             QUrl("https://www.google.com/m8/feeds/groups/xyz%40no.where/full"));
    QCOMPARE(KGAPI::Services::Contacts::updateGroupUrl("xyz%40no.where", "1234abcd"),
             QUrl("https://www.google.com/m8/feeds/groups/xyz%40no.where/full/1234abcd"));
    QCOMPARE(KGAPI::Services::Contacts::removeGroupUrl("xyz%40no.where", "1234abcd"),
             QUrl("https://www.google.com/m8/feeds/groups/xyz%40no.where/full/1234abcd"));

    QCOMPARE(KGAPI::Services::Contacts::photoUrl("xyz%40no.where", "1234abcd"),
             QUrl("https://www.google.com/m8/feeds/photos/media/xyz%40no.where/1234abcd"));
}

QTEST_KDEMAIN(ServicesContactsTests, NoGUI);
