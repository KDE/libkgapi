/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drive/teamdrive.h"
#include "drive/teamdrivecreatejob.h"
#include "drive/teamdrivedeletejob.h"
#include "drive/teamdrivefetchjob.h"
#include "drive/teamdrivemodifyjob.h"
#include "drive/teamdrivesearchquery.h"
#include "drive/file.h"
#include "drive/filesearchquery.h"
#include "drive/filefetchjob.h"
#include "core/authjob.h"
#include "core/account.h"

#include <QListWidgetItem>
#include <QUuid>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialize GUI */
    m_ui->setupUi(this);
    m_ui->errorLabel->setVisible(false);
    connect(m_ui->authButton, &QAbstractButton::clicked,
            this, &MainWindow::authenticate);
    connect(m_ui->newTeamdriveButton, &QAbstractButton::clicked,
            this, &MainWindow::createTeamdrive);
    connect(m_ui->teamdriveListButton, &QAbstractButton::clicked,
            this, &MainWindow::fetchTeamdriveList);
    connect(m_ui->teamdriveSelectedDeleteButton, &QAbstractButton::clicked,
            this, &MainWindow::deleteSelectedTeamdrive);
    connect(m_ui->renameTeamdriveButton, &QAbstractButton::clicked,
            this, &MainWindow::renameSelectedTeamdrive);
    connect(m_ui->teamdriveList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::teamdriveSelected);
    connect(m_ui->teamdrivePreviewList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::teamdriveItemSelected);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::authenticate()
{
    KGAPI2::AccountPtr account(new KGAPI2::Account);
    account->setScopes( QList<QUrl>() << KGAPI2::Account::driveScopeUrl() );

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

    m_ui->authStatusLabel->setText(QStringLiteral("Authenticated"));
    m_ui->teamdriveListButton->setEnabled(true);
    m_ui->newTeamdriveEdit->setEnabled(true);
    m_ui->newTeamdriveButton->setEnabled(true);
    m_ui->authButton->setEnabled(false);
}

void MainWindow::createTeamdrive()
{
    QString teamdriveName = m_ui->newTeamdriveEdit->text();
    if (teamdriveName.isEmpty()) {
        return;
    }
    QString requestId = QUuid::createUuid().toString();

    KGAPI2::Drive::TeamdrivePtr teamdrive = KGAPI2::Drive::TeamdrivePtr::create();
    teamdrive->setName(teamdriveName);

    KGAPI2::Drive::TeamdriveCreateJob *createJob = new KGAPI2::Drive::TeamdriveCreateJob(requestId, teamdrive, m_account, this);
    connect(createJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTeamdriveCreateJobFinished);
}

void MainWindow::slotTeamdriveCreateJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::TeamdriveCreateJob *createJob = qobject_cast<KGAPI2::Drive::TeamdriveCreateJob*>(job);
    Q_ASSERT(createJob);
    createJob->deleteLater();

    if (createJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(createJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->teamdriveListButton->setEnabled(true);
        return;
    }

    m_ui->newTeamdriveEdit->clear();
    fetchTeamdriveList();
}

void MainWindow::renameSelectedTeamdrive()
{
    QString teamdriveName = m_ui->renameTeamdriveEdit->text();
    if (teamdriveName.isEmpty()) {
        return;
    }
    QString teamdriveId = m_ui->teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::TeamdrivePtr teamdrive = KGAPI2::Drive::TeamdrivePtr::create();
    teamdrive->setId(teamdriveId);
    teamdrive->setName(teamdriveName);

    KGAPI2::Drive::TeamdriveModifyJob *modifyJob = new KGAPI2::Drive::TeamdriveModifyJob(teamdrive, m_account, this);
    connect(modifyJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTeamdriveModifyJobFinished);
}

void MainWindow::slotTeamdriveModifyJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::TeamdriveModifyJob *modifyJob = qobject_cast<KGAPI2::Drive::TeamdriveModifyJob*>(job);
    Q_ASSERT(modifyJob);
    modifyJob->deleteLater();

    if (modifyJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(modifyJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->teamdriveListButton->setEnabled(true);
        return;
    }

    fetchTeamdriveList();
}

void MainWindow::fetchTeamdriveList()
{
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    KGAPI2::Drive::TeamdriveFetchJob *fetchJob = new KGAPI2::Drive::TeamdriveFetchJob(m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotFetchJobFinished);

    m_ui->teamdriveListButton->setEnabled(false);
}

