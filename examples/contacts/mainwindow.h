/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <types.h>

namespace Ui {
    class MainWindow;
}

namespace KGAPI2 {
  class Job;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private Q_SLOTS:
    /**
     * Retrieves tokens from Google that we will use to authenticate
     * fursther requests
     */
    void authenticate();

    /**
     * Authentication has finished
     */
    void slotAuthJobFinished(KGAPI2::Job *job);

    /**
     * All contacts were fetched.
     */
    void slotFetchJobFinished(KGAPI2::Job *job);

    /**
     * Contact details were fetched.
     */
    void slotContactFetchJobFinished(KGAPI2::Job *job);

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
    Ui::MainWindow *m_ui;

    KGAPI2::AccountPtr m_account;

};

#endif // MAINWINDOW_H
