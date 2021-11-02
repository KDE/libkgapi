/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"

#include "core/types.h"
#include "tasks/task.h"

namespace KGAPI2 {
  class Job;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:
    /**
     * Retrieves tokens from Google that we will use to authenticate
     * further requests
     */
    void authenticate();

    /**
     * Retrieves list of all task lists
     */
    void slotFetchTaskLists();

    /**
     * A specific task list has been selected. Sends a request
     * to Google to retrieve the associated tasks
     */
    void taskListSelected();

    /**
     * Slot to create a new task list
     */
    void slotCreateTaskList();

private:
    Ui::MainWindow ui;

    KGAPI2::AccountPtr m_account;

    void enableCreateTaskList(bool enabled);
    void enableCreateTask(bool enabled);
};

