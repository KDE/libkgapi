/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "contacts/contact.h"
#include "contacts/contactfetchjob.h"
#include "core/account.h"
#include "core/authjob.h"

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Initialize GUI */
    ui.setupUi(this);
    ui.errorLabel->setVisible(false);
    connect(ui.authButton, &QAbstractButton::clicked, this, &MainWindow::authenticate);
    connect(ui.contactListButton, &QAbstractButton::clicked, this, &MainWindow::fetchContactList);
    connect(ui.contactList, &QListWidget::itemSelectionChanged, this, &MainWindow::contactSelected);
}

void MainWindow::authenticate()
{
    auto account = KGAPI2::AccountPtr::create();
    account->setScopes({KGAPI2::Account::contactsScopeUrl()});

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
        ui.contactListButton->setEnabled(true);
        ui.authButton->setEnabled(false);
    });
}

void MainWindow::fetchContactList()
{
    if (m_account.isNull()) {
        ui.errorLabel->setText(QStringLiteral("Error: Please authenticate first"));
        ui.errorLabel->setVisible(true);
        ui.authButton->setVisible(true);
        return;
    }

    auto *fetchJob = new KGAPI2::ContactFetchJob(m_account, this);
    connect(fetchJob, &KGAPI2::Job::finished, this, [this, fetchJob]() {
        fetchJob->deleteLater();

        if (fetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.contactListButton->setEnabled(true);
            return;
        }

        /* Get all items the job has retrieved */
        const auto objects = fetchJob->items();
        for (const auto &object : objects) {
            const auto contact = object.dynamicCast<KGAPI2::Contact>();

            /* Convert the contact to QListWidget item */
            auto *item = new QListWidgetItem(ui.contactList);
            item->setText(contact->name());
            item->setData(Qt::UserRole, contact->uid());
            ui.contactList->addItem(item);
        }

        ui.contactListButton->setEnabled(true);
    });

    ui.contactListButton->setEnabled(false);
}

void MainWindow::contactSelected()
{
    if (ui.contactList->selectedItems().count() == 0) {
        ui.contactInfo->clear();
        return;
    }

    const auto id = ui.contactList->selectedItems().at(0)->data(Qt::UserRole).toString();

    auto *fetchJob = new KGAPI2::ContactFetchJob(id, m_account);
    connect(fetchJob, &KGAPI2::Job::finished, this, [this, fetchJob]() {
        fetchJob->deleteLater();

        if (fetchJob->error() != KGAPI2::NoError) {
            ui.errorLabel->setText(QStringLiteral("Error: %1").arg(fetchJob->errorString()));
            ui.errorLabel->setVisible(true);
            ui.contactListButton->setEnabled(true);
            return;
        }

        /* Get all items we have received from Google (should be just one) */
        const auto objects = fetchJob->items();
        if (objects.count() != 1) {
            ui.errorLabel->setText(QStringLiteral("Error: Server sent unexpected amount of contacts"));
            ui.errorLabel->setVisible(true);
            return;
        }

        const auto contact = objects.first().dynamicCast<KGAPI2::Contact>();
        QString text = QStringLiteral("Name: %1").arg(contact->name());

        if (!contact->phoneNumbers().isEmpty()) {
            text += QLatin1Char('\n') % QStringLiteral("Phone: %1").arg(contact->phoneNumbers().at(0).number());
        }

        if (!contact->emails().isEmpty()) {
            text += QLatin1Char('\n') % QStringLiteral("Email: %1").arg(contact->emails().at(0));
        }

        ui.contactInfo->setText(text);
    });
}
