/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#pragma once

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
     * further requests
     */
    void authenticate();

    /**
     * Authentication has finished
     */
    void slotAuthJobFinished(KGAPI2::Job *job);

    /**
     * Creates the Team Drive with name in newTeamdriveEdit
     */
    void createTeamdrive();

    /**
     * Team Drive was created.
     */
    void slotTeamdriveCreateJobFinished(KGAPI2::Job *job);

    /**
     * Rename the selected Team Drive with name in renameTeamdriveEdit
     */
    void renameSelectedTeamdrive();

    /**
     * Team Drive was modified.
     */
    void slotTeamdriveModifyJobFinished(KGAPI2::Job *job);

    /**
     * All Team Drives were fetched.
     */
    void slotFetchJobFinished(KGAPI2::Job *job);

    /**
     * Team Drive listing was fetched.
     */
    void slotTeamdriveFetchJobFinished(KGAPI2::Job *job);

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
     * Team Drive was deleted.
     */
    void slotTeamdriveDeleteJobFinished(KGAPI2::Job *job);

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

    /**
     * Team Drive item detail was fetched.
     */
    void slotTeamdriveItemFetchJobFinished(KGAPI2::Job *job);

  private:
    Ui::MainWindow *m_ui;

    KGAPI2::AccountPtr m_account;

};

