/*
    Akonadi Google - Contacts Resource
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

#include <KDE/KMessageBox>
#include <KDE/KWindowSystem>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include "libkgoogle/kgoogleauth.h"

using namespace KGoogle;

SettingsDialog::SettingsDialog(WId windowId, KGoogleAuth *googleAuth, QWidget* parent):
  KDialog(parent),
  m_windowId (windowId),
  m_googleAuth(googleAuth)
{
  KWindowSystem::setMainWindow(this, windowId);
  
  connect(m_googleAuth, SIGNAL(tokensRecevied(QString,QString)),
	  this, SLOT(authenticated(QString,QString)));
  
  m_mainWidget = new QWidget();
  m_ui = new Ui::SettingsDialog();
  m_ui->setupUi(m_mainWidget);
  setMainWidget(m_mainWidget);
  
  connect(m_ui->revokeAccessButton, SIGNAL(clicked(bool)),
	  this, SLOT(revokeTokens()));
  connect(m_ui->authenticateButton, SIGNAL(clicked(bool)),
	  this, SLOT(authenticate()));
  
  if (m_googleAuth->accessToken().isEmpty()) {
    setAuthenticated(false);
  } else {
    setAuthenticated(true);
  }
}

SettingsDialog::~SettingsDialog()
{
  Settings::self()->writeConfig();  
  
  delete m_ui;
  delete m_mainWidget;
}

void SettingsDialog::setAuthenticated(bool authenticated)
{
  m_ui->authenticateButton->setVisible(!authenticated);
  m_ui->authenticateLabel->setVisible(!authenticated);
  
  m_ui->revokeAccessButton->setVisible(authenticated);
  m_ui->revokeAccessLabel->setVisible(authenticated);
}


void SettingsDialog::authenticated(QString accessToken, QString refreshToken)
{
  if (!accessToken.isEmpty() && !refreshToken.isEmpty()) {
    setAuthenticated(true);
  } else {
    setAuthenticated(false);
  }
}

void SettingsDialog::authenticate()
{
  m_googleAuth->requestToken(0);
}

void SettingsDialog::revokeTokens()
{
  m_googleAuth->revokeTokens();
  
  Settings::self()->lastSync().clear();

  setAuthenticated(false);
}