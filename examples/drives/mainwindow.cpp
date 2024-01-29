/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mainwindow.h"

#include "core/account.h"
#include "core/authjob.h"
#include "drive/drives.h"
#include "drive/drivescreatejob.h"
#include "drive/drivesdeletejob.h"
#include "drive/drivesfetchjob.h"
#include "drive/driveshidejob.h"
#include "drive/drivesmodifyjob.h"
#include "drive/drivessearchquery.h"
#include "drive/file.h"
#include "drive/filefetchjob.h"
#include "drive/filesearchquery.h"

#include <QListWidgetItem>
#include <QUuid>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Initialize GUI */
    ui.setupUi(this);
    ui.errorLabel->setVisible(false);
    connect(ui.authButton, &QAbstractButton::clicked, this, &MainWindow::authenticate);
    connect(ui.newDrivesButton, &QAbstractButton::clicked, this, &MainWindow::createDrives);
    connect(ui.drivesListButton, &QAbstractButton::clicked, this, &MainWindow::fetchDrivesList);
    connect(ui.drivesSelectedDeleteButton, &QAbstractButton::clicked, this, &MainWindow::deleteSelectedDrives);
    connect(ui.renameDrivesButton, &QAbstractButton::clicked, this, &MainWindow::renameSelectedDrives);
    connect(ui.hideDrivesButton, &QAbstractButton::clicked, this, &MainWindow::hideSelectedDrives);
    connect(ui.unhideDrivesButton, &QAbstractButton::clicked, this, &MainWindow::unhideSelectedDrives);
    connect(ui.drivesList, &QListWidget::itemSelectionChanged, this, &MainWindow::drivesSelected);
    connect(ui.drivesPreviewList, &QListWidget::itemSelectionChanged, this, &MainWindow::drivesItemSelected);
}

void MainWindow::authenticate()
{
    auto account = KGAPI2::AccountPtr::create();
    account->setScopes({KGAPI2::Account::driveScopeUrl()});

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
        ui.drivesListButton->setEnabled(true);
        ui.newDrivesEdit->setEnabled(true);
        ui.newDrivesButton->setEnabled(true);
        ui.authButton->setEnabled(false);
    });
}

