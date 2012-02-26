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
#include <qlistwidget.h>

#include <kmessagebox.h>
#include <kwindowsystem.h>
#include <klocalizedstring.h>

#include "calendareditor.h"
#include "settingsdialog.h"
#include "settings.h"
#include "tasklisteditor.h"
#include "ui_settingsdialog.h"

#include <libkgoogle/accessmanager.h>
#include <libkgoogle/request.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/auth.h>
#include <libkgoogle/objects/calendar.h>
#include <libkgoogle/objects/tasklist.h>
#include <libkgoogle/services/calendar.h>
#include <libkgoogle/services/tasks.h>
#include <libkgoogle/ui/accountscombo.h>

using namespace KGoogle;

enum {
  KGoogleObjectRole = Qt::UserRole,
  ObjectUIDRole = Qt::UserRole + 1
};

SettingsDialog::SettingsDialog(WId windowId, QWidget* parent):
  KDialog(parent),
  m_windowId (windowId)
{
  qRegisterMetaType<KGoogle::Services::Calendar>("Calendar");
  qRegisterMetaType<KGoogle::Services::Tasks>("Tasks");

  KWindowSystem::setMainWindow(this, windowId);

  m_ui = new ::Ui::SettingsDialog();
  m_ui->setupUi(this->mainWidget());

  m_ui->addAccountBtn->setIcon(QIcon::fromTheme("list-add-user"));
  m_ui->removeAccountBtn->setIcon(QIcon::fromTheme("list-remove-user"));
  m_ui->reloadCalendarsBtn->setIcon(QIcon::fromTheme("view-refresh"));
  m_ui->addCalBtn->setIcon(QIcon::fromTheme("list-add"));
  m_ui->removeCalBtn->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->reloadTasksBtn->setIcon(QIcon::fromTheme("view-refresh"));
  m_ui->addTasksBtn->setIcon(QIcon::fromTheme("list-add"));
  m_ui->removeTasksBtn->setIcon(QIcon::fromTheme("list-remove"));

  connect(m_ui->addAccountBtn, SIGNAL(clicked()),
          this, SLOT(addAccountClicked()));
  connect(m_ui->removeAccountBtn, SIGNAL(clicked()),
          this, SLOT(removeAccountClicked()));
  connect(m_ui->accountsCombo, SIGNAL(currentIndexChanged(int)),
          this, SLOT(accountChanged()));
  connect(m_ui->addCalBtn, SIGNAL(clicked()),
          this, SLOT(addCalendarClicked()));
  connect(m_ui->editCalBtn, SIGNAL(clicked()),
          this, SLOT(editCalendarClicked()));
  connect(m_ui->calendarsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(editCalendarClicked()));
  connect(m_ui->calendarsList, SIGNAL(itemChanged(QListWidgetItem*)),
          this, SLOT(calendarChecked(QListWidgetItem*)));
  connect(m_ui->removeCalBtn, SIGNAL(clicked()),
          this, SLOT(removeCalendarClicked()));
  connect(m_ui->reloadCalendarsBtn, SIGNAL(clicked()),
          this, SLOT(reloadCalendarsClicked()));
  connect(m_ui->addTasksBtn, SIGNAL(clicked()),
          this, SLOT(addTaskListClicked()));
  connect(m_ui->editTasksBtn, SIGNAL(clicked()),
          this, SLOT(editTaskListClicked()));
  connect(m_ui->tasksList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(editCalendarClicked()));
  connect(m_ui->tasksList, SIGNAL(itemChanged(QListWidgetItem*)),
          this, SLOT(taskListChecked(QListWidgetItem*)));
  connect(m_ui->removeTasksBtn, SIGNAL(clicked()),
          this, SLOT(removeTaskListClicked()));
  connect(m_ui->reloadTasksBtn, SIGNAL(clicked()),
          this, SLOT(reloadTaskListsClicked()));

  KGoogle::Auth *auth = KGoogle::Auth::instance();
  connect(auth, SIGNAL(authenticated(KGoogle::Account*)),
          this, SLOT(reloadAccounts()));

  reloadAccounts();
}

SettingsDialog::~SettingsDialog()
{
  Settings::self()->writeConfig();

  delete m_ui;
}

