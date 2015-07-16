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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <types.h>

namespace Ui {
    class MainWindow;
}

namespace KGAPI2 {
  class Job;
};

class KJob;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

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
