/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <drive/permissionfetchjob.h>
#include <authjob.h>
#include <account.h>

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialize GUI */
    m_ui->setupUi(this);
    m_ui->errorLabel->setVisible(false);
    connect(m_ui->authButton, &QAbstractButton::clicked,
            this, &MainWindow::authenticate);
    connect(m_ui->getPermissionsButton, &QAbstractButton::clicked,
            this, &MainWindow::slotFetchPermissions);
    connect(m_ui->permissionsList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::permissionSelected);

    // m_account = KGAPI2::AccountPtr::create();
    // m_account->setAccessToken(QStringLiteral(""));
    // m_ui->authStatusLabel->setText(QStringLiteral("Authenticated"));
    // m_ui->fileIdEdit->setEnabled(true);
    // m_ui->fileIdEdit->setText(QStringLiteral("0ABDsfTyhwbTyUk9PVA"));
    // m_ui->getPermissionsButton->setEnabled(true);
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
    // qDebug() << "Got access token" << m_account->accessToken();

    m_ui->authStatusLabel->setText(QStringLiteral("Authenticated"));
    m_ui->fileIdEdit->setEnabled(true);
    m_ui->getPermissionsButton->setEnabled(true);
}

void MainWindow::slotFetchPermissions()
{
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    const QString fileId = m_ui->fileIdEdit->text();
    KGAPI2::Drive::PermissionFetchJob *fetchJob = new KGAPI2::Drive::PermissionFetchJob(fileId, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotPermissionsFetchJobFinished);
}

void MainWindow::slotPermissionsFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::PermissionFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::PermissionFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    /* Get all items the job has retrieved */
    const KGAPI2::ObjectsList objects = fetchJob->items();
    m_ui->permissionsList->clear();
    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::Drive::PermissionPtr permission = object.dynamicCast<KGAPI2::Drive::Permission>();

        /* Convert the permission to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->permissionsList);
        QString displayText;
        if (permission->name().isEmpty()) {
            displayText = permission->emailAddress();
        }
        else {
            displayText = QStringLiteral("%0 (%1)").arg(permission->name()).arg(permission->emailAddress());
        }
        item->setText(displayText);
        item->setData(Qt::UserRole, permission->id());

        m_ui->permissionsList->addItem(item);
    }
}

void MainWindow::permissionSelected()
{
    bool hasSelection = (m_ui->permissionsList->selectedItems().count() != 0);

    m_ui->permissionDetailsList->clear();

    if (!hasSelection) {
        return;
    }

    const QString permissionId = m_ui->permissionsList->selectedItems().at(0)->data(Qt::UserRole).toString();
    const QString fileId = m_ui->fileIdEdit->text();
    KGAPI2::Drive::PermissionFetchJob *fetchJob = new KGAPI2::Drive::PermissionFetchJob(fileId, permissionId, m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotPermissionFetchJobFinished);
}

void MainWindow::slotPermissionFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::Drive::PermissionFetchJob *fetchJob = qobject_cast<KGAPI2::Drive::PermissionFetchJob*>(job);
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
        const KGAPI2::Drive::PermissionPtr permission = object.dynamicCast<KGAPI2::Drive::Permission>();

        m_ui->permissionDetailsList->addItem(QStringLiteral("Id: %0").arg(permission->id()));
        m_ui->permissionDetailsList->addItem(QStringLiteral("Name: %0").arg(permission->name()));
        m_ui->permissionDetailsList->addItem(QStringLiteral("Email: %0").arg(permission->emailAddress()));
        m_ui->permissionDetailsList->addItem(QStringLiteral("Type: %0").arg(typeToName(permission->type())));
        m_ui->permissionDetailsList->addItem(QStringLiteral("Role: %0").arg(roleToName(permission->role())));
        m_ui->permissionDetailsList->addItem(QStringLiteral("Permission details count: %0").arg(permission->permissionDetails().size()));
        for (int i = 0; i < permission->permissionDetails().size(); ++i) {
            KGAPI2::Drive::Permission::PermissionDetailsPtr permissionDetails = permission->permissionDetails().at(i);
            m_ui->permissionDetailsList->addItem(QStringLiteral("  [%0] permissionType: %1").arg(i).arg(permissionTypeToName(permissionDetails->permissionType())));
            m_ui->permissionDetailsList->addItem(QStringLiteral("  [%0] role: %1").arg(i).arg(roleToName(permissionDetails->role())));
            m_ui->permissionDetailsList->addItem(QStringLiteral("  [%0] inherited: %1").arg(i).arg(permissionDetails->inherited() ? QStringLiteral("true") : QStringLiteral("false")));
            if (permissionDetails->inherited()) {
                m_ui->permissionDetailsList->addItem(QStringLiteral("  [%0] inherited from: %1").arg(i).arg(permissionDetails->inheritedFrom()));
            }
        }
    }
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
