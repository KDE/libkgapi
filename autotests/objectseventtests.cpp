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

#include "objectseventtests.h"

#include <objects/event.h>

using namespace KGAPI;

void ObjectsEventTests::testEvent_data()
{
    QTest::addColumn< bool >("deleted");
    QTest::addColumn< bool >("useDefaultReminders");

    QTest::newRow("event1")
            << true
            << true;

    QTest::newRow("event2")
            << false
            << false;
}

void ObjectsEventTests::testEvent()
{
    KGAPI::Objects::Event event;

    QFETCH(bool, deleted);
    QFETCH(bool, useDefaultReminders);

    event.setDeleted(deleted);
    event.setUseDefaultReminders(useDefaultReminders);

    QCOMPARE(event.deleted(), deleted);
    QCOMPARE(event.useDefaultReminders(), useDefaultReminders);
}

QTEST_KDEMAIN(ObjectsEventTests, NoGUI);
