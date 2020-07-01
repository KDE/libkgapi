/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "taskstestutils.h"
#include "tasksservice.h"
#include "testutils.h"

#include <QFile>

KGAPI2::TaskPtr taskFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto task = KGAPI2::TasksService::JSONToTask(f.readAll());
    VERIFY_RET(task, {});
    return task;
}

KGAPI2::TaskListPtr taskListFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto taskList = KGAPI2::TasksService::JSONToTaskList(f.readAll());
    VERIFY_RET(taskList, {});
    return taskList;
}