void MainWindow::createDrives()
{
    const auto drivesName = ui.newDrivesEdit->text();
    if (drivesName.isEmpty()) {
        return;
    }
    const auto requestId = QUuid::createUuid().toString();

    auto drives = KGAPI2::Drive::DrivesPtr::create();
    drives->setName(drivesName);

    auto *createJob = new KGAPI2::Drive::DrivesCreateJob(requestId, drives, m_account, this);
    connect(createJob, &KGAPI2::Job::finished, this, [this, createJob]() {
        createJob->deleteLater();

        if (createJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(createJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.drivesListButton->setEnabled(true);
            return;
        }

        ui.newDrivesEdit->clear();
        fetchDrivesList();
    });
}

void MainWindow::renameSelectedDrives()
{
    const auto drivesName = ui.renameDrivesEdit->text();
    if (drivesName.isEmpty()) {
        return;
    }
    const auto drivesId = ui.drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto drives = KGAPI2::Drive::DrivesPtr::create();
    drives->setId(drivesId);
    drives->setName(drivesName);

    auto *modifyJob = new KGAPI2::Drive::DrivesModifyJob(drives, m_account, this);
    connect(modifyJob, &KGAPI2::Job::finished, this, [this, modifyJob]() {
        modifyJob->deleteLater();

        if (modifyJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(modifyJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.drivesListButton->setEnabled(true);
            return;
        }

        fetchDrivesList();
    });
}

void MainWindow::hideSelectedDrives()
{
    const auto drivesId = ui.drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto drive = KGAPI2::Drive::DrivesPtr::create();
    drive->setId(drivesId);

    auto *hideJob = new KGAPI2::Drive::DrivesHideJob(drive, true, m_account, this);
    connect(hideJob, &KGAPI2::Job::finished, this, &MainWindow::slotDrivesHideJobFinished);
}

void MainWindow::unhideSelectedDrives()
{
    const auto drivesId = ui.drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto drive = KGAPI2::Drive::DrivesPtr::create();
    drive->setId(drivesId);

    auto *hideJob = new KGAPI2::Drive::DrivesHideJob(drive, false, m_account, this);
    connect(hideJob, &KGAPI2::Job::finished, this, &MainWindow::slotDrivesHideJobFinished);
}

void MainWindow::slotDrivesHideJobFinished(KGAPI2::Job *job)
{
    auto *hideJob = qobject_cast<KGAPI2::Drive::DrivesHideJob *>(job);
    Q_ASSERT(hideJob);
    hideJob->deleteLater();

    if (hideJob->error() != KGAPI2::NoError) {
        ui.errorLabel->setText(QStringLiteral("Error: %1").arg(hideJob->errorString()));
        ui.errorLabel->setVisible(true);
        ui.drivesListButton->setEnabled(true);
        return;
    }

    fetchDrivesList();
}

void MainWindow::fetchDrivesList()
{
    if (m_account.isNull()) {
        ui.errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        ui.errorLabel->setVisible(true);
        ui.authButton->setVisible(true);
        return;
    }

    const bool showHidden = ui.showHiddenCheckBox->isChecked();

    KGAPI2::Drive::DrivesSearchQuery query;
    query.addQuery(KGAPI2::Drive::DrivesSearchQuery::Hidden, KGAPI2::Drive::DrivesSearchQuery::Equals, showHidden);

    auto *fetchJob = new KGAPI2::Drive::DrivesFetchJob(query, m_account, this);
    fetchJob->setUseDomainAdminAccess(false);
    fetchJob->setFields({KGAPI2::Drive::Drives::Fields::Id, KGAPI2::Drive::Drives::Fields::Name, KGAPI2::Drive::Drives::Fields::Hidden});
    connect(fetchJob, &KGAPI2::Job::finished, this, [this, fetchJob]() {
        fetchJob->deleteLater();

        if (fetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.drivesListButton->setEnabled(true);
            return;
        }

        /* Get all items the job has retrieved */
        const auto objects = fetchJob->items();
        ui.drivesList->clear();
        for (const auto &object : objects) {
            const auto drives = object.dynamicCast<KGAPI2::Drive::Drives>();

            /* Convert the drives to QListWidget item */
            auto *item = new QListWidgetItem(ui.drivesList);
            item->setText(drives->name() + (drives->hidden() ? QStringLiteral(" (hidden)") : QStringLiteral("")));
            item->setData(Qt::UserRole, drives->id());

            ui.drivesList->addItem(item);
        }

        ui.drivesListButton->setEnabled(true);
    });

    ui.drivesListButton->setEnabled(false);
}

void MainWindow::deleteSelectedDrives()
{
    const auto drives_id = ui.drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto *deleteJob = new KGAPI2::Drive::DrivesDeleteJob(drives_id, m_account, this);
    connect(deleteJob, &KGAPI2::Job::finished, this, [this, deleteJob]() {
        deleteJob->deleteLater();

        if (deleteJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(deleteJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.drivesListButton->setEnabled(true);
            return;
        }

        fetchDrivesList();
    });
}

void MainWindow::drivesSelected()
{
    const bool hasSelection = (ui.drivesList->selectedItems().count() != 0);

    ui.drivesSelectedDeleteButton->setEnabled(hasSelection);
    ui.renameDrivesButton->setEnabled(hasSelection);
    ui.hideDrivesButton->setEnabled(hasSelection);
    ui.unhideDrivesButton->setEnabled(hasSelection);
    ui.renameDrivesEdit->setEnabled(hasSelection);

    ui.drivesPreviewList->clear();

    if (!hasSelection) {
        ui.renameDrivesEdit->clear();
        return;
    }

    const auto id = ui.drivesList->selectedItems().at(0)->data(Qt::UserRole).toString();
    const auto name = ui.drivesList->selectedItems().at(0)->data(Qt::DisplayRole).toString();

    ui.renameDrivesEdit->setText(name);

    KGAPI2::Drive::FileSearchQuery query;
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Trashed, KGAPI2::Drive::FileSearchQuery::Equals, false);
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Parents, KGAPI2::Drive::FileSearchQuery::In, id);

    auto *fileFetchJob = new KGAPI2::Drive::FileFetchJob(query, m_account, nullptr);
    fileFetchJob->setFields({
        KGAPI2::Drive::File::Fields::Id,
        KGAPI2::Drive::File::Fields::Title,
    });
    connect(fileFetchJob, &KGAPI2::Job::finished, this, [this, fileFetchJob]() {
        fileFetchJob->deleteLater();

        if (fileFetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fileFetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.drivesListButton->setEnabled(true);
            return;
        }

        /* Get all items we have received from Google */
        const auto objects = fileFetchJob->items();

        for (const auto &object : objects) {
            const auto file = object.dynamicCast<KGAPI2::Drive::File>();

            /* Convert the drives to QListWidget item */
            auto *item = new QListWidgetItem(ui.drivesPreviewList);
            item->setText(file->title());
            item->setData(Qt::UserRole, file->id());

            ui.drivesPreviewList->addItem(item);
        }
    });
}

void MainWindow::drivesItemSelected()
{
    const bool hasSelection = (ui.drivesPreviewList->selectedItems().count() != 0);
    if (!hasSelection) {
        return;
    }

    const auto id = ui.drivesPreviewList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto *fileFetchJob = new KGAPI2::Drive::FileFetchJob(id, m_account, nullptr);
    fileFetchJob->setFields({
        KGAPI2::Drive::File::Fields::Title,
        KGAPI2::Drive::File::Fields::FileSize,
    });
    connect(fileFetchJob, &KGAPI2::Job::finished, this, [this, fileFetchJob]() {
        fileFetchJob->deleteLater();

        if (fileFetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fileFetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            return;
        }

        const auto objects = fileFetchJob->items();
        if (objects.size() != 1) {
            return;
        }

        const auto object = objects.at(0);
        const auto file = object.dynamicCast<KGAPI2::Drive::File>();
        QStringList msgBuilder;
        msgBuilder << file->title();
        msgBuilder << QString::number(file->fileSize()) + QStringLiteral(" bytes");
        QString msg = msgBuilder.join(QLatin1StringView(", "));
        ui.statusbar->showMessage(msg);
    });
}

#include "moc_mainwindow.cpp"
