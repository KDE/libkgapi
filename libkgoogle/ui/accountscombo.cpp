/*
    libKGoogle - Ui - AccountsCombo
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


#include "accountscombo.h"
#include "auth.h"

#include <klocalizedstring.h>

using namespace KGoogle::Ui;

AccountsCombo::AccountsCombo (QWidget *parent):
  QComboBox (parent)
{
  QStandardItemModel *model;
  QList< QStandardItem* > columns;

  model = new QStandardItemModel(parent);

  initModel(model);
  setModel(model);
}

AccountsCombo::~AccountsCombo()
{

}

KGoogle::Account *AccountsCombo::currentAccount() const
{
  int index = currentIndex();

  return qVariantValue< KGoogle::Account* >(itemData(index, AccountDataRole));
}

void AccountsCombo::initModel (QStandardItemModel *model)
{
  KGoogle::Auth *auth = KGoogle::Auth::instance();
  QList< KGoogle::Account* > accounts;

  accounts = auth->getAccounts();
  foreach (KGoogle::Account *account, accounts) {
    QStandardItem *row = new QStandardItem(account->accountName());
    row->setData(qVariantFromValue<KGoogle::Account*>(account), AccountDataRole);
    model->appendRow(row);
  }
}

void AccountsCombo::reload()
{
  QStandardItemModel *si_model = static_cast< QStandardItemModel* >(model());

  si_model->clear();
  initModel(si_model);
}
