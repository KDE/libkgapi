/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "people/person.h"
#include "people/emailaddress.h"
#include "people/phonenumber.h"
#include "people/address.h"
#include "people/name.h"

#include <KContacts/Addressee>
#include <KContacts/Email>
#include <KContacts/PhoneNumber>
#include <KContacts/Address>

namespace KGAPI2::People {

class KContactsConversionTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmailConversion()
    {
        EmailAddress email;
        email.setValue(QStringLiteral("test@example.com"));
        email.setType(QStringLiteral("home"));

        KContacts::Email kemail = email.toKContactsEmail();
        QCOMPARE(kemail.mail(), QStringLiteral("test@example.com"));
        QCOMPARE(kemail.type(), KContacts::Email::Home);

        email.setType(QStringLiteral("work"));
        kemail = email.toKContactsEmail();
        QCOMPARE(kemail.type(), KContacts::Email::Work);

        email.setType(QStringLiteral("other"));
        kemail = email.toKContactsEmail();
        QCOMPARE(kemail.type(), KContacts::Email::Other);
    }

    void testPhoneConversion()
    {
        PhoneNumber phone;
        phone.setValue(QStringLiteral("123456789"));
        phone.setType(QStringLiteral("home"));

        KContacts::PhoneNumber kphone = phone.toKContactsPhoneNumber();
        QCOMPARE(kphone.number(), QStringLiteral("123456789"));
        QCOMPARE(kphone.type(), KContacts::PhoneNumber::Home);

        phone.setType(QStringLiteral("work"));
        kphone = phone.toKContactsPhoneNumber();
        QCOMPARE(kphone.type(), KContacts::PhoneNumber::Work);

        phone.setType(QStringLiteral("mobile"));
        kphone = phone.toKContactsPhoneNumber();
        QCOMPARE(kphone.type(), KContacts::PhoneNumber::Cell);
    }

    void testAddressConversion()
    {
        Address addr;
        addr.setCity(QStringLiteral("City"));
        addr.setCountry(QStringLiteral("Country"));
        addr.setStreetAddress(QStringLiteral("Street 123"));
        addr.setPostalCode(QStringLiteral("12345"));
        addr.setType(QStringLiteral("home"));

        KContacts::Address kaddr = addr.toKContactsAddress();
        QCOMPARE(kaddr.locality(), QStringLiteral("City"));
        QCOMPARE(kaddr.country(), QStringLiteral("Country"));
        QCOMPARE(kaddr.street(), QStringLiteral("Street 123"));
        QCOMPARE(kaddr.postalCode(), QStringLiteral("12345"));
        QVERIFY(kaddr.type() & KContacts::Address::Home);

        KContacts::Address kaddr2;
        kaddr2.setLocality(QStringLiteral("OtherCity"));
        kaddr2.setType(KContacts::Address::Work);
        
        Address addr2 = Address::fromKContactsAddress(kaddr2);
        QCOMPARE(addr2.city(), QStringLiteral("OtherCity"));
        QCOMPARE(addr2.type(), QStringLiteral("work"));
    }

    void testPersonConversionDeletions()
    {
        PersonPtr person = PersonPtr::create();
        
        EmailAddress email;
        email.setValue(QStringLiteral("old@example.com"));
        person->setEmailAddresses({email});
        
        QCOMPARE(person->emailAddresses().count(), 1);

        KContacts::Addressee addressee;
        // No emails in addressee
        
        person->setFromKContactsAddressee(addressee);
        QCOMPARE(person->emailAddresses().count(), 0); // Should be cleared now
    }
};

}

QTEST_GUILESS_MAIN(KGAPI2::People::KContactsConversionTest)

#include "kcontactsconversiontest.moc"
