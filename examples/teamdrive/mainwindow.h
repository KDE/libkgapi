/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"

#include "core/types.h"

namespace KGAPI2 {
  class Job;
}

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
     * Creates the Team Drive with name in newTeamdriveEdit
     */
    void createTeamdrive();

    /**
     * Rename the selected Team Drive with name in renameTeamdriveEdit
     */
    void renameSelectedTeamdrive();

    /**
     * Retrieves list of all teamdrive from user's Google teamdrive
     * addressbook
     */
    void fetchTeamdriveList();

    /**
     * Deletes the selected Team Drive
     */
    void deleteSelectedTeamdrive();

    /**
     * A specific team drive in teamdrive list has been selected. Sends a request
     * to Google to retrieve the team drive file list.
     */
    void teamdriveSelected();

    /**
     * A specific item in the teamdrive list has been selected. Sends a request
     * to Google to retrieve full details about the specific file
     */
    void teamdriveItemSelected();

  private:
    Ui::MainWindow ui;

    KGAPI2::AccountPtr m_account;

};

