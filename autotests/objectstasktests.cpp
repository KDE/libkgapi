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