void MainWindow::slotFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::TeamdriveFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::TeamdriveFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->teamdriveListButton->setEnabled(true);
        return;
    }

    /* Get all items the job has retrieved */
    const KGAPI2::ObjectsList objects = fetchJob->items();
    m_ui->teamdriveList->clear();
    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::Drive::TeamdrivePtr teamdrive = object.dynamicCast<KGAPI2::Drive::Teamdrive>();

        /* Convert the teamdrive to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->teamdriveList);
        item->setText(teamdrive->name());
        item->setData(Qt::UserRole, teamdrive->id());

        m_ui->teamdriveList->addItem(item);
    }

    m_ui->teamdriveListButton->setEnabled(true);
}

void MainWindow::deleteSelectedTeamdrive() {
    const QString teamdrive_id = m_ui->teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::TeamdriveDeleteJob *deleteJob = new KGAPI2::Drive::TeamdriveDeleteJob(teamdrive_id, m_account, this);
    connect(deleteJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTeamdriveDeleteJobFinished);
}

void MainWindow::slotTeamdriveDeleteJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::TeamdriveDeleteJob *deleteJob = qobject_cast<KGAPI2::Drive::TeamdriveDeleteJob*>(job);
    Q_ASSERT(deleteJob);
    deleteJob->deleteLater();

    if (deleteJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(deleteJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->teamdriveListButton->setEnabled(true);
        return;
    }

    fetchTeamdriveList();
}

void MainWindow::teamdriveSelected()
{
    bool hasSelection = (m_ui->teamdriveList->selectedItems().count() != 0);

    m_ui->teamdriveSelectedDeleteButton->setEnabled(hasSelection);
    m_ui->renameTeamdriveButton->setEnabled(hasSelection);
    m_ui->renameTeamdriveEdit->setEnabled(hasSelection);

    m_ui->teamdrivePreviewList->clear();

    if (!hasSelection) {
        m_ui->renameTeamdriveEdit->clear();
        return;
    }

    const QString id = m_ui->teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();
    const QString name = m_ui->teamdriveList->selectedItems().at(0)->data(Qt::DisplayRole).toString();

    m_ui->renameTeamdriveEdit->setText(name);

    KGAPI2::Drive::FileSearchQuery query;
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Trashed, KGAPI2::Drive::FileSearchQuery::Equals, false);
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Parents, KGAPI2::Drive::FileSearchQuery::In, id);

    KGAPI2::Drive::FileFetchJob *fileFetchJob = new KGAPI2::Drive::FileFetchJob(query, m_account, nullptr);
    fileFetchJob->setFields({
        KGAPI2::Drive::File::Fields::Id,
        KGAPI2::Drive::File::Fields::Title,
    });
    connect(fileFetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTeamdriveFetchJobFinished);
}

void MainWindow::slotTeamdriveFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::FileFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::FileFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->teamdriveListButton->setEnabled(true);
        return;
    }

    /* Get all items we have received from Google */
    const KGAPI2::ObjectsList objects = fetchJob->items();

    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::Drive::FilePtr file = object.dynamicCast<KGAPI2::Drive::File>();

        /* Convert the teamdrive to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->teamdrivePreviewList);
        item->setText(file->title());
        item->setData(Qt::UserRole, file->id());

        m_ui->teamdrivePreviewList->addItem(item);
    }
}

void MainWindow::teamdriveItemSelected()
{
    bool hasSelection = (m_ui->teamdrivePreviewList->selectedItems().count() != 0);
    if (!hasSelection) {
        return;
    }

    const QString id = m_ui->teamdrivePreviewList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::FileFetchJob *fileFetchJob = new KGAPI2::Drive::FileFetchJob(id, m_account, nullptr);
    fileFetchJob->setFields({
        KGAPI2::Drive::File::Fields::Title,
        KGAPI2::Drive::File::Fields::FileSize,
    });
    connect(fileFetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotTeamdriveItemFetchJobFinished);
}

void MainWindow::slotTeamdriveItemFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::FileFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::FileFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    KGAPI2::ObjectsList objects = fetchJob->items();
    if (objects.size() != 1) {
        return;
    }

    KGAPI2::ObjectPtr object = objects.at(0);
    const KGAPI2::Drive::FilePtr file = object.dynamicCast<KGAPI2::Drive::File>();
    QStringList msgBuilder;
    msgBuilder << file->title();
    msgBuilder << QString::number(file->fileSize()) + QStringLiteral(" bytes");
    QString msg = msgBuilder.join(QLatin1String(", "));
    m_ui->statusbar->showMessage(msg);
}
