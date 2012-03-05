/*
    Akonadi Google - Calendar Resource
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <KDE/KDialog>
#include <KDE/KJob>
#include <KDE/Akonadi/ResourceBase>

namespace Ui
{
class SettingsDialog;
}

namespace KGoogle
{
class Reply;

namespace Objects
{
class Calendar;
class TaskList;
}
}

class QListWidgetItem;

using namespace KGoogle;

class SettingsDialog : public KDialog
{
    Q_OBJECT
  public:
    SettingsDialog(WId windowId, QWidget *parent = 0);
    ~SettingsDialog();

  private Q_SLOTS:
    void reloadAccounts();
    void addAccountClicked();
    void removeAccountClicked();
    void accountChanged();
    void addCalendarClicked();
    void editCalendarClicked();
    void removeCalendarClicked();
    void reloadCalendarsClicked();
    void addTaskListClicked();
    void editTaskListClicked();
    void removeTaskListClicked();
    void reloadTaskListsClicked();

    void gam_objectsListReceived(KGoogle::Reply *reply);
    void gam_objectCreated(KGoogle::Reply *reply);
    void gam_objectModified(KGoogle::Reply *reply);

    void addCalendar(KGoogle::Objects::Calendar *calendar);
    void editCalendar(KGoogle::Objects::Calendar *calendar);

    void addTaskList(KGoogle::Objects::TaskList *taskList);
    void editTaskList(KGoogle::Objects::TaskList *taskList);

    void saveSettings();

  private:
    Ui::SettingsDialog *m_ui;
    WId m_windowId;
};

#endif // SETTINGSDIALOG_H
