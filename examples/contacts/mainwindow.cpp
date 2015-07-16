/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <contacts/contact.h>
#include <contacts/contactfetchjob.h>
#include <authjob.h>
#include <account.h>

#include <QListWidgetItem>
#include <QStringBuilder>
#include <QDebug>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialice GUI */
    m_ui->setupUi(this);
    m_ui->errorLabel->setVisible(false);
    connect(m_ui->authButton, SIGNAL(clicked(bool)),
            this, SLOT(authenticate()));
    connect(m_ui->contactListButton, SIGNAL(clicked(bool)),
            this, SLOT(fetchContactList()));
    connect(m_ui->contactList, SIGNAL(itemSelectionChanged()),
            this, SLOT(contactSelected()));
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
        QLatin1String("554041944266.apps.googleusercontent.com"),
        QLatin1String("mdT1DjzohxN3npUUzkENT0gO"));
    connect(authJob, SIGNAL(finished(KGAPI2::Job*)),
             this, SLOT(slotAuthJobFinished(KGAPI2::Job*)));
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
    connect(fetchJob, SIGNAL(finished(KGAPI2::Job*)),
            this, SLOT(slotFetchJobFinished(KGAPI2::Job*)));

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

    Q_FOREACH (const KGAPI2::ObjectPtr &object, objects) {
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
    connect(fetchJob, SIGNAL(finished(KGAPI2::Job*)),
            this, SLOT(slotContactFetchJobFinished(KGAPI2::Job*)));
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
        text += QLatin1Char('\n') % QStringLiteral("Phone: %1").arg(contact->phoneNumbers().first().number());
    }

    if (!contact->emails().isEmpty()) {
        text += QLatin1Char('\n') % QStringLiteral("Email: %1").arg(contact->emails().first());
    }

    m_ui->contactInfo->setText(text);
}
