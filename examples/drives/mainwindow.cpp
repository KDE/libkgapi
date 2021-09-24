/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <drive/drives.h>
#include <drive/drivescreatejob.h>
#include <drive/drivesdeletejob.h>
#include <drive/drivesfetchjob.h>
#include <drive/drivesmodifyjob.h>
#include <drive/driveshidejob.h>
#include <drive/drivessearchquery.h>
#include <drive/file.h>
#include <drive/filesearchquery.h>
#include <drive/filefetchjob.h>
#include <authjob.h>
#include <account.h>

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
    connect(m_ui->newDrivesButton, &QAbstractButton::clicked,
            this, &MainWindow::createDrives);
    connect(m_ui->drivesListButton, &QAbstractButton::clicked,
            this, &MainWindow::fetchDrivesList);
    connect(m_ui->drivesSelectedDeleteButton, &QAbstractButton::clicked,
            this, &MainWindow::deleteSelectedDrives);
    connect(m_ui->renameDrivesButton, &QAbstractButton::clicked,
            this, &MainWindow::renameSelectedDrives);
    connect(m_ui->hideDrivesButton, &QAbstractButton::clicked,
            this, &MainWindow::hideSelectedDrives);
    connect(m_ui->unhideDrivesButton, &QAbstractButton::clicked,
            this, &MainWindow::unhideSelectedDrives);
    connect(m_ui->drivesList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::drivesSelected);
    connect(m_ui->drivesPreviewList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::drivesItemSelected);
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
    m_ui->drivesListButton->setEnabled(true);
    m_ui->newDrivesEdit->setEnabled(true);
    m_ui->newDrivesButton->setEnabled(true);
    m_ui->authButton->setEnabled(false);
}

void MainWindow::createDrives()
{
    QString drivesName = m_ui->newDrivesEdit->text();
    if (drivesName.isEmpty()) {
        return;
    }
    QString requestId = QUuid::createUuid().toString();

    KGAPI2::Drive::DrivesPtr drives = KGAPI2::Drive::DrivesPtr::create();
    drives->setName(drivesName);

    KGAPI2::Drive::DrivesCreateJob *createJob = new KGAPI2::Drive::DrivesCreateJob(requestId, drives, m_account, this);
    connect(createJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesCreateJobFinished);
}

void MainWindow::slotDrivesCreateJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::DrivesCreateJob *createJob = qobject_cast<KGAPI2::Drive::DrivesCreateJob*>(job);
    Q_ASSERT(createJob);
    createJob->deleteLater();

    if (createJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(createJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->drivesListButton->setEnabled(true);
        return;
    }

    m_ui->newDrivesEdit->clear();
    fetchDrivesList();
}

void MainWindow::renameSelectedDrives()
{
    QString drivesName = m_ui->renameDrivesEdit->text();
    if (drivesName.isEmpty()) {
        return;
    }
    QString drivesId = m_ui->drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::DrivesPtr drives = KGAPI2::Drive::DrivesPtr::create();
    drives->setId(drivesId);
    drives->setName(drivesName);

    KGAPI2::Drive::DrivesModifyJob *modifyJob = new KGAPI2::Drive::DrivesModifyJob(drives, m_account, this);
    connect(modifyJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesModifyJobFinished);
}

void MainWindow::slotDrivesModifyJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::DrivesModifyJob *modifyJob = qobject_cast<KGAPI2::Drive::DrivesModifyJob*>(job);
    Q_ASSERT(modifyJob);
    modifyJob->deleteLater();

    if (modifyJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(modifyJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->drivesListButton->setEnabled(true);
        return;
    }

    fetchDrivesList();
}

void MainWindow::hideSelectedDrives()
{
    QString drivesId = m_ui->drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::DrivesPtr drive = KGAPI2::Drive::DrivesPtr::create();
    drive->setId(drivesId);

    KGAPI2::Drive::DrivesHideJob *hideJob = new KGAPI2::Drive::DrivesHideJob(drive, true, m_account, this);
    connect(hideJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesHideJobFinished);
}

void MainWindow::unhideSelectedDrives()
{
    QString drivesId = m_ui->drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::DrivesPtr drive = KGAPI2::Drive::DrivesPtr::create();
    drive->setId(drivesId);

    KGAPI2::Drive::DrivesHideJob *hideJob = new KGAPI2::Drive::DrivesHideJob(drive, false, m_account, this);
    connect(hideJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesHideJobFinished);
}

void MainWindow::slotDrivesHideJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::DrivesHideJob *hideJob = qobject_cast<KGAPI2::Drive::DrivesHideJob*>(job);
    Q_ASSERT(hideJob);
    hideJob->deleteLater();

    if (hideJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(hideJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->drivesListButton->setEnabled(true);
        return;
    }

    fetchDrivesList();
}

