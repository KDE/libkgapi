/*
    Contacts Resources - Group Editor
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

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


#include "groupeditor.h"

#include "ui_groupeditor.h"

#include <libkgoogle/objects/contactsgroup.h>

GroupEditor::GroupEditor(KGoogle::Objects::ContactsGroup *group):
  KDialog(),
  m_ui(new Ui::GroupEditor()),
  m_group(group)
{
  m_ui->setupUi(mainWidget());

  if (m_group) {
    m_ui->groupNameEdit->setText(m_group->title());
  }

  connect(this, SIGNAL(okClicked()),
	  this, SLOT(accepted()));
}

GroupEditor::~GroupEditor()
{ }

void GroupEditor::accepted()
{
  if (!m_group) {
    m_group = new KGoogle::Objects::ContactsGroup();
  }

  m_group->setTitle(m_ui->groupNameEdit->text());

  emit accepted(m_group);
}

