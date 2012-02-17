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
#include "groupeditor.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include <libkgoogle/auth.h>
#include <libkgoogle/accessmanager.h>
#include <libkgoogle/request.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/services/contacts.h>
#include <libkgoogle/objects/contactsgroup.h>


using namespace KGoogle;

enum {
  KGoogleObjectRole = Qt::UserRole,
  ObjectUIDRole
};

SettingsDialog::SettingsDialog(WId windowId, QWidget *parent):
  KDialog(parent),
  m_windowId (windowId)
{
  KWindowSystem::setMainWindow(this, windowId);

  qRegisterMetaType<KGoogle::Services::Contacts>("Contacts");

  this->setButtons (Ok | Cancel);

  m_mainWidget = new QWidget();
  m_ui = new ::Ui::SettingsDialog();
  m_ui->setupUi(m_mainWidget);
  setMainWidget(m_mainWidget);

  connect(this, SIGNAL(accepted()),
	  this, SLOT(saveSettings()));

  connect(m_ui->addAccountBtn, SIGNAL(clicked(bool)),
          this, SLOT(addAccountClicked()));
  connect(m_ui->removeAccountBtn, SIGNAL(clicked(bool)),
          this, SLOT(removeAccountClicked()));
  connect(m_ui->accountsCombo, SIGNAL(currentIndexChanged(int)),
          this, SLOT(accountChanged()));

  connect(m_ui->addGroupBtn, SIGNAL(clicked(bool)),
          this, SLOT(addGroupClicked()));
  connect(m_ui->editGroupBtn, SIGNAL(clicked(bool)),
          this, SLOT(editGroupClicked()));
  connect(m_ui->removeGroupBtn, SIGNAL(clicked(bool)),
          this, SLOT(removeGroupClicked()));
  connect(m_ui->reloadGroupsBtn, SIGNAL(clicked(bool)),
          this, SLOT(reloadGroupsClicked()));
  connect(m_ui->groupsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
          this, SLOT(editGroupClicked()));
  connect(m_ui->groupsTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
	  this, SLOT(groupSelected(QTreeWidgetItem*)));
  connect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  KGoogle::Auth *auth = KGoogle::Auth::instance();
  connect(auth, SIGNAL(authenticated(KGoogle::Account*)),
	  this, SLOT(reloadAccounts()));

  m_ui->addGroupBtn->setDisabled(true);
  m_ui->editGroupBtn->setDisabled(true);
  m_ui->removeGroupBtn->setDisabled(true);
  m_ui->reloadGroupsBtn->setDisabled(true);

  reloadAccounts();
}

SettingsDialog::~SettingsDialog()
{
  delete m_ui;
  delete m_mainWidget;
}

void SettingsDialog::saveSettings()
{
  Settings::self()->setAccount(m_ui->accountsCombo->currentText());

  QStringList groups;
  for (int i = 0; i < m_ui->groupsTree->topLevelItemCount(); i++) {

    QTreeWidgetItem *item = m_ui->groupsTree->topLevelItem(i);
    if (item->checkState(0) != Qt::Checked)
      continue;

    Objects::ContactsGroup *group;

    group = item->data(0, KGoogleObjectRole).value< KGoogle::Objects::ContactsGroup * >();
    if (!group)
      continue;

    groups << group->id();

    for (int j = 0; j < item->childCount(); j++) {

      QTreeWidgetItem *child = item->child(j);
      if (child->checkState(0) != Qt::Checked)
	continue;

      Objects::ContactsGroup *group;

      group = child->data(0, KGoogleObjectRole).value< KGoogle::Objects::ContactsGroup * >();
      if (!group)
	continue;

      groups << group->id();

    }

  }
  Settings::self()->setGroups(groups);

  Settings::self()->writeConfig();
}


void SettingsDialog::error(KGoogle::Error errCode, const QString &msg)
{
  if (errCode == KGoogle::OK)
    return;

  KMessageBox::error(this, msg, i18n("An error occured"));

  m_ui->accountsBox->setEnabled(true);
  m_ui->groupsBox->setEnabled(true);
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
  account->addScope(Services::Contacts::ScopeUrl);

  try {
    auth->authenticate(account, true);
  } catch (KGoogle::Exception::BaseException &e) {
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
  } catch (KGoogle::Exception::BaseException &e) {
    KMessageBox::error(this, e.what());
  }

  reloadAccounts();
}

void SettingsDialog::accountChanged()
{
  reloadGroupsClicked();

  Settings::self()->setAccount(m_ui->accountsCombo->currentText());
  Settings::self()->setGroups(QStringList());

  m_ui->reloadGroupsBtn->setEnabled(true);
}

void SettingsDialog::addGroupClicked()
{
  GroupEditor *editor = new GroupEditor;
  connect(editor, SIGNAL(accepted(KGoogle::Objects::ContactsGroup *)),
          this, SLOT(addGroup(KGoogle::Objects::ContactsGroup *)));

  editor->exec();

  delete editor;
}