void SettingsDialog::reloadAccounts()
{
  disconnect(m_ui->accountsCombo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(accountChanged()));

  m_ui->accountsCombo->reload();

  QString accName = Settings::self()->account();
  int index = m_ui->accountsCombo->findText(accName);
  if (index > -1)
    m_ui->accountsCombo->setCurrentIndex(index);

  accountChanged();

  connect(m_ui->accountsCombo, SIGNAL(currentIndexChanged(int)),
          this, SLOT(accountChanged()));
}

void SettingsDialog::addAccountClicked()
{
  KGoogle::Auth *auth = KGoogle::Auth::instance();

  KGoogle::Account *account = new KGoogle::Account();
  account->addScope(Services::Calendar::ScopeUrl);
  account->addScope(Services::Tasks::ScopeUrl);

  try {
    auth->authenticate(account, true);
  }
  catch (KGoogle::Exception::BaseException &e) {
    KMessageBox::error(this, e.what());
  }
}

void SettingsDialog::removeAccountClicked()
{
  KGoogle::Account *account = m_ui->accountsCombo->currentAccount();
  if (!account)
    return;

  if (KMessageBox::warningYesNo(this,
      i18n("Do you really want to revoke access to account <b>%1</b>?"
           "<br>This will revoke access to all application using this account!", account->accountName()),
      i18n("Revoke Access?"),
      KStandardGuiItem::yes(), KStandardGuiItem::no(), QString(), KMessageBox::Dangerous) != KMessageBox::Yes)
    return;

  KGoogle::Auth *auth = KGoogle::Auth::instance();
  try {
    auth->revoke(account);
  }
  catch (KGoogle::Exception::BaseException &e) {
    KMessageBox::error(this, e.what());
  }

  reloadAccounts();
}

void SettingsDialog::accountChanged()
{
  m_ui->accountsBox->setDisabled(true);
  m_ui->calendarsBox->setDisabled(true);
  m_ui->tasksBox->setDisabled(true);

  Account *account = m_ui->accountsCombo->currentAccount();
  if (account == 0) {
    m_ui->accountsBox->setEnabled(true);
    m_ui->calendarsList->clear();
    m_ui->calendarsBox->setEnabled(true);
    m_ui->tasksList->clear();
    m_ui->tasksBox->setEnabled(true);
    return;
  }

  KGoogle::AccessManager *gam;
  KGoogle::Request *request;

  Settings::self()->setAccount(m_ui->accountsCombo->currentText());
  Settings::self()->setCalendars(QStringList());
  Settings::self()->setTaskLists(QStringList());
  Settings::self()->writeConfig();

  m_ui->calendarsList->clear();
  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectsListReceived(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));
  request = new KGoogle::Request(Services::Calendar::fetchAllUrl("default", "allcalendars"), 
                                 Request::FetchAll, "Calendar", account);
  gam->sendRequest(request);

  m_ui->tasksList->clear();
  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectsListReceived(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));
  request = new KGoogle::Request(Services::Tasks::fetchTaskListsUrl(), Request::FetchAll, "Tasks", account);
  gam->sendRequest(request);
}

void SettingsDialog::addCalendarClicked()
{
  CalendarEditor *editor = new CalendarEditor;
  connect(editor, SIGNAL(accepted(KGoogle::Objects::Calendar*)),
          this, SLOT(addCalendar(KGoogle::Objects::Calendar*)));

  editor->exec();

  delete editor;
}

void SettingsDialog::addCalendar(KGoogle::Objects::Calendar *calendar)
{
  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;
  Services::Calendar parser;
  QByteArray data;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->calendarsBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectCreated(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Calendar::createUrl("default", "owncalendars"),
                                 Request::Create, "Calendar", account);
  data = parser.objectToJSON(dynamic_cast< KGoogle::Object* >(calendar));
  request->setRequestData(data, "application/json");
  gam->sendRequest(request);

  delete calendar;
}

