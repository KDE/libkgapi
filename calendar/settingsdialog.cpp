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

#include <qpixmap.h>
#include <qicon.h>
#include <qcolor.h>

#include <KDE/KMessageBox>
#include <KDE/KWindowSystem>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include "libkgoogle/kgoogleaccessmanager.h"
#include "libkgoogle/kgooglerequest.h"
#include "libkgoogle/kgooglereply.h"
#include "libkgoogle/kgoogleauth.h"
#include "libkgoogle/objects/calendar.h"
#include "libkgoogle/services/calendar.h"

using namespace KGoogle;

SettingsDialog::SettingsDialog(WId windowId, KGoogleAuth *googleAuth, QWidget* parent):
  KDialog(parent),
  m_calendar(0),
  m_windowId (windowId),
  m_googleAuth(googleAuth)
{
  qRegisterMetaType<KGoogle::Service::Calendar>("Calendar");
  
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
	  this, SLOT(refreshCalendarList()));
  connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(calendarChanged(int)));
  
  if (!Settings::self()->calendarId().isEmpty()) {
    m_calendar = new Object::Calendar();
    m_calendar->setId(Settings::self()->calendarId());
    m_calendar->setTitle(Settings::self()->calendarName());
    m_calendar->setColor(Settings::self()->calendarColor());
  }
  
  if (m_googleAuth->accessToken().isEmpty()) {
    setAuthenticated(false);
  } else {
    setAuthenticated(true);
    refreshCalendarList();
  }
}

SettingsDialog::~SettingsDialog()
{
  Settings::self()->writeConfig();  
  
  delete m_ui;
  delete m_mainWidget;
  
  if (m_calendar)
    delete m_calendar;
}

void SettingsDialog::setAuthenticated(bool authenticated)
{
  m_ui->authenticateButton->setVisible(!authenticated);
  m_ui->authenticateLabel->setVisible(!authenticated);

  m_ui->revokeAuthButton->setVisible(authenticated);
  m_ui->revokeAuthLabel->setVisible(authenticated);
  
  m_ui->calendarGroupBox->setEnabled(authenticated);
}


void SettingsDialog::refreshCalendarList()
{
  QString url = Service::Calendar::fetchAllUrl().arg("default")
					        .arg("allcalendars");
  KGoogleRequest *request = new KGoogleRequest(QUrl(url),
					       KGoogleRequest::FetchAll,
					       "Calendar");
  m_gam->sendRequest(request);
}


void SettingsDialog::authenticated(QString accessToken, QString refreshToken)
{
  if (!accessToken.isEmpty() && !refreshToken.isEmpty()) {
    setAuthenticated(true);
    refreshCalendarList();
  } else {
    setAuthenticated(false);
  }
}


void SettingsDialog::replyReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    qDebug() << reply->readAll();
    KMessageBox::error(this, 
      i18n("Failed to refresh the list of calendars."),
      i18n("Failed to refresh the list of calendars"), 0);
    return;
  }
  
  disconnect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	   this, SLOT(calendarChanged(int)));


  m_ui->comboBox->clear();
  
  QList<KGoogleObject*> replyData = reply->replyData();
  
  foreach (KGoogleObject* data, replyData) {
    Object::Calendar* calendar = static_cast<Object::Calendar*>(data);
  
    QPixmap pixmap(12, 12);
    QColor color(calendar->color());
    pixmap.fill(color);
    QIcon icon(pixmap);
    QVariant varCal;
    varCal.setValue(calendar);
    
    m_ui->comboBox->addItem(icon, calendar->title());
    int index = m_ui->comboBox->count()-1;
    m_ui->comboBox->setItemData(index, calendar->id(), 1001);
    m_ui->comboBox->setItemData(index, varCal, 1001);
  }

  if (m_calendar) {
    int index = m_ui->comboBox->findData(m_calendar->id(), 1001);
    m_ui->comboBox->setCurrentIndex(index);
  } else {
    int index = m_ui->comboBox->currentIndex();
    setCalendar(m_ui->comboBox->itemData(index, 1001).value<Object::Calendar*>());
  }

  connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(calendarChanged(int)));
    
  m_ui->refreshListButton->setText("Refresh list");
  m_ui->calendarGroupBox->setEnabled(true);
}

void SettingsDialog::calendarChanged(int index)
{
  setCalendar(m_ui->comboBox->itemData(index, 1001).value<Object::Calendar*>());
}


void SettingsDialog::authenticate()
{
  m_googleAuth->requestToken(0);
}

void SettingsDialog::revokeTokens()
{
  m_googleAuth->revokeTokens();
  
  Settings::self()->calendarId().clear();
  Settings::self()->calendarColor().clear();
  Settings::self()->calendarName().clear();
  Settings::self()->writeConfig();
  
  setAuthenticated(false);
}

void SettingsDialog::setCalendar(Object::Calendar* calendar)
{
  if (m_calendar)
    delete m_calendar;
  
  m_calendar = calendar;
   
  if (m_calendar) {
    Settings::self()->setCalendarId(m_calendar->id());
    Settings::self()->setCalendarColor(m_calendar->color());
    Settings::self()->setCalendarName(m_calendar->title());
  } else {
    Settings::self()->calendarId().clear();
    Settings::self()->calendarColor().clear();
    Settings::self()->calendarName().clear();
  }
}
