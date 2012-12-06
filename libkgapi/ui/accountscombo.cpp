/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "accountscombo.h"
#include "auth.h"

#include <KLocalizedString>

namespace KGAPI {

namespace Ui {

class AccountsComboPrivate {

};

} /* namespace Ui */

} /* namespace KGAPI */


using namespace KGAPI::Ui;

AccountsCombo::AccountsCombo(QWidget *parent):
    KComboBox(parent),
    d_ptr(new AccountsComboPrivate)
{
    reload();
}

AccountsCombo::~AccountsCombo()
{
    delete d_ptr;
}

KGAPI::Account::Ptr AccountsCombo::currentAccount() const
{
    int index = currentIndex();

    if (index == -1)
        return KGAPI::Account::Ptr();

    return qVariantValue< KGAPI::Account::Ptr >(itemData(index, Qt::UserRole));
}

void AccountsCombo::reload()
{
    KGAPI::Auth *auth = KGAPI::Auth::instance();
    QList< KGAPI::Account::Ptr > accounts;
    clear();

    try {
        accounts = auth->getAccounts();
    } catch (KGAPI::Exception::BackendNotReady &e) {
        /* Slot must not throw an exception, application might not be ready for that */
        Q_UNUSED(e);
        return;
    }

    Q_FOREACH(const KGAPI::Account::Ptr &account, accounts) {
        addItem(account->accountName(), qVariantFromValue(account));
    }
}