void SettingsDialog::editCalendarClicked()
{
  Objects::Calendar *calendar;
  QListWidgetItem *item;

  item = m_ui->calendarsList->currentItem();
  calendar = item->data(KGoogleObjectRole).value< KGoogle::Objects::Calendar* >();

  CalendarEditor *editor = new CalendarEditor(calendar);
  connect(editor, SIGNAL(accepted(KGoogle::Objects::Calendar*)),
          this, SLOT(editCalendar(KGoogle::Objects::Calendar*)));

  editor->exec();

  delete editor;
}

void SettingsDialog::editCalendar(KGoogle::Objects::Calendar *calendar)
{
  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;
  Services::Calendar parser;
  QByteArray data;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->calendarsBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectModified(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Calendar::updateUrl("default", "owncalendars", calendar->uid()),
                                 Request::Update, "Calendar", account);
  data = parser.objectToJSON(dynamic_cast< KGoogle::Object* >(calendar));
  request->setRequestData(data, "application/json");
  gam->sendRequest(request);

  delete calendar;
}

void SettingsDialog::removeCalendarClicked()
{
  Objects::Calendar *calendar;
  QListWidgetItem *item;

  item = m_ui->calendarsList->currentItem();
  calendar = item->data(KGoogleObjectRole).value< KGoogle::Objects::Calendar* >();

  if (KMessageBox::warningYesNo(this,
      i18n("Do you really want to remove calendar <b>%1</b>?<br>"
           "<b>This will remove the calendar from Google servers as well!</b>", calendar->title()),
      i18n("Remove Calendar?"),
      KStandardGuiItem::yes(), KStandardGuiItem::no(), QString(), KMessageBox::Dangerous) != KStandardGuiItem::Yes)
    return;

  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->calendarsBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(reloadCalendarsClicked()));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Calendar::removeUrl("default", "owncalendars", calendar->uid()),
                                 Request::Remove, "Calendar", account);
  gam->sendRequest(request);
}

void SettingsDialog::addTaskListClicked()
{
  TasklistEditor *editor = new TasklistEditor;
  connect(editor, SIGNAL(accepted(KGoogle::Objects::TaskList*)),
          this, SLOT(addTaskList(KGoogle::Objects::TaskList*)));

  editor->exec();

  delete editor;
}

void SettingsDialog::reloadCalendarsClicked()
{
  KGoogle::AccessManager *gam;
  KGoogle::Account *account;
  KGoogle::Request *request;

  m_ui->accountsBox->setDisabled(true);
  m_ui->calendarsBox->setDisabled(true);

  account = m_ui->accountsCombo->currentAccount();

  m_ui->calendarsList->clear();
  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectsListReceived(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Calendar::fetchAllUrl("default", "allcalendars"),
                                 Request::FetchAll, "Calendar", account);
  gam->sendRequest(request);
}

void SettingsDialog::calendarChecked(QListWidgetItem *item)
{
  QStringList calendars;
  QString uid;

  calendars = Settings::self()->calendars();
  uid = item->data(ObjectUIDRole).toString();

  if ((item->checkState() == Qt::Checked) && !calendars.contains(uid)) {
    calendars.append(uid);
  } else if ((item->checkState() == Qt::Unchecked) && calendars.contains(uid)) {
    calendars.removeAll(uid);
  }

  Settings::self()->setCalendars(calendars);
  Settings::self()->writeConfig();
}


void SettingsDialog::addTaskList(TaskList *taskList)
{
  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;
  Services::Tasks parser;
  QByteArray data;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->tasksBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectCreated(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Tasks::createTaskListUrl(), Request::Create, "Tasks", account);
  data = parser.objectToJSON(dynamic_cast< KGoogle::Object* >(taskList));
  request->setRequestData(data, "application/json");
  gam->sendRequest(request);

  delete taskList;
}

void SettingsDialog::editTaskListClicked()
{
  Objects::TaskList *taskList;
  QListWidgetItem *item;

  item = m_ui->tasksList->currentItem();
  taskList = item->data(KGoogleObjectRole).value< KGoogle::Objects::TaskList* >();

  TasklistEditor *editor = new TasklistEditor(taskList);
  connect(editor, SIGNAL(accepted(KGoogle::Objects::TaskList*)),
          this, SLOT(editTaskList(KGoogle::Objects::TaskList*)));

  editor->exec();

  delete editor;
}

