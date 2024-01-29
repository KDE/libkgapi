/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mainwindow.h"

#include "core/account.h"
#include "core/authjob.h"
#include "drive/file.h"
#include "drive/filefetchjob.h"
#include "drive/filesearchquery.h"
#include "drive/teamdrive.h"
#include "drive/teamdrivecreatejob.h"
#include "drive/teamdrivedeletejob.h"
#include "drive/teamdrivefetchjob.h"
#include "drive/teamdrivemodifyjob.h"
#include "drive/teamdrivesearchquery.h"

#include <QListWidgetItem>
#include <QUuid>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Initialize GUI */
    ui.setupUi(this);
    ui.errorLabel->setVisible(false);
    connect(ui.authButton, &QAbstractButton::clicked, this, &MainWindow::authenticate);
    connect(ui.newTeamdriveButton, &QAbstractButton::clicked, this, &MainWindow::createTeamdrive);
    connect(ui.teamdriveListButton, &QAbstractButton::clicked, this, &MainWindow::fetchTeamdriveList);
    connect(ui.teamdriveSelectedDeleteButton, &QAbstractButton::clicked, this, &MainWindow::deleteSelectedTeamdrive);
    connect(ui.renameTeamdriveButton, &QAbstractButton::clicked, this, &MainWindow::renameSelectedTeamdrive);
    connect(ui.teamdriveList, &QListWidget::itemSelectionChanged, this, &MainWindow::teamdriveSelected);
    connect(ui.teamdrivePreviewList, &QListWidget::itemSelectionChanged, this, &MainWindow::teamdriveItemSelected);
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
        ui.teamdriveListButton->setEnabled(true);
        ui.newTeamdriveEdit->setEnabled(true);
        ui.newTeamdriveButton->setEnabled(true);
        ui.authButton->setEnabled(false);
    });
}

void MainWindow::createTeamdrive()
{
    const auto teamdriveName = ui.newTeamdriveEdit->text();
    if (teamdriveName.isEmpty()) {
        return;
    }
    const auto requestId = QUuid::createUuid().toString();

    auto teamdrive = KGAPI2::Drive::TeamdrivePtr::create();
    teamdrive->setName(teamdriveName);

    auto *createJob = new KGAPI2::Drive::TeamdriveCreateJob(requestId, teamdrive, m_account, this);
    connect(createJob, &KGAPI2::Job::finished, this, [this, createJob]() {
        createJob->deleteLater();

        if (createJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(createJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.teamdriveListButton->setEnabled(true);
            return;
        }

        ui.newTeamdriveEdit->clear();
        fetchTeamdriveList();
    });
}

void MainWindow::renameSelectedTeamdrive()
{
    const auto teamdriveName = ui.renameTeamdriveEdit->text();
    if (teamdriveName.isEmpty()) {
        return;
    }
    const auto teamdriveId = ui.teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto teamdrive = KGAPI2::Drive::TeamdrivePtr::create();
    teamdrive->setId(teamdriveId);
    teamdrive->setName(teamdriveName);

    auto *modifyJob = new KGAPI2::Drive::TeamdriveModifyJob(teamdrive, m_account, this);
    connect(modifyJob, &KGAPI2::Job::finished, this, [this, modifyJob]() {
        modifyJob->deleteLater();

        if (modifyJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(modifyJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.teamdriveListButton->setEnabled(true);
            return;
        }

        fetchTeamdriveList();
    });
}

void MainWindow::fetchTeamdriveList()
{
    if (m_account.isNull()) {
        ui.errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        ui.errorLabel->setVisible(true);
        ui.authButton->setVisible(true);
        return;
    }

    auto *fetchJob = new KGAPI2::Drive::TeamdriveFetchJob(m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished, this, [this, fetchJob]() {
        fetchJob->deleteLater();

        if (fetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.teamdriveListButton->setEnabled(true);
            return;
        }

        /* Get all items the job has retrieved */
        const auto objects = fetchJob->items();
        ui.teamdriveList->clear();
        for (const auto &object : objects) {
            const auto teamdrive = object.dynamicCast<KGAPI2::Drive::Teamdrive>();

            /* Convert the teamdrive to QListWidget item */
            auto *item = new QListWidgetItem(ui.teamdriveList);
            item->setText(teamdrive->name());
            item->setData(Qt::UserRole, teamdrive->id());

            ui.teamdriveList->addItem(item);
        }

        ui.teamdriveListButton->setEnabled(true);
    });

    ui.teamdriveListButton->setEnabled(false);
}

void MainWindow::deleteSelectedTeamdrive()
{
    const auto teamdrive_id = ui.teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto *deleteJob = new KGAPI2::Drive::TeamdriveDeleteJob(teamdrive_id, m_account, this);
    connect(deleteJob, &KGAPI2::Job::finished, this, [this, deleteJob]() {
        deleteJob->deleteLater();

        if (deleteJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(deleteJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.teamdriveListButton->setEnabled(true);
            return;
        }

        fetchTeamdriveList();
    });
}

void MainWindow::teamdriveSelected()
{
    const bool hasSelection = (ui.teamdriveList->selectedItems().count() != 0);

    ui.teamdriveSelectedDeleteButton->setEnabled(hasSelection);
    ui.renameTeamdriveButton->setEnabled(hasSelection);
    ui.renameTeamdriveEdit->setEnabled(hasSelection);

    ui.teamdrivePreviewList->clear();

    if (!hasSelection) {
        ui.renameTeamdriveEdit->clear();
        return;
    }

    const auto id = ui.teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();
    const auto name = ui.teamdriveList->selectedItems().at(0)->data(Qt::DisplayRole).toString();

    ui.renameTeamdriveEdit->setText(name);

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
            ui.teamdriveListButton->setEnabled(true);
            return;
        }

        /* Get all items we have received from Google */
        const auto objects = fileFetchJob->items();

        for (const auto &object : objects) {
            const auto file = object.dynamicCast<KGAPI2::Drive::File>();

            /* Convert the teamdrive to QListWidget item */
            auto *item = new QListWidgetItem(ui.teamdrivePreviewList);
            item->setText(file->title());
            item->setData(Qt::UserRole, file->id());

            ui.teamdrivePreviewList->addItem(item);
        }
    });
}

void MainWindow::teamdriveItemSelected()
{
    const bool hasSelection = (ui.teamdrivePreviewList->selectedItems().count() != 0);
    if (!hasSelection) {
        return;
    }

    const auto id = ui.teamdrivePreviewList->selectedItems().at(0)->data(Qt::UserRole).toString();

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
        const auto msg = msgBuilder.join(QLatin1StringView(", "));
        ui.statusbar->showMessage(msg);
    });
}

#include "moc_mainwindow.cpp"
