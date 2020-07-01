/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <types.h>
#include <tasks/task.h>

namespace Ui {
    class MainWindow;
}

namespace KGAPI2 {
  class Job;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private Q_SLOTS:
    /**
     * Retrieves tokens from Google that we will use to authenticate
     * fursther requests
     */
    void authenticate();

    /**
     * Authentication has finished
     */
    void slotAuthJobFinished(KGAPI2::Job *job);

    /**
     * Retrieves list of all task lists
     */
    void slotFetchTaskLists();

    /**
     * Task lists listing was fetched.
     */
    void slotTaskListsFetchJobFinished(KGAPI2::Job *job);

    /**
     * A specific task list has been selected. Sends a request
     * to Google to retrieve the associated tasks
     */
    void taskListSelected();

    /**
     * Tasks listing was fetched.
     */
    void slotTasksFetchJobFinished(KGAPI2::Job *job);

    /**
     * Slot to create a new task list
     */
    void slotCreateTaskList();

    /**
     * Tasks List creation is finished
     */
    void slotCreateTaskListFinished(KGAPI2::Job *job);

    /**
     * Slot to create a new task
     */
    void slotCreateTask();

    /**
     * Tasks creation is finished
     */
    void slotCreateTaskFinished(KGAPI2::Job *job);

  private:
    Ui::MainWindow *m_ui;

    KGAPI2::AccountPtr m_account;

    void enableCreateTaskList(bool enabled);
    void enableCreateTask(bool enabled);
};

#endif // MAINWINDOW_H