void SettingsDialog::editTaskList(TaskList *taskList)
{
  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;
  Services::Tasks parser;
  QByteArray data;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->tasksBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectModified(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Tasks::updateTaskListUrl(taskList->uid()),
                                 Request::Update, "Tasks", account);
  data = parser.objectToJSON(dynamic_cast< KGoogle::Object* >(taskList));
  request->setRequestData(data, "application/json");
  gam->sendRequest(request);

  delete taskList;
}

void SettingsDialog::removeTaskListClicked()
{
  Objects::TaskList *taskList;
  QListWidgetItem *item;

  item = m_ui->tasksList->currentItem();
  taskList = item->data(KGoogleObjectRole).value< KGoogle::Objects::TaskList* >();

  if (KMessageBox::warningYesNo(this,
    i18n("Do you really want to remove tasklist <b>%1</b>?<br>"
    "<b>This will remove the tasklist from Google servers as well!</b>", taskList->title()),
    i18n("Remove tasklist?"),
    KStandardGuiItem::yes(), KStandardGuiItem::no(), QString(), KMessageBox::Dangerous) != KStandardGuiItem::Yes)
    return;

  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->tasksBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(reloadTaskListsClicked()));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));

  request = new KGoogle::Request(Services::Tasks::removeTaskListUrl(taskList->uid()),
                                 Request::Remove, "Tasks", account);
  gam->sendRequest(request);
}

void SettingsDialog::reloadTaskListsClicked()
{
  KGoogle::AccessManager *gam;
  KGoogle::Account *account;
  KGoogle::Request *request;

  m_ui->accountsBox->setDisabled(true);
  m_ui->tasksBox->setDisabled(true);

  account = m_ui->accountsCombo->currentAccount();

  m_ui->tasksList->clear();
  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
          this, SLOT(gam_objectsListReceived(KGoogle::Reply*)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
          gam, SLOT(deleteLater()));
  request = new KGoogle::Request(Services::Tasks::fetchTaskListsUrl(), Request::FetchAll, "Tasks", account);
  gam->sendRequest(request);
}

void SettingsDialog::taskListChecked(QListWidgetItem *item)
{
  QStringList taskLists;
  QString uid;

  taskLists = Settings::self()->taskLists();
  uid = item->data(ObjectUIDRole).toString();

  if ((item->checkState() == Qt::Checked) && !taskLists.contains(uid)) {
    taskLists.append(uid);
  } else if ((item->checkState() == Qt::Unchecked) && taskLists.contains(uid)) {
    taskLists.removeAll(uid);
  }

  Settings::self()->setTaskLists(taskLists);
  Settings::self()->writeConfig();
}

void SettingsDialog::gam_objectCreated(Reply *reply)
{
  QList< KGoogle::Object* > objects = reply->replyData();

  if (reply->serviceName() == "Calendar") {

    foreach (KGoogle::Object *object, objects) {
      KGoogle::Objects::Calendar *calendar = static_cast< KGoogle::Objects::Calendar* >(object);

      QListWidgetItem *item = new QListWidgetItem(calendar->title());
      item->setData(KGoogleObjectRole, qVariantFromValue(calendar));
      item->setData(ObjectUIDRole, calendar->uid());
      item->setCheckState(Qt::Unchecked);

      m_ui->calendarsList->addItem(item);
    }

    m_ui->calendarsBox->setEnabled(true);
    m_ui->accountsBox->setEnabled(true);

  } else if (reply->serviceName() == "Tasks") {

    foreach (KGoogle::Object *object, objects) {
      KGoogle::Objects::TaskList *taskList = static_cast< KGoogle::Objects::TaskList* >(object);

      QListWidgetItem *item = new QListWidgetItem(taskList->title());
      item->setData(KGoogleObjectRole, qVariantFromValue(taskList));
      item->setData(ObjectUIDRole, taskList->uid());
      item->setCheckState(Qt::Unchecked);

      m_ui->tasksList->addItem(item);
    }

    m_ui->tasksBox->setEnabled(true);
    m_ui->accountsBox->setEnabled(true);

  }

  delete reply;
}


