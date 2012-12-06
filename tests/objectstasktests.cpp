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

#include "objectstasktests.h"

#include <objects/task.h>

using namespace KGAPI;

void ObjectsTaskTests::testTask_data()
{
    QTest::addColumn< bool >("deleted");

    QTest::newRow("task1")
            << true;
    QTest::newRow("task2")
            << false;
}

void ObjectsTaskTests::testTask()
{
    KGAPI::Objects::Task task;

    QFETCH(bool, deleted);

    task.setDeleted(deleted);

    QCOMPARE(task.deleted(), deleted);
}

QTEST_KDEMAIN(ObjectsTaskTests, NoGUI);
