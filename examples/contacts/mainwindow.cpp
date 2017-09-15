/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

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
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <contacts/contact.h>
#include <contacts/contactfetchjob.h>
#include <authjob.h>
#include <account.h>

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialice GUI */
    m_ui->setupUi(this);
    m_ui->errorLabel->setVisible(false);
    connect(m_ui->authButton, &QAbstractButton::clicked,
            this, &MainWindow::authenticate);
    connect(m_ui->contactListButton, &QAbstractButton::clicked,
            this, &MainWindow::fetchContactList);
    connect(m_ui->contactList, &QListWidget::itemSelectionChanged,
            this, &MainWindow::contactSelected);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::authenticate()
{
    KGAPI2::AccountPtr account(new KGAPI2::Account);
    account->setScopes( QList<QUrl>() << KGAPI2::Account::contactsScopeUrl() );

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
    m_ui->contactListButton->setEnabled(true);
    m_ui->authButton->setEnabled(false);
}

void MainWindow::fetchContactList()
{
    if (m_account.isNull()) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        m_ui->errorLabel->setVisible(true);
        m_ui->authButton->setVisible(true);
        return;
    }

    KGAPI2::ContactFetchJob *fetchJob = new KGAPI2::ContactFetchJob(m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotFetchJobFinished);

    m_ui->contactListButton->setEnabled(false);
}

void MainWindow::slotFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::ContactFetchJob *fetchJob = qobject_cast<KGAPI2::ContactFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->contactListButton->setEnabled(true);
        return;
    }

    /* Get all items the job has retrieved */
    const KGAPI2::ObjectsList objects = fetchJob->items();

    for (const KGAPI2::ObjectPtr &object : objects) {
        const KGAPI2::ContactPtr contact = object.dynamicCast<KGAPI2::Contact>();

        /* Convert the contact to QListWidget item */
        QListWidgetItem *item = new QListWidgetItem(m_ui->contactList);
        item->setText(contact->name());
        item->setData(Qt::UserRole, contact->uid());

        m_ui->contactList->addItem(item);
    }

    m_ui->contactListButton->setEnabled(true);
}

void MainWindow::contactSelected()
{
    if (m_ui->contactList->selectedItems().count() == 0) {
        m_ui->contactInfo->clear();
        return;
    }

    const QString id = m_ui->contactList->selectedItems().at(0)->data(Qt::UserRole).toString();

    KGAPI2::ContactFetchJob *fetchJob = new KGAPI2::ContactFetchJob(id, m_account);
    connect(fetchJob, &KGAPI2::Job::finished,
            this, &MainWindow::slotContactFetchJobFinished);
}

void MainWindow::slotContactFetchJobFinished(KGAPI2::Job *job)
{
    KGAPI2::ContactFetchJob *fetchJob = qobject_cast<KGAPI2::ContactFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();


    if (fetchJob->error() != KGAPI2::NoError) {
        m_ui->errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
        m_ui->errorLabel->setVisible(true);
        m_ui->contactListButton->setEnabled(true);
        return;
    }

    /* Get all items we have received from Google (should be just one) */
    KGAPI2::ObjectsList objects = fetchJob->items();
    if (objects.count() != 1) {
        m_ui->errorLabel->setText(QStringLiteral("Error: Server sent unexpected amount of contacts"));
        m_ui->errorLabel->setVisible(true);
        return;
    }

    KGAPI2::ContactPtr contact = objects.first().dynamicCast<KGAPI2::Contact>();

    QString text = QStringLiteral("Name: %1").arg(contact->name());

    if (!contact->phoneNumbers().isEmpty()) {
        text += QLatin1Char('\n') % QStringLiteral("Phone: %1").arg(contact->phoneNumbers().at(0).number());
    }

    if (!contact->emails().isEmpty()) {
        text += QLatin1Char('\n') % QStringLiteral("Email: %1").arg(contact->emails().at(0));
    }

    m_ui->contactInfo->setText(text);
}
