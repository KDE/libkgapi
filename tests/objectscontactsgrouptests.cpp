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

#include "objectscontactsgrouptests.h"

#include <objects/contactsgroup.h>

using namespace KGAPI;

void ObjectsContactsGroupTests::testContactsGroup_data()
{
    QTest::addColumn< QString >("id");
    QTest::addColumn< QString >("title");
    QTest::addColumn< QString >("content");
    QTest::addColumn< QDateTime >("updated");
    QTest::addColumn< bool >("isSystemGroup");

    QTest::newRow("contactsGroup1_czech")
            << "1234abcd"
            << "Testovací skupina"
            << "Nejbližší přátelé a rodina"
            << QDateTime::currentDateTime()
            << false;
}


void ObjectsContactsGroupTests::testContactsGroup()
{
    KGAPI::Objects::ContactsGroup contactsGroup;

    QFETCH(QString, id);
    QFETCH(QString, title);
    QFETCH(QString, content);
    QFETCH(QDateTime, updated);
    QFETCH(bool, isSystemGroup);

    contactsGroup.setId(id);
    contactsGroup.setTitle(title);
    contactsGroup.setContent(content);
    contactsGroup.setUpdated(updated);
    contactsGroup.setIsSystemGroup(isSystemGroup);

    QCOMPARE(contactsGroup.id(), id);
    QCOMPARE(contactsGroup.title(), title);
    QCOMPARE(contactsGroup.content(), content);
    QCOMPARE(contactsGroup.updated(), updated);
    QCOMPARE(contactsGroup.isSystemGroup(), isSystemGroup);
}

QTEST_KDEMAIN(ObjectsContactsGroupTests, NoGUI);
