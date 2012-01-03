/*
    libKGoogle - Ui - AccountsTreeView
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


#ifndef LIBKGOOGLE_UI_ACCOUNTSTREEVIEW_H
#define LIBKGOOGLE_UI_ACCOUNTSTREEVIEW_H

#include <qlistview.h>
#include <account.h>

class QStandardItemModel;

namespace KGoogle
{

  class Account;

  namespace Ui
  {

    class AccountsListView : public QListView
    {
        Q_OBJECT

      public:
        enum ItemRoles
        {
          AccountDataRole = Qt::UserRole + 1
        };

        explicit AccountsListView (QWidget *parent = 0);

        virtual ~AccountsListView();

        KGoogle::Account* currentAccount() const;

      public Q_SLOTS:
        void reload();

      private:
        void initModel (QStandardItemModel *model);
    };

  }

}

#endif // KGOOGLE_UI_ACCOUNTSTREEVIEW_H
