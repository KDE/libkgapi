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
  reload();
}

AccountsCombo::~AccountsCombo()
{

}

KGoogle::Account *AccountsCombo::currentAccount() const
{
  int index = currentIndex();

  if (index == -1)
    return 0;

  return qVariantValue< KGoogle::Account* >(itemData(index, Qt::UserRole));
}

void AccountsCombo::reload()
{
  KGoogle::Auth *auth = KGoogle::Auth::instance();
  QList< KGoogle::Account* > accounts;
  clear();

  try {
    accounts = auth->getAccounts();
  }
  catch (KGoogle::Exception::BackendNotReady &e) {
    /* Slot must not throw an exception, application might not be ready for that */
    return;
  }

  foreach (KGoogle::Account *account, accounts) {
    addItem(account->accountName(), qVariantFromValue(account));
  }
}