void SettingsDialog::gam_objectsListReceived(Reply *reply)
{
  QList< KGoogle::Object* > objects = reply->replyData();

  if (reply->serviceName() == "Calendar") {

    foreach (KGoogle::Object *object, objects) {
      Objects::Calendar *calendar;
      QListWidgetItem *item;

      calendar = static_cast< Objects::Calendar* >(object);
      item = new QListWidgetItem;
      item->setText(calendar->title());
      item->setData(KGoogleObjectRole, qVariantFromValue(calendar));
      item->setData(ObjectUIDRole, calendar->uid());

      if (Settings::self()->calendars().contains(calendar->uid()))
        item->setCheckState(Qt::Checked);
      else
        item->setCheckState(Qt::Unchecked);

      m_ui->calendarsList->addItem(item);
    }

    m_ui->calendarsBox->setEnabled(true);
    delete reply;

  } else if (reply->serviceName() == "Tasks") {

    foreach (KGoogle::Object *object, objects) {
      Objects::TaskList *taskList;
      QListWidgetItem *item;

      taskList = static_cast< Objects::TaskList* >(object);
      item = new QListWidgetItem;
      item->setText(taskList->title());
      item->setData(KGoogleObjectRole, qVariantFromValue(taskList));
      item->setData(ObjectUIDRole, taskList->uid());

      if (Settings::self()->taskLists().contains(taskList->uid()))
        item->setCheckState(Qt::Checked);
      else
        item->setCheckState(Qt::Unchecked);

      m_ui->tasksList->addItem(item);
    }

    m_ui->tasksBox->setEnabled(true);
    delete reply;

  }

  if (m_ui->calendarsBox->isEnabled() && m_ui->tasksList->isEnabled())
    m_ui->accountsBox->setEnabled(true);
}

void SettingsDialog::gam_objectModified(Reply *reply)
{
  QList< KGoogle::Object* > objects = reply->replyData();

  if (reply->serviceName() == "Calendar") {

    foreach (KGoogle::Object *object, objects) {
      KGoogle::Objects::Calendar *calendar = static_cast< KGoogle::Objects::Calendar* >(object);
      QListWidgetItem *item = 0;

      for (int i = 0; i < m_ui->calendarsList->count(); i++) {
        QListWidgetItem *t= m_ui->calendarsList->item(i);

        if (t->data(ObjectUIDRole).toString() == calendar->uid())
          item = t;
      }

      if (item) {
        KGoogle::Objects::Calendar *oldCal;

        oldCal = item->data(KGoogleObjectRole).value< KGoogle::Objects::Calendar* >();
        delete oldCal;
        item->setText(calendar->uid());
        item->setData(KGoogleObjectRole, qVariantFromValue(calendar));

      } else {
        item = new QListWidgetItem;
        item->setText(calendar->uid());
        item->setData(KGoogleObjectRole, qVariantFromValue(calendar));
        m_ui->calendarsList->addItem(item);
      }
    }

    m_ui->calendarsBox->setEnabled(true);
    m_ui->accountsBox->setEnabled(true);

  } else if (reply->serviceName() == "Tasks") {

    foreach (KGoogle::Object *object, objects) {
      KGoogle::Objects::TaskList *taskList = static_cast< KGoogle::Objects::TaskList* >(object);
      QListWidgetItem *item = 0;

      for (int i = 0; i < m_ui->tasksList->count(); i++) {
        QListWidgetItem *t= m_ui->tasksList->item(i);

        if (t->data(ObjectUIDRole).toString() == taskList->uid())
          item = t;
      }

      if (item) {
        KGoogle::Objects::TaskList *oldTL;

        oldTL = item->data(KGoogleObjectRole).value< KGoogle::Objects::TaskList* >();
        delete oldTL;
        item->setText(taskList->uid());
        item->setData(KGoogleObjectRole, qVariantFromValue(taskList));

      } else {
        item = new QListWidgetItem;
        item->setText(taskList->uid());
        item->setData(KGoogleObjectRole, qVariantFromValue(taskList));
        m_ui->tasksList->addItem(item);
      }
    }
    m_ui->tasksBox->setEnabled(true);
    m_ui->accountsBox->setEnabled(true);

  }

  delete reply;
}