void SettingsDialog::addGroup(Objects::ContactsGroup *group)
{
  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;
  Services::Contacts parser;
  QByteArray data;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->groupsBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply *)),
          this, SLOT(gam_groupCreated(KGoogle::Reply *)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request *)),
          gam, SLOT(deleteLater()));
  connect(gam, SIGNAL(error(KGoogle::Error,QString)),
	  this, SLOT(error(KGoogle::Error,QString)));

  request = new KGoogle::Request(Services::Contacts::createGroupUrl(account->accountName()),
                                 Request::Create, "Contacts", account);
  data = parser.objectToXML(dynamic_cast< KGoogle::Object * >(group));
  data.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
			   "xmlns:gd=\"http://schemas.google.com/g/2005\" "
			   "xmlns:gContact=\"http://schemas.google.com/contact/2008\">");
  data.append("</atom:entry>");
  request->setRequestData(data, "application/atom+xml");
  gam->sendRequest(request);

  delete group;
}

void SettingsDialog::editGroupClicked()
{
  Objects::ContactsGroup *group;
  QTreeWidgetItem *item;

  item = m_ui->groupsTree->currentItem();
  if (!item)
    return;

  group = item->data(0, KGoogleObjectRole).value< KGoogle::Objects::ContactsGroup * >();
  if (!group)
    return;

  if (group->isSystemGroup()) {
    KMessageBox::sorry(this, i18n("System groups cannot be modified."));
    return;
  }

  GroupEditor *editor = new GroupEditor(group);
  connect(editor, SIGNAL(accepted(KGoogle::Objects::ContactsGroup *)),
          this, SLOT(editGroup(KGoogle::Objects::ContactsGroup *)));

  editor->exec();

  delete editor;
}

void SettingsDialog::editGroup(Objects::ContactsGroup *group)
{
  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;
  Services::Contacts parser;
  QByteArray data;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->groupsBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply *)),
          this, SLOT(gam_groupModified(KGoogle::Reply *)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request *)),
          gam, SLOT(deleteLater()));
  connect(gam, SIGNAL(error(KGoogle::Error,QString)),
	  this, SLOT(error(KGoogle::Error,QString)));

  request = new KGoogle::Request(Services::Contacts::updateGroupUrl(account->accountName(), group->id()),
                                 Request::Update, "Contacts", account);
  data = parser.objectToXML(dynamic_cast< KGoogle::Object * >(group));
  data.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
			   "xmlns:gd=\"http://schemas.google.com/g/2005\" "
			   "xmlns:gContact=\"http://schemas.google.com/contact/2008\">");
  data.append("</atom:entry>");

  request->setRequestData(data, "application/atom+xml");
  gam->sendRequest(request);

  delete group;
}

void SettingsDialog::removeGroupClicked()
{
  Objects::ContactsGroup *group;
  QTreeWidgetItem *item;

  item = m_ui->groupsTree->currentItem();
  if (!item)
    return;

  group = item->data(0, KGoogleObjectRole).value< KGoogle::Objects::ContactsGroup * >();
  if (!group)
    return;

  if (group->isSystemGroup()) {
    KMessageBox::sorry(this, i18n("System groups cannot be removed."));
    return;
  }

  if (KMessageBox::warningYesNo(this,
                                i18n("Do you really want to remove contact group <b>%1</b>?<br>"
                                     "<b>This will remove the group from Google servers as well!</b>", group->title()),
                                i18n("Remove Group?"),
                                KStandardGuiItem::yes(), KStandardGuiItem::no(), QString(), KMessageBox::Dangerous) != KStandardGuiItem::Yes)
    return;

  KGoogle::Account *account;
  KGoogle::AccessManager *gam;
  KGoogle::Request *request;

  account = m_ui->accountsCombo->currentAccount();

  m_ui->accountsBox->setDisabled(true);
  m_ui->groupsBox->setDisabled(true);

  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply *)),
          this, SLOT(reloadGroupsClicked()));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request *)),
          gam, SLOT(deleteLater()));
  connect(gam, SIGNAL(error(KGoogle::Error,QString)),
	  this, SLOT(error(KGoogle::Error,QString)));

  request = new KGoogle::Request(Services::Contacts::removeGroupUrl(account->accountName(), group->id()),
                                 Request::Remove, "Contacts", account);
  gam->sendRequest(request);
}

void SettingsDialog::reloadGroupsClicked()
{
  KGoogle::AccessManager *gam;
  KGoogle::Account *account;
  KGoogle::Request *request;

  m_ui->accountsBox->setDisabled(true);
  m_ui->groupsBox->setDisabled(true);

  account = m_ui->accountsCombo->currentAccount();
  if (!account) {
    m_ui->accountsBox->setEnabled(true);
    return;
  }

  m_ui->groupsTree->clear();
  gam = new KGoogle::AccessManager;
  connect(gam, SIGNAL(replyReceived(KGoogle::Reply *)),
          this, SLOT(gam_groupsListReceived(KGoogle::Reply *)));
  connect(gam, SIGNAL(requestFinished(KGoogle::Request *)),
          gam, SLOT(deleteLater()));
  connect(gam, SIGNAL(error(KGoogle::Error,QString)),
	  this, SLOT(error(KGoogle::Error,QString)));

  request = new KGoogle::Request(Services::Contacts::fetchAllGroupsUrl(account->accountName()),
                                 Request::FetchAll, "Contacts", account);
  gam->sendRequest(request);
}

