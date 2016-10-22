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

#include "objectstasklisttests.h"

#include <objects/tasklist.h>

using namespace KGAPI;

void ObjectsTaskListTests::testTaskList_data()
{
    QTest::addColumn< QString >("uid");
    QTest::addColumn< QString >("title");

    QTest::newRow("taskList1_cz")
            << "1234abcd"
            << "Nějaké úkoly";
}

void ObjectsTaskListTests::testTaskList()
{
    KGAPI::Objects::TaskList taskList;

    QFETCH(QString, uid);
    QFETCH(QString, title);

    taskList.setUid(uid);
    taskList.setTitle(title);

    QCOMPARE(taskList.uid(), uid);
    QCOMPARE(taskList.title(), title);
}

QTEST_KDEMAIN(ObjectsTaskListTests, NoGUI);
