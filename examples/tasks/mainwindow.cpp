/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <tasks/tasklist.h>
#include <tasks/tasklistcreatejob.h>
#include <tasks/tasklistfetchjob.h>
#include <tasks/taskfetchjob.h>
#include <tasks/taskcreatejob.h>
#include <authjob.h>
#include <account.h>

#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialize GUI */
    m_ui->setupUi(this);
    m_ui->errorLabel->setVisible(false);
    connect(m_ui->authButton, &QAbstractButton::clicked,
            this, &MainWindow::authenticate);
    connect(m_ui->createTaskListButton, &QAbstractButton::clicked,
            this, &MainWindow::slotCreateTaskList);
    connect(m_ui->taskListsList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::taskListSelected);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::authenticate()
{
    KGAPI2::AccountPtr account(new KGAPI2::Account);
    account->setScopes( QList<QUrl>() << KGAPI2::Account::tasksScopeUrl() );

    /* Create AuthJob to retrieve OAuth tokens for the account */
    KGAPI2::AuthJob *authJob = new KGAPI2::AuthJob(
        account,
        QStringLiteral("554041944266.apps.googleusercontent.com"),
        QStringLiteral("mdT1DjzohxN3npUUzkENT0gO"));
    connect(authJob, &KGAPI2::Job::finished,
             this, &MainWindow::slotAuthJobFinished);
}

void MainWindow::slotAuthJobFinished(KGAPI2::Job *job)
{
    KGAPI2::AuthJob *authJob = qobject_cast<KGAPI2::AuthJob*>(job);
    Q_ASSERT(authJob);
    /* Always remember to delete the jobs, otherwise your application will
     * leak memory. */
    authJob->deleteLater();

    if (authJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(authJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    m_account = authJob->account();
    qDebug() << "Got access token" << m_account->accessToken();

    m_ui->authStatusLabel->setText(QStringLiteral("Authenticated"));
    enableCreateTaskList(true);

    QMetaObject::invokeMethod(this, &MainWindow::slotFetchTaskLists, Qt::QueuedConnection);
}

void MainWindow::slotFetchTaskLists()
{
    qDebug() << Q_FUNC_INFO;
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    KGAPI2::TaskListFetchJob *fetchJob = new KGAPI2::TaskListFetchJob(m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTaskListsFetchJobFinished);
}

void MainWindow::slotTaskListsFetchJobFinished(KGAPI2::Job *job)
{
    qDebug() << Q_FUNC_INFO << job;
    KGAPI2::TaskListFetchJob *fetchJob = qobject_cast<KGAPI2::TaskListFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    /* Get all items the job has retrieved */
    const KGAPI2::ObjectsList objects = fetchJob->items();
    m_ui->taskListsList->clear();
    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::TaskListPtr taskList = object.dynamicCast<KGAPI2::TaskList>();

        /* Convert the taskList to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->taskListsList);
        item->setText(taskList->title());
        item->setData(Qt::UserRole, taskList->uid());

        m_ui->taskListsList->addItem(item);
    }
}

void MainWindow::taskListSelected()
{
    qDebug() << Q_FUNC_INFO;
    bool hasSelection = (m_ui->taskListsList->selectedItems().count() != 0);

    m_ui->taskListTasksList->clear();

    enableCreateTask(hasSelection);

    if (!hasSelection) {
        return;
    }

    const QString taskListId = m_ui->taskListsList->selectedItems().at(0)->data(Qt::UserRole).toString();
    KGAPI2::TaskFetchJob *fetchJob = new KGAPI2::TaskFetchJob(taskListId, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTasksFetchJobFinished);
}

void MainWindow::slotTasksFetchJobFinished(KGAPI2::Job *job)
{
    qDebug() << Q_FUNC_INFO << job;
    KGAPI2::TaskFetchJob *fetchJob = qobject_cast<KGAPI2::TaskFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    /* Get all items the job has retrieved */
    const KGAPI2::ObjectsList objects = fetchJob->items();
    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::TaskPtr task = object.dynamicCast<KGAPI2::Task>();

        m_ui->taskListTasksList->addItem(task->summary());
    }
}

void MainWindow::slotCreateTaskList()
{
    qDebug() << Q_FUNC_INFO;
    const QString title = m_ui->taskListNameEdit->text();

    KGAPI2::TaskListPtr taskList = KGAPI2::TaskListPtr::create();
    taskList->setTitle(title);

    KGAPI2::TaskListCreateJob *fetchJob = new KGAPI2::TaskListCreateJob(taskList, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotCreateTaskListFinished);
}

void MainWindow::slotCreateTaskListFinished(KGAPI2::Job *job)
{
    qDebug() << Q_FUNC_INFO << job;
    KGAPI2::TaskListCreateJob *fetchJob = qobject_cast<KGAPI2::TaskListCreateJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    // QMetaObject::invokeMethod(this, &MainWindow::slotFetchTaskLists, Qt::QueuedConnection);
}

void MainWindow::slotCreateTask()
{
    qDebug() << Q_FUNC_INFO;
    // const QString title = m_ui->taskListNameEdit->text();

    // KGAPI2::TaskListPtr taskList = KGAPI2::TaskListPtr::create();
    // taskList->setTitle(title);

    // KGAPI2::TaskCreateJob *fetchJob = new KGAPI2::TaskCreateJob(taskList, m_account, this);
    // connect(fetchJob, &KGAPI2::Job::finished,
            // this, &MainWindow::slotCreateTaskListFinished);
}

void MainWindow::slotCreateTaskFinished(KGAPI2::Job *job)
{
    qDebug() << Q_FUNC_INFO << job;
    // KGAPI2::TaskFetchJob *fetchJob = qobject_cast<KGAPI2::TaskFetchJob*>(job);
    // Q_ASSERT(fetchJob);
    // fetchJob->deleteLater();

    // if (fetchJob->error() != KGAPI2::NoError) {
        // m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        // m_ui->errorLabel->setVisible(true);
        // return;
    // }

    // QMetaObject::invokeMethod(this, &MainWindow::slotFetchTaskLists, Qt::QueuedConnection);
}

void MainWindow::enableCreateTaskList(bool enabled)
{
    m_ui->taskListNameEdit->setEnabled(enabled);
    m_ui->createTaskListButton->setEnabled(enabled);
}

void MainWindow::enableCreateTask(bool enabled)
{
    m_ui->taskNameEdit->setEnabled(enabled);
    m_ui->createTaskButton->setEnabled(enabled);
}
