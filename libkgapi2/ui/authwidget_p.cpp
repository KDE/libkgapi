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

#include "authwidget_p.h"
#include "account.h"
#include "debug.h"
#include "accountinfo/accountinfo.h"
#include "accountinfo/accountinfofetchjob.h"
#include "private/newtokensfetchjob_p.h"

#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

#include <qjson/parser.h>

#include <KDE/KUrl>
#include <KDE/KLocalizedString>
#include <KIO/AccessManager>
#include <KDE/KWebView>

#include <QtCore/QStringBuilder>


using namespace KGAPI2;

#include "common/authwidget_p.inc.cpp"
