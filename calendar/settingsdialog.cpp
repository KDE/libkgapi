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

#include <Qt/qpixmap.h>
#include <Qt/qicon.h>
#include <Qt/qcolor.h>

#include <KDE/KMessageBox>
#include <KDE/KWindowSystem>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include "calendarlistjob.h"
#include "libkgoogle/authdialog.h"

SettingsDialog::SettingsDialog(WId windowId, QWidget* parent):
  KDialog(parent),
  m_windowId (windowId)
{
  KWindowSystem::setMainWindow(this, windowId);
  
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
    m_calendar = new Calendar();
    m_calendar->setId(Settings::self()->calendarId());
    m_calendar->setTitle(Settings::self()->calendarName());
    m_calendar->setColor(Settings::self()->calendarColor());
  }
  
  if (Settings::self()->accessToken().isEmpty() ||
      Settings::self()->refreshToken().isEmpty()) {
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
  CalendarListJob *clJob = new CalendarListJob(Settings::self()->accessToken());
  connect (clJob, SIGNAL(finished(KJob*)),
	   this, SLOT(calendarListRetrieved(KJob*)));

  m_ui->refreshListButton->setText("Refreshing list...");
  m_ui->calendarGroupBox->setEnabled(false);
  
  clJob->start();
}

void SettingsDialog::calendarListRetrieved(KJob* job)
{
  CalendarListJob *clJob = dynamic_cast<CalendarListJob*>(job);
  
  if (clJob->error()) {
    KMessageBox::error(this, 
      i18n("Failed to refresh the list of calendars. The remote server replied:\n")+clJob->errorString(),
      i18n("Failed to refresh the list of calendars"), 0);
  } else {
    disconnect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	       this, SLOT(calendarChanged(int)));

    m_ui->comboBox->clear();
    foreach (Calendar *calendar, clJob->calendars()) {
      int index;
      
      QPixmap pixmap(12, 12);
      QColor color(calendar->color());
      pixmap.fill(color);
      QIcon icon(pixmap);
      
      m_ui->comboBox->addItem(icon, calendar->title());
      index = m_ui->comboBox->count()-1;
      m_ui->comboBox->setItemData(index, calendar->id(), Calendar::CalendarIdRole);
      m_ui->comboBox->setItemData(index, qVariantFromValue(calendar->clone()), Calendar::CalendarRole);
    }

  if (m_calendar) {
      int index = m_ui->comboBox->findData(m_calendar->id(), Calendar::CalendarIdRole);
      m_ui->comboBox->setCurrentIndex(index);
    } else {
      int index = m_ui->comboBox->currentIndex();
      setCalendar(m_ui->comboBox->itemData(index, Calendar::CalendarRole).value<Calendar*>());
    }

    connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)),
	    this, SLOT(calendarChanged(int)));
    
    
  }
  
  m_ui->refreshListButton->setText("Refresh list");
  m_ui->calendarGroupBox->setEnabled(true);
}

void SettingsDialog::calendarChanged(int index)
{
  setCalendar(m_ui->comboBox->itemData(index, Calendar::CalendarRole).value<Calendar*>());
}


void SettingsDialog::authenticate()
{
  AuthDialog *authDialog = new AuthDialog(0, m_windowId);
  authDialog->setScopes(QStringList() << "https://www.google.com/calendar/feeds/");      
  authDialog->auth(Settings::self()->clientId(),
		   Settings::self()->clientSecret());
  if (authDialog->exec() == KDialog::Accepted) {
    Settings::self()->setAccessToken(authDialog->accessToken());
    Settings::self()->setRefreshToken(authDialog->refreshToken());
    Settings::self()->writeConfig();
    setAuthenticated(true);
    refreshCalendarList();      
  } else {
    setAuthenticated(false);
  }
    
  delete authDialog;
}

void SettingsDialog::revokeTokens()
{
  Settings::self()->accessToken().clear();
  Settings::self()->refreshToken().clear();
  Settings::self()->calendarId().clear();
  Settings::self()->calendarColor().clear();
  Settings::self()->calendarName().clear();
  
  Settings::self()->writeConfig();
  
  setAuthenticated(false);
}

void SettingsDialog::setCalendar(Calendar* calendar)
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
