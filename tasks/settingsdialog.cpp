/*
    Akonadi Google - Tasks Resource
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

#include "libkgoogle/kgoogleaccessmanager.h"
#include "libkgoogle/kgooglerequest.h"
#include "libkgoogle/kgooglereply.h"
#include "libkgoogle/kgoogleauth.h"
#include "libkgoogle/objects/tasklist.h"
#include "libkgoogle/services/tasks.h"

using namespace KGoogle;

SettingsDialog::SettingsDialog(WId windowId, KGoogleAuth *googleAuth, QWidget* parent):
  KDialog(parent),
  m_taskList(0),
  m_windowId (windowId),
  m_googleAuth(googleAuth)
{
  qRegisterMetaType<KGoogle::Service::Tasks>("Tasks");
  
  KWindowSystem::setMainWindow(this, windowId);
  
  connect(m_googleAuth, SIGNAL(tokensRecevied(QString,QString)),
	  this, SLOT(authenticated(QString,QString)));
  
  m_gam = new KGoogleAccessManager(m_googleAuth);
  connect (m_gam, SIGNAL (replyReceived(KGoogleReply*)),
	   this, SLOT (replyReceived(KGoogleReply*)));
  
  m_mainWidget = new QWidget();
  
  m_ui = new Ui::SettingsDialog();
  m_ui->setupUi(m_mainWidget);
  setMainWidget(m_mainWidget);
  
  connect(m_ui->revokeAuthButton, SIGNAL(clicked(bool)),
	  this, SLOT(revokeTokens()));
  connect(m_ui->authenticateButton, SIGNAL(clicked(bool)),
	  this, SLOT(authenticate()));
  connect(m_ui->refreshListButton, SIGNAL(clicked(bool)),
	  this, SLOT(refreshTasksList()));
  connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(taskListChanged(int)));
  connect(m_ui->refreshTaskListComboBox, SIGNAL(clicked(bool)),
	  m_ui->refreshIntervalSpinBox, SLOT(setEnabled(bool)));
  
  m_ui->refreshTaskListComboBox->setChecked(Settings::self()->refreshTaskList());
  m_ui->refreshIntervalSpinBox->setEnabled(m_ui->refreshTaskListComboBox->isChecked());
  m_ui->refreshIntervalSpinBox->setValue(Settings::self()->refreshInterval());
  
  if (!Settings::self()->taskListId().isEmpty()) {
    m_taskList = new Object::TaskList();
    m_taskList->setId(Settings::self()->taskListId());
    m_taskList->setTitle(Settings::self()->taskListName());
  }
  
  if (m_googleAuth->accessToken().isEmpty()) {
    setAuthenticated(false);
  } else {
    setAuthenticated(true);
    refreshTasksList();
  }
}

SettingsDialog::~SettingsDialog()
{
  Settings::self()->setRefreshTaskList(m_ui->refreshTaskListComboBox->isChecked());
  Settings::self()->setRefreshInterval(m_ui->refreshIntervalSpinBox->value());
  Settings::self()->writeConfig();  
  
  delete m_ui;
  delete m_mainWidget;
  
  if (m_taskList)
    delete m_taskList;
}

void SettingsDialog::setAuthenticated(bool authenticated)
{
  m_ui->authenticateButton->setVisible(!authenticated);
  m_ui->authenticateLabel->setVisible(!authenticated);

  m_ui->revokeAuthButton->setVisible(authenticated);
  m_ui->revokeAuthLabel->setVisible(authenticated);
  
  m_ui->taskListGroupBox->setEnabled(authenticated);
}


void SettingsDialog::refreshTasksList()
{
  QString url = Service::Tasks::fetchTaskListsUrl();
  KGoogleRequest *request = new KGoogleRequest(QUrl(url),
					       KGoogleRequest::FetchAll,
					       "Tasks");
  m_gam->sendRequest(request);
}


void SettingsDialog::authenticated(QString accessToken, QString refreshToken)
{
  if (!accessToken.isEmpty() && !refreshToken.isEmpty()) {
    setAuthenticated(true);
    refreshTasksList();
  } else {
    setAuthenticated(false);
  }
}


void SettingsDialog::replyReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    qDebug() << reply->readAll();
    KMessageBox::error(this, 
      i18n("Failed to refresh the list of task lists."),
      i18n("Failed to refresh the list of task lists."), 0);
    return;
  }
  
  disconnect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	   this, SLOT(taskListChanged(int)));


  m_ui->comboBox->clear();
  
  QList<KGoogleObject*> replyData = reply->replyData();
  
  foreach (KGoogleObject* data, replyData) {
    Object::TaskList* taskList = static_cast<Object::TaskList*>(data);
  
    m_ui->comboBox->addItem(taskList->title());
    int index = m_ui->comboBox->count()-1;
    m_ui->comboBox->setItemData(index, taskList->id(), 1001);
    m_ui->comboBox->setItemData(index, QVariant::fromValue(taskList), 1002);
  }

  if (m_taskList) {
    int index = m_ui->comboBox->findData(m_taskList->id(), 1001);
    m_ui->comboBox->setCurrentIndex(index);
  } else {
    int index = m_ui->comboBox->currentIndex();
    setTaskList(m_ui->comboBox->itemData(index, 1002).value<Object::TaskList*>());
  }

  connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(taskListChanged(int)));
    
  m_ui->refreshListButton->setText("Refresh list");
  m_ui->taskListGroupBox->setEnabled(true);
}

void SettingsDialog::taskListChanged(int index)
{
  setTaskList(m_ui->comboBox->itemData(index, 1002).value<Object::TaskList*>());
}


void SettingsDialog::authenticate()
{
  m_googleAuth->requestToken(0);
}

void SettingsDialog::revokeTokens()
{
  m_googleAuth->revokeTokens();
  
  Settings::self()->setTaskListId(QString());
  Settings::self()->setTaskListName(QString());
  Settings::self()->writeConfig();
  
  setAuthenticated(false);
}

void SettingsDialog::setTaskList(Object::TaskList* taskList)
{
  if (m_taskList)
    delete m_taskList;
  
  m_taskList = taskList;
   
  if (m_taskList) {
    Settings::self()->setTaskListId(m_taskList->id());
    Settings::self()->setTaskListName(m_taskList->title());
  } else {
    Settings::self()->setTaskListId(QString());
    Settings::self()->setTaskListName(QString());
  }
  Settings::self()->writeConfig();
}
