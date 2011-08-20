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

namespace Ui {
  class SettingsDialog;
}

namespace KGoogle {
  class KGoogleAuth;
}

using namespace KGoogle;

class SettingsDialog : public KDialog
{
  Q_OBJECT
  public:
    SettingsDialog(WId windowId, KGoogleAuth *googleAuth, QWidget *parent = 0);
    ~SettingsDialog();

  private Q_SLOTS:
    void revokeTokens();
    void authenticate();
    
    void setAuthenticated(bool authenticated);
    
    void authenticated(QString accessToken, QString refreshToken);
    
  private:
    Ui::SettingsDialog *m_ui;
    QWidget *m_mainWidget;    
    
    WId m_windowId;

    KGoogleAuth *m_googleAuth;
};

#endif // SETTINGSDIALOG_H
