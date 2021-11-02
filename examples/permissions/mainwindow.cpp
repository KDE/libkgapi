/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drive/permissionfetchjob.h"
#include "core/authjob.h"
#include "core/account.h"

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent)
{
    /* Initialize GUI */
    ui.setupUi(this);
    ui.errorLabel->setVisible(false);
    connect(ui.authButton, &QAbstractButton::clicked, this, &MainWindow::authenticate);
    connect(ui.getPermissionsButton, &QAbstractButton::clicked, this, &MainWindow::slotFetchPermissions);
    connect(ui.permissionsList, &QListWidget::itemSelectionChanged,this, &MainWindow::permissionSelected);
}

void MainWindow::authenticate()
{
    auto account = KGAPI2::AccountPtr::create();
    account->setScopes({KGAPI2::Account::driveScopeUrl()});

    /* Create AuthJob to retrieve OAuth tokens for the account */
    auto *authJob = new KGAPI2::AuthJob(
        account,
        QStringLiteral("554041944266.apps.googleusercontent.com"),
        QStringLiteral("mdT1DjzohxN3npUUzkENT0gO"));
    connect(authJob, &KGAPI2::Job::finished, this,
            [this, authJob]() {
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
                ui.fileIdEdit->setEnabled(true);
                ui.getPermissionsButton->setEnabled(true);
            });
}

void MainWindow::slotFetchPermissions()
{
    if (m_account.isNull()) {
        ui.errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        ui.errorLabel->setVisible(true);
        ui.authButton->setVisible(true);
        return;
    }

    const auto fileId = ui.fileIdEdit->text();
    auto *fetchJob = new KGAPI2::Drive::PermissionFetchJob(fileId, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished, this,
            [this, fetchJob]() {
                fetchJob->deleteLater();

                if (fetchJob->error() != KGAPI2::NoError) {
                    ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
                    ui.errorLabel->setVisible(true);
                    return;
                }

                /* Get all items the job has retrieved */
                const auto objects = fetchJob->items();
                ui.permissionsList->clear();
                for (const auto &object : objects) {
                    const auto permission = object.dynamicCast<KGAPI2::Drive::Permission>();

                    /* Convert the permission to QListWidget item */
                    auto *item = new QListWidgetItem(ui.permissionsList);
                    QString displayText;
                    if (permission->name().isEmpty()) {
                        displayText = permission->emailAddress();
                    }
                    else {
                        displayText = QStringLiteral("%0 (%1)").arg(permission->name()).arg(permission->emailAddress());
                    }
                    item->setText(displayText);
                    item->setData(Qt::UserRole, permission->id());

                    ui.permissionsList->addItem(item);
                }
            });
}

void MainWindow::permissionSelected()
{
    const bool hasSelection = (ui.permissionsList->selectedItems().count() != 0);

    ui.permissionDetailsList->clear();

    if (!hasSelection) {
        return;
    }

    const auto permissionId = ui.permissionsList->selectedItems().at(0)->data(Qt::UserRole).toString();
    const auto fileId = ui.fileIdEdit->text();
    auto *fetchJob = new KGAPI2::Drive::PermissionFetchJob(fileId, permissionId, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished, this,
            [this, fetchJob]() {
                fetchJob->deleteLater();

                if (fetchJob->error() != KGAPI2::NoError) {
                    ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
                    ui.errorLabel->setVisible(true);
                    return;
                }

                /* Get all items the job has retrieved */
                const auto objects = fetchJob->items();
                for (const auto &object : objects) {
                    const auto permission = object.dynamicCast<KGAPI2::Drive::Permission>();

                    ui.permissionDetailsList->addItem(QStringLiteral("Id: %0").arg(permission->id()));
                    ui.permissionDetailsList->addItem(QStringLiteral("Name: %0").arg(permission->name()));
                    ui.permissionDetailsList->addItem(QStringLiteral("Email: %0").arg(permission->emailAddress()));
                    ui.permissionDetailsList->addItem(QStringLiteral("Type: %0").arg(typeToName(permission->type())));
                    ui.permissionDetailsList->addItem(QStringLiteral("Role: %0").arg(roleToName(permission->role())));
                    ui.permissionDetailsList->addItem(QStringLiteral("Permission details count: %0").arg(permission->permissionDetails().size()));
                    for (int i = 0; i < permission->permissionDetails().size(); ++i) {
                        const auto permissionDetails = permission->permissionDetails().at(i);
                        ui.permissionDetailsList->addItem(QStringLiteral("  [%0] permissionType: %1").arg(i).arg(permissionTypeToName(permissionDetails->permissionType())));
                        ui.permissionDetailsList->addItem(QStringLiteral("  [%0] role: %1").arg(i).arg(roleToName(permissionDetails->role())));
                        ui.permissionDetailsList->addItem(QStringLiteral("  [%0] inherited: %1").arg(i).arg(permissionDetails->inherited() ? QStringLiteral("true") : QStringLiteral("false")));
                        if (permissionDetails->inherited()) {
                            ui.permissionDetailsList->addItem(QStringLiteral("  [%0] inherited from: %1").arg(i).arg(permissionDetails->inheritedFrom()));
                        }
                    }
                }
            });
}

QString MainWindow::roleToName(KGAPI2::Drive::Permission::Role role)
{
    switch (role) {
    case KGAPI2::Drive::Permission::OwnerRole:
        return QStringLiteral("owner");
    case KGAPI2::Drive::Permission::ReaderRole:
        return QStringLiteral("reader");
    case KGAPI2::Drive::Permission::WriterRole:
        return QStringLiteral("writer");
    case KGAPI2::Drive::Permission::CommenterRole:
        return QStringLiteral("commenter");
    case KGAPI2::Drive::Permission::OrganizerRole:
        return QStringLiteral("organizerRole");
    case KGAPI2::Drive::Permission::FileOrganizerRole:
        return QStringLiteral("fileOrganizerRole");
    default:
        return QString();
    }
}

QString MainWindow::typeToName(KGAPI2::Drive::Permission::Type type)
{
    switch (type) {
    case KGAPI2::Drive::Permission::TypeUser:
        return QStringLiteral("user");
    case KGAPI2::Drive::Permission::TypeGroup:
        return QStringLiteral("group");
    case KGAPI2::Drive::Permission::TypeDomain:
        return QStringLiteral("domain");
    case KGAPI2::Drive::Permission::TypeAnyone:
        return QStringLiteral("anyone");
    default:
        return QString();
    }
}

QString MainWindow::permissionTypeToName(KGAPI2::Drive::Permission::PermissionDetails::PermissionType permissionType)
{
    switch (permissionType) {
    case KGAPI2::Drive::Permission::PermissionDetails::TypeFile:
        return QStringLiteral("file");
    case KGAPI2::Drive::Permission::PermissionDetails::TypeMember:
        return QStringLiteral("member");
    default:
        return QString();
    }
}