void MainWindow::fetchDrivesList()
{
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    bool showHidden = m_ui->showHiddenCheckBox->isChecked();

    KGAPI2::Drive::DrivesSearchQuery query;
    query.addQuery(KGAPI2::Drive::DrivesSearchQuery::Hidden, KGAPI2::Drive::DrivesSearchQuery::Equals, showHidden);

    KGAPI2::Drive::DrivesFetchJob *fetchJob = new KGAPI2::Drive::DrivesFetchJob(query, m_account, this);
    fetchJob->setUseDomainAdminAccess(false);
    fetchJob->setFields({
            KGAPI2::Drive::Drives::Fields::Id,
            KGAPI2::Drive::Drives::Fields::Name,
            KGAPI2::Drive::Drives::Fields::Hidden});
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotFetchJobFinished);

    m_ui->drivesListButton->setEnabled(false);
}

void MainWindow::slotFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::DrivesFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::DrivesFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->drivesListButton->setEnabled(true);
        return;
    }

    /* Get all items the job has retrieved */
    const KGAPI2::ObjectsList objects = fetchJob->items();
    m_ui->drivesList->clear();
    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::Drive::DrivesPtr drives = object.dynamicCast<KGAPI2::Drive::Drives>();

        /* Convert the drives to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->drivesList);
        item->setText(drives->name() + (drives->hidden() ? QStringLiteral(" (hidden)") : QStringLiteral("")));
        item->setData(Qt::UserRole, drives->id());

        m_ui->drivesList->addItem(item);
    }

    m_ui->drivesListButton->setEnabled(true);
}

void MainWindow::deleteSelectedDrives() {
    const QString drives_id = m_ui->drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::DrivesDeleteJob *deleteJob = new KGAPI2::Drive::DrivesDeleteJob(drives_id, m_account, this);
    connect(deleteJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesDeleteJobFinished);
}

void MainWindow::slotDrivesDeleteJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::DrivesDeleteJob *deleteJob = qobject_cast<KGAPI2::Drive::DrivesDeleteJob*>(job);
    Q_ASSERT(deleteJob);
    deleteJob->deleteLater();

    if (deleteJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(deleteJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->drivesListButton->setEnabled(true);
        return;
    }

    fetchDrivesList();
}

void MainWindow::drivesSelected()
{
    bool hasSelection = (m_ui->drivesList->selectedItems().count() != 0);

    m_ui->drivesSelectedDeleteButton->setEnabled(hasSelection);
    m_ui->renameDrivesButton->setEnabled(hasSelection);
    m_ui->hideDrivesButton->setEnabled(hasSelection);
    m_ui->unhideDrivesButton->setEnabled(hasSelection);
    m_ui->renameDrivesEdit->setEnabled(hasSelection);

    m_ui->drivesPreviewList->clear();

    if (!hasSelection) {
        m_ui->renameDrivesEdit->clear();
        return;
    }

    const QString id = m_ui->drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();
    const QString name = m_ui->drivesList->selectedItems().at(0)->data(Qt::DisplayRole).toString();

    m_ui->renameDrivesEdit->setText(name);

    KGAPI2::Drive::FileSearchQuery query;
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Trashed, KGAPI2::Drive::FileSearchQuery::Equals, false);
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Parents, KGAPI2::Drive::FileSearchQuery::In, id);

    KGAPI2::Drive::FileFetchJob *fileFetchJob = new KGAPI2::Drive::FileFetchJob(query, m_account, nullptr);
    fileFetchJob->setFields({
        KGAPI2::Drive::File::Fields::Id,
        KGAPI2::Drive::File::Fields::Title,
    });
    connect(fileFetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesFetchJobFinished);
}

void MainWindow::slotDrivesFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::FileFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::FileFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->drivesListButton->setEnabled(true);
        return;
    }

    /* Get all items we have received from Google */
    const KGAPI2::ObjectsList objects = fetchJob->items();

    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::Drive::FilePtr file = object.dynamicCast<KGAPI2::Drive::File>();

        /* Convert the drives to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->drivesPreviewList);
        item->setText(file->title());
        item->setData(Qt::UserRole, file->id());

        m_ui->drivesPreviewList->addItem(item);
    }
}

void MainWindow::drivesItemSelected()
{
    bool hasSelection = (m_ui->drivesPreviewList->selectedItems().count() != 0);
    if (!hasSelection) {
        return;
    }

    const QString id = m_ui->drivesPreviewList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::FileFetchJob *fileFetchJob = new KGAPI2::Drive::FileFetchJob(id, m_account, nullptr);
    fileFetchJob->setFields({
        KGAPI2::Drive::File::Fields::Title,
        KGAPI2::Drive::File::Fields::FileSize,
    });
    connect(fileFetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotDrivesItemFetchJobFinished);
}

void MainWindow::slotDrivesItemFetchJobFinished(KGAPI2::Job *job)
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
