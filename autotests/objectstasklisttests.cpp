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
