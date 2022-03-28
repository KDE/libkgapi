/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "ui_mainwindow.h"
#include <QMainWindow>

#include "core/types.h"

namespace KGAPI2
{
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
     * Creates the Drive with name in newDrivesEdit
     */
    void createDrives();

    /**
     * Rename the selected Drive with name in renameDrivesEdit
     */
    void renameSelectedDrives();

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
     * Retrieves list of all drives from user's Google drives
     */
    void fetchDrivesList();

    /**
     * Deletes the selected Drive
     */
    void deleteSelectedDrives();

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

private:
    Ui::MainWindow ui;

    KGAPI2::AccountPtr m_account;
};
