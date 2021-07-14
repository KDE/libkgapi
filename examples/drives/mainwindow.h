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
     * Creates the Drive with name in newDrivesEdit
     */
    void createDrives();

    /**
     * Drive was created.
     */
    void slotDrivesCreateJobFinished(KGAPI2::Job *job);

    /**
     * Rename the selected Drive with name in renameDrivesEdit
     */
    void renameSelectedDrives();

    /**
     * Drive was modified.
     */
    void slotDrivesModifyJobFinished(KGAPI2::Job *job);

    /**
     * Hide the selected Drive
     */
    void hideSelectedDrives();

    /**
     * Unhide the selected Drive
     */
    void unhideSelectedDrives();

    /**
     * Drive was modified.
     */
    void slotDrivesHideJobFinished(KGAPI2::Job *job);

    /**
     * All Drives were fetched.
     */
    void slotFetchJobFinished(KGAPI2::Job *job);

    /**
     * Drive listing was fetched.
     */
    void slotDrivesFetchJobFinished(KGAPI2::Job *job);

    /**
     * Retrieves list of all drives from user's Google drives
     */
    void fetchDrivesList();

    /**
     * Deletes the selected Drive
     */
    void deleteSelectedDrives();

    /**
     * Drive was deleted.
     */
    void slotDrivesDeleteJobFinished(KGAPI2::Job *job);

    /**
     * A specific team drive in drives list has been selected. Sends a request
     * to Google to retrieve the team drive file list.
     */
    void drivesSelected();

    /**
     * A specific item in the drives list has been selected. Sends a request
     * to Google to retrieve full details about the specific file
     */
    void drivesItemSelected();

    /**
     * Drive item detail was fetched.
     */
    void slotDrivesItemFetchJobFinished(KGAPI2::Job *job);

  private:
    Ui::MainWindow *m_ui;

    KGAPI2::AccountPtr m_account;

};

