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
     * A specific contact in contact list has been selected. Sends a request
     * to Google to retrieve full details about the specific contact
     */
    void teamdriveSelected();

  private:
    Ui::MainWindow *m_ui;

    KGAPI2::AccountPtr m_account;

};

#endif // MAINWINDOW_H
