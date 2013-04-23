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

#include "account.h"

using namespace KGAPI2;

#include "common/account.inc.cpp"

QUrl Account::accountInfoScopeUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/auth/userinfo.profile"));
}

QUrl Account::accountInfoEmailScopeUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/auth/userinfo.email"));
}

QUrl Account::calendarScopeUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/auth/calendar"));
}

QUrl Account::contactsScopeUrl()
{
    return QUrl(QLatin1String("https://www.google.com/m8/feeds/"));
}

QUrl Account::latitudeScopeUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/auth/latitude.all.best"));
}

QUrl Account::tasksScopeUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/auth/tasks"));
}
