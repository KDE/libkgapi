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
#include <Akonadi/ResourceBase>

namespace Ui {
  class SettingsDialog;
};

class SettingsDialog : public KDialog
{
  Q_OBJECT
  public:
    SettingsDialog(WId windowId, QWidget *parent = 0);
    ~SettingsDialog();

  private Q_SLOTS:
    void refreshCalendarList();
    void calendarListRetrieved(KJob *job);
    void calendarChanged(int index);
    
    void revokeTokens();
    void authenticate();
    
    void setAuthenticated(bool authenticated);
    
  private:
    Ui::SettingsDialog *m_ui;
    QWidget *m_mainWidget;    
    
    QString m_accessToken;
    QString m_refreshToken;
    QString m_calendarId;
    
    WId m_windowId;
};

#endif // SETTINGSDIALOG_H
