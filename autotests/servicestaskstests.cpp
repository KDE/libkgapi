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

#include "servicestaskstests.h"

#include "objects/task.h"
#include "objects/tasklist.h"
#include "services/tasks.h"

void ServicesTasksTests::compareTasks(const KGAPI::Objects::Task* task, const QSettings& values, bool fromSerializer)
{
    if (!fromSerializer) {
        QCOMPARE(task->uid(), values.value("uid").toString());
        QCOMPARE(task->etag(), values.value("etag").toString());
        QCOMPARE(task->lastModified(), QDateTime::fromString(values.value("lastModified").toString(), Qt::ISODate));
    }
    QCOMPARE(task->summary(), values.value("summary").toString());
    QCOMPARE(task->description(), values.value("description").toString());
    QCOMPARE((int) task->status(), values.value("status").toInt());
    QCOMPARE(task->dtDue(), QDateTime::fromString(values.value("due").toString(), Qt::ISODate));
    QCOMPARE(task->completed(), QDateTime::fromString(values.value("completed").toString(), Qt::ISODate));
    QCOMPARE(task->deleted(), values.value("deleted").toBool());
    QCOMPARE(task->relatedTo(), values.value("parent").toString());
}

void ServicesTasksTests::compareTasklists(const KGAPI::Objects::TaskList* taskList, const QSettings& values, bool fromSerializer)
{
    if (!fromSerializer)
        QCOMPARE(taskList->uid(), values.value("uid").toString());
    QCOMPARE(taskList->title(), values.value("title").toString());
}

void ServicesTasksTests::testTasksParser()
{
   KGAPI::Services::Tasks service;

    QSettings src("../../tests/data/tasks.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Objects::Task *task;

        src.beginGroup(group);

        QFile jsonSrc("../../tests/data/json/task_" + group + ".json");
        if (jsonSrc.exists()) {
            qDebug() << "Running JSON parser test" << group;
            jsonSrc.open(QIODevice::ReadOnly);
            task = static_cast< KGAPI::Objects::Task* >(service.JSONToObject(jsonSrc.readAll()));
            compareTasks(task, src);
            jsonSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping JSON parser test" << group << "(missing source)";
        }

        src.endGroup();
    }
}

void ServicesTasksTests::testTasklistsParser()
{
   KGAPI::Services::Tasks service;

    QSettings src("../../tests/data/tasklists.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Objects::TaskList *tasklist;

        src.beginGroup(group);

        QFile jsonSrc("../../tests/data/json/tasklist_" + group + ".json");
        if (jsonSrc.exists()) {
            qDebug() << "Running JSON parser test" << group;
            jsonSrc.open(QIODevice::ReadOnly);
            tasklist = static_cast< KGAPI::Objects::TaskList* >(service.JSONToObject(jsonSrc.readAll()));
            compareTasklists(tasklist, src);
            jsonSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping JSON parser test" << group << "(missing source)";
        }

        src.endGroup();
    }
}

void ServicesTasksTests::testTasksSerializer()
{
    KGAPI::Services::Tasks service;

    QSettings src("../../tests/data/tasks.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Object *task;

        src.beginGroup(group);

        QFile jsonSrc("../../tests/data/json/task_" + group + ".json");
        if (jsonSrc.exists()) {
            qDebug() << "Running JSON serializer test" << group;
            jsonSrc.open(QIODevice::ReadOnly);
            task = service.JSONToObject(jsonSrc.readAll());
            QByteArray rawJson = service.objectToJSON(task);
            task = service.JSONToObject(rawJson);
            compareTasks(static_cast< KGAPI::Objects::Task* >(task), src, true);
            jsonSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping JSON serializer test" << group << "(missing source)";
        }

        src.endGroup();
    }
}

void ServicesTasksTests::testTasklistsSerializer()
{
    KGAPI::Services::Tasks service;

    QSettings src("../../tests/data/tasklists.ini", QSettings::IniFormat);
    QStringList groups = src.childGroups();

    Q_FOREACH (const QString &group, groups) {
        KGAPI::Object *task;

        src.beginGroup(group);

        QFile jsonSrc("../../tests/data/json/tasklist_" + group + ".json");
        if (jsonSrc.exists()) {
            qDebug() << "Running JSON serializer test" << group;
            jsonSrc.open(QIODevice::ReadOnly);
            task = service.JSONToObject(jsonSrc.readAll());
            QByteArray rawJson = service.objectToJSON(task);
            task = service.JSONToObject(rawJson);
            compareTasklists(static_cast< KGAPI::Objects::TaskList* >(task), src, true);
            jsonSrc.close();
            qDebug() << "Passed";
        } else {
            qDebug() << "Skipping JSON serializer test" << group << "(missing source)";
        }

        src.endGroup();
    }
}



void ServicesTasksTests::testUrls()
{
    QCOMPARE(KGAPI::Services::Tasks::fetchTaskListsUrl(),
             QUrl("https://www.googleapis.com/tasks/v1/users/@me/lists"));
    QCOMPARE(KGAPI::Services::Tasks::createTaskListUrl(),
             QUrl("https://www.googleapis.com/tasks/v1/users/@me/lists"));
    QCOMPARE(KGAPI::Services::Tasks::updateTaskListUrl("1234abcd"),
             QUrl("https://www.googleapis.com/tasks/v1/users/@me/lists/1234abcd"));
    QCOMPARE(KGAPI::Services::Tasks::removeTaskListUrl("1234abcd"),
             QUrl("https://www.googleapis.com/tasks/v1/users/@me/lists/1234abcd"));

    QCOMPARE(KGAPI::Services::Tasks::fetchAllTasksUrl("1234abcd"),
             QUrl("https://www.googleapis.com/tasks/v1/lists/1234abcd/tasks"));
    QCOMPARE(KGAPI::Services::Tasks::fetchTaskUrl("1234abcd", "5678efgh"),
             QUrl("https://www.googleapis.com/tasks/v1/lists/1234abcd/tasks/5678efgh"));
    QCOMPARE(KGAPI::Services::Tasks::createTaskUrl("1234abcd"),
             QUrl("https://www.googleapis.com/tasks/v1/lists/1234abcd/tasks"));
    QCOMPARE(KGAPI::Services::Tasks::updateTaskUrl("1234abcd", "5678efgh"),
             QUrl("https://www.googleapis.com/tasks/v1/lists/1234abcd/tasks/5678efgh"));
    QCOMPARE(KGAPI::Services::Tasks::removeTaskUrl("1234abcd", "5678efgh"),
             QUrl("https://www.googleapis.com/tasks/v1/lists/1234abcd/tasks/5678efgh"));
    QCOMPARE(KGAPI::Services::Tasks::moveTaskUrl("1234abcd", "5678efgh", "dcba4321"),
             QUrl("https://www.googleapis.com/tasks/v1/lists/1234abcd/tasks/5678efgh/move?parent=dcba4321"));
}

QTEST_KDEMAIN(ServicesTasksTests, NoGUI);
