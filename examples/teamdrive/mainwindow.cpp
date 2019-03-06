/*
    Copyright (C) 2019  David Barchiesi <david@barchie.si>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <drive/teamdrive.h>
#include <drive/teamdrivefetchjob.h>
#include <drive/teamdrivesearchquery.h>
#include <drive/file.h>
#include <drive/filesearchquery.h>
#include <drive/filefetchjob.h>
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
    connect(m_ui->teamdriveListButton, &QAbstractButton::clicked,
            this, &MainWindow::fetchTeamdriveList);
    connect(m_ui->teamdriveList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::teamdriveSelected);
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
    m_ui->authButton->setEnabled(false);
}

void MainWindow::fetchTeamdriveList()
{
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    KGAPI2::Drive::TeamdriveSearchQuery query;
    query.addQuery(KGAPI2::Drive::TeamdriveSearchQuery::MemberCount, KGAPI2::Drive::TeamdriveSearchQuery::Less, 10);

    KGAPI2::Drive::TeamdriveFetchJob *fetchJob = new KGAPI2::Drive::TeamdriveFetchJob(query, m_account, this);
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

void MainWindow::teamdriveSelected()
{
    if (m_ui->teamdriveList->selectedItems().count() == 0) {
        m_ui->teamdrivePreview->clear();
        return;
    }

    const QString id = m_ui->teamdriveList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::Drive::FileSearchQuery query;
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Trashed, KGAPI2::Drive::FileSearchQuery::Equals, false);
    query.addQuery(KGAPI2::Drive::FileSearchQuery::Parents, KGAPI2::Drive::FileSearchQuery::In, id);

    KGAPI2::Drive::FileFetchJob *fileFetchJob = new KGAPI2::Drive::FileFetchJob(query, m_account, nullptr);
    fileFetchJob->setIncludeTeamDriveItems(true);
    fileFetchJob->setFields((KGAPI2::Drive::FileFetchJob::BasicFields & ~KGAPI2::Drive::FileFetchJob::Permissions)
                            | KGAPI2::Drive::FileFetchJob::Labels
                            | KGAPI2::Drive::FileFetchJob::ExportLinks
                            | KGAPI2::Drive::FileFetchJob::LastViewedByMeDate);
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

    /* Get all items we have received from Google (should be just one) */
    KGAPI2::ObjectsList objects = fetchJob->items();

    QString text;
    Q_FOREACH (const KGAPI2::ObjectPtr &object, objects) {
        const KGAPI2::Drive::FilePtr file = object.dynamicCast<KGAPI2::Drive::File>();
        text += QStringLiteral("%1").arg(file->title());
        text += QLatin1Char('\n');
    }

    m_ui->teamdrivePreview->setText(text);
}
