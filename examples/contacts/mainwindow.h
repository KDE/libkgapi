/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "ui_mainwindow.h"
#include <QMainWindow>

#include "core/types.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:
    /**
     * Retrieves tokens from Google that we will use to authenticate
     * further requests
     */
    void authenticate();

    /**
     * Retrieves list of all contacts from user's Google Contacts
     * addressbook
     */
    void fetchContactList();

    /**
     * A specific contact in contact list has been selected. Sends a request
     * to Google to retrieve full details about the specific contact
     */
    void contactSelected();

private:
    Ui::MainWindow ui;

    KGAPI2::AccountPtr m_account;
};
