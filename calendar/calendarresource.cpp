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

#include <Akonadi/ChangeRecorder>
#include <Akonadi/ItemFetchScope>

#include <QtCore/QPointer>

#include "calendarresource.h"
#include "settingsdialog.h"
#include "settings.h"

CalendarResource::CalendarResource(const QString &id): 
  ResourceBase(id)
{
  setObjectName(QLatin1String("GoogleCalendarResource"));
  setNeedsNetwork(true);
  
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  
  resetState();
  
  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);
  
  /* If the resource is ready then sync it */
  if (!Settings::self()->calendarId().isEmpty() &&
      !Settings::self()->accessToken().isEmpty() &&
      !Settings::self()->refreshToken().isEmpty()) {
    synchronize();
  }
}

CalendarResource::~CalendarResource()
{

}

void CalendarResource::abort()
{
  resetState();
  cancelTask();
}


void CalendarResource::resetState()
{
  m_currentJobs.clear();
}

void CalendarResource::slotAbortRequested()
{
  foreach (const QPointer<KJob> &job, m_currentJobs) {
    if (job) {
      job->kill(KJob::Quietly);
    }
  }
    
  
}


void CalendarResource::configure(WId windowId)
{
  SettingsDialog *settingsDialog = new SettingsDialog(windowId);
  if (settingsDialog->exec() == KDialog::Accepted) {
    emit configurationDialogAccepted();
  } else {
    emit configurationDialogRejected();
  }
  
  delete settingsDialog;
  
  synchronize();  
}

void CalendarResource::retrieveCollections()
{

}

void CalendarResource::retrieveItems(const Akonadi::Collection& collection)
{
  /* TODO :Implement me */
  
  Q_UNUSED (collection)
}

bool CalendarResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  /* TODO: Implement me */
  
  Q_UNUSED(item)
  Q_UNUSED(parts)
  
  return false;
}

void CalendarResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  /* TODO: Implement me */
  
  Q_UNUSED(item);
  Q_UNUSED(collection)
}

void CalendarResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
  /* TODO: Implement me */
  
  Q_UNUSED(item);
  Q_UNUSED(partIdentifiers)
}

void CalendarResource::itemRemoved(const Akonadi::Item& item)
{
  /* TODO: Implement me */
  
  Q_UNUSED(item);
}

AKONADI_RESOURCE_MAIN (CalendarResource)