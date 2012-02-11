/*
    Akonadi Google - Calendar Resource
    Copyright (C) 2011, 2012  Dan Vratil <dan@progdan.cz>

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

#include <libkgoogle/common.h>

namespace Ui {
  class SettingsDialog;
}

namespace KGoogle {

  namespace Objects {

    class ContactsGroup;

  }

  class Reply;

}

class QTreeWidgetItem;

class SettingsDialog : public KDialog
{
  Q_OBJECT
  public:
    SettingsDialog(WId windowId, QWidget *parent = 0);
    ~SettingsDialog();

  private Q_SLOTS:
    void addAccountClicked();
    void removeAccountClicked();
    void accountChanged();

    void addGroupClicked();
    void editGroupClicked();
    void removeGroupClicked();
    void reloadGroupsClicked();

    void reloadAccounts();

    void gam_groupCreated(KGoogle::Reply *reply);
    void gam_groupModified(KGoogle::Reply *reply);
    void gam_groupRemoved(KGoogle::Reply *reply);
    void gam_groupsListReceived(KGoogle::Reply *reply);

    void addGroup(KGoogle::Objects::ContactsGroup *group);
    void editGroup(KGoogle::Objects::ContactsGroup *group);

    void groupSelected(QTreeWidgetItem *item);
    void groupChecked(QTreeWidgetItem *item);

    void error(KGoogle::Error errCode, const QString &msg);
    void saveSettings();
  private:
    Ui::SettingsDialog *m_ui;
    QWidget *m_mainWidget;

    WId m_windowId;

};

#endif // SETTINGSDIALOG_H