void SettingsDialog::gam_groupsListReceived(KGoogle::Reply *reply)
{
  QList< KGoogle::Object * > objects = reply->replyData();

  disconnect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  while (!objects.isEmpty()) {

    KGoogle::Object *object = objects.first();

    Objects::ContactsGroup *group;
    QTreeWidgetItem *item;

    group = static_cast< Objects::ContactsGroup * >(object);
    item = new QTreeWidgetItem();
    item->setText(0, group->title());
    item->setData(0, KGoogleObjectRole, qVariantFromValue(group));
    item->setData(0, ObjectUIDRole, group->id());

    if (Settings::self()->groups().contains(group->id()))
      item->setCheckState(0, Qt::Checked);
    else
      item->setCheckState(0, Qt::Unchecked);

    if (group->isSystemGroup()) {
      m_ui->groupsTree->addTopLevelItem(item);
      objects.removeAll(object);
    } else {
      if (m_ui->groupsTree->topLevelItemCount() == 0)
	continue;

      QTreeWidgetItem *tlItem = m_ui->groupsTree->topLevelItem(0);
      tlItem->addChild(item);
      objects.removeAll(object);
    }
  }

  connect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  m_ui->groupsBox->setEnabled(true);
  m_ui->accountsBox->setEnabled(true);
  m_ui->addGroupBtn->setEnabled(true);

  delete reply;
}

void SettingsDialog::gam_groupCreated(KGoogle::Reply *reply)
{
  QList< KGoogle::Object* > objects = reply->replyData();

  /* FIXME: error! */
  if (objects.count() != 1)
    return;

  Objects::ContactsGroup *group = static_cast< Objects::ContactsGroup* >(objects.first());
  QTreeWidgetItem *item;

  disconnect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  item = new QTreeWidgetItem;
  item->setText(0, group->title());
  item->setData(0, KGoogleObjectRole, qVariantFromValue(group));
  item->setData(0, ObjectUIDRole, group->id());

  connect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  if (Settings::self()->groups().contains(group->id()))
    item->setCheckState(0, Qt::Checked);
  else
    item->setCheckState(0, Qt::Unchecked);

  QTreeWidgetItem *tlItem = m_ui->groupsTree->topLevelItem(0);
  tlItem->addChild(item);

  m_ui->groupsBox->setEnabled(true);
  m_ui->accountsBox->setEnabled(true);
}

void SettingsDialog::gam_groupModified(Reply *reply)
{
  QList< KGoogle::Object* > objects = reply->replyData();

  /* FIXME: error! */
  if (objects.count() != 1)
    return;

  Objects::ContactsGroup *group = static_cast< Objects::ContactsGroup* >(objects.first());

  disconnect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  QTreeWidgetItem *tlItem = m_ui->groupsTree->topLevelItem(0);
  for (int i = 0; i < tlItem->childCount(); i++) {
    QTreeWidgetItem *item = tlItem->child(i);

    if (item->data(0, ObjectUIDRole).toString() == group->id()) {
      item->setText(0, group->title());
      item->setData(0, KGoogleObjectRole, qVariantFromValue(group));
      break;
    }
  }

  connect(m_ui->groupsTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	  this, SLOT(groupChecked(QTreeWidgetItem*)));

  m_ui->groupsBox->setEnabled(true);
}

void SettingsDialog::gam_groupRemoved(Reply *reply)
{
  QTreeWidgetItem *item;
  QTreeWidgetItem *parent;

  item = m_ui->groupsTree->currentItem();
  parent =  item->parent();

  item->setCheckState(0, Qt::Unchecked);
  parent->removeChild(item);

  m_ui->groupsBox->setEnabled(true);
  m_ui->accountsBox->setEnabled(true);

  Q_UNUSED (reply);
}


void SettingsDialog::groupSelected(QTreeWidgetItem *item)
{
  Objects::ContactsGroup *group;

  if (!item)
    return;

  group = item->data(0, KGoogleObjectRole).value< KGoogle::Objects::ContactsGroup * >();
  if (!group)
    return;


  if (group->isSystemGroup()) {
    m_ui->editGroupBtn->setDisabled(true);
    m_ui->removeGroupBtn->setDisabled(true);
  } else {
    m_ui->editGroupBtn->setEnabled(true);
    m_ui->removeGroupBtn->setEnabled(true);
  }
}

void SettingsDialog::groupChecked(QTreeWidgetItem *item)
{
  if (!item)
    return;

  if ((item->parent() != 0) && (item->checkState(0) == Qt::Checked))
    item->parent()->setCheckState(0, Qt::Checked);

  if ((item->parent() == 0) && (item->checkState(0) == Qt::Unchecked)) {
    for (int i = 0; i < item->childCount(); i++)
      item->child(i)->setCheckState(0, Qt::Unchecked);
  }
}
