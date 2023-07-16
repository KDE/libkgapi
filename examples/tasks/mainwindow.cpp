/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mainwindow.h"

#include "core/account.h"
#include "core/authjob.h"
#include "tasks/taskcreatejob.h"
#include "tasks/taskfetchjob.h"
#include "tasks/tasklist.h"
#include "tasks/tasklistcreatejob.h"
#include "tasks/tasklistfetchjob.h"

#include <QDebug>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Initialize GUI */
    ui.setupUi(this);
    ui.errorLabel->setVisible(false);
    connect(ui.authButton, &QAbstractButton::clicked, this, &MainWindow::authenticate);
    connect(ui.createTaskListButton, &QAbstractButton::clicked, this, &MainWindow::slotCreateTaskList);
    connect(ui.taskListsList, &QListWidget::itemSelectionChanged, this, &MainWindow::taskListSelected);
}

void MainWindow::authenticate()
{
    auto account = KGAPI2::AccountPtr::create();
    account->setScopes({KGAPI2::Account::tasksScopeUrl()});

    /* Create AuthJob to retrieve OAuth tokens for the account */
    auto *authJob = new KGAPI2::AuthJob(account, QStringLiteral("554041944266.apps.googleusercontent.com"), QStringLiteral("mdT1DjzohxN3npUUzkENT0gO"));
    connect(authJob, &KGAPI2::Job::finished, this, [this, authJob]() {
        /* Always remember to delete the jobs, otherwise your application will
         * leak memory. */
        authJob->deleteLater();

        if (authJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(authJob->errorString()));
            ui.errorLabel->setVisible(true);
            return;
        }

        m_account = authJob->account();

        ui.authStatusLabel->setText(QStringLiteral("Authenticated"));
        enableCreateTaskList(true);

        QMetaObject::invokeMethod(this, &MainWindow::slotFetchTaskLists, Qt::QueuedConnection);
    });
}

void MainWindow::slotFetchTaskLists()
{
    if (m_account.isNull()) {
        ui.errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        ui.errorLabel->setVisible(true);
        ui.authButton->setVisible(true);
        return;
    }

    auto *fetchJob = new KGAPI2::TaskListFetchJob(m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished, this, [this, fetchJob]() {
        fetchJob->deleteLater();

        if (fetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            return;
        }

        /* Get all items the job has retrieved */
        const auto objects = fetchJob->items();
        ui.taskListsList->clear();
        for (const auto &object : objects) {
            const auto taskList = object.dynamicCast<KGAPI2::TaskList>();

            /* Convert the taskList to QListWidget item */
            auto *item = new QListWidgetItem(ui.taskListsList);
            item->setText(taskList->title());
            item->setData(Qt::UserRole, taskList->uid());

            ui.taskListsList->addItem(item);
        }
    });
}

void MainWindow::taskListSelected()
{
    const bool hasSelection = (ui.taskListsList->selectedItems().count() != 0);

    ui.taskListTasksList->clear();
    enableCreateTask(hasSelection);

    if (!hasSelection) {
        return;
    }

    const auto taskListId = ui.taskListsList->selectedItems().at(0)->data(Qt::UserRole).toString();
    auto *fetchJob = new KGAPI2::TaskFetchJob(taskListId, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished, this, [this, fetchJob]() {
        fetchJob->deleteLater();

        if (fetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            return;
        }

        /* Get all items the job has retrieved */
        const auto objects = fetchJob->items();
        for (const auto &object : objects) {
            const auto task = object.dynamicCast<KGAPI2::Task>();

            ui.taskListTasksList->addItem(task->summary());
        }
    });
}

void MainWindow::slotCreateTaskList()
{
    const auto title = ui.taskListNameEdit->text();

    auto taskList = KGAPI2::TaskListPtr::create();
    taskList->setTitle(title);

    auto *createJob = new KGAPI2::TaskListCreateJob(taskList, m_account, this);
    connect(createJob, &KGAPI2::Job::finished, this, [this, createJob]() {
        createJob->deleteLater();

        if (createJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(createJob->errorString()));
            ui.errorLabel->setVisible(true);
            return;
        }

        QMetaObject::invokeMethod(this, &MainWindow::slotFetchTaskLists, Qt::QueuedConnection);
    });
}

void MainWindow::enableCreateTaskList(bool enabled)
{
    ui.taskListNameEdit->setEnabled(enabled);
    ui.createTaskListButton->setEnabled(enabled);
}

void MainWindow::enableCreateTask(bool enabled)
{
    ui.taskNameEdit->setEnabled(enabled);
    ui.createTaskButton->setEnabled(enabled);
}

#include "moc_mainwindow.cpp"
