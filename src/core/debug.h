/*
    Copyright 2013  Daniel Vrátil <dvratil@redhat.com>

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

#ifndef LIBKGAPI2_DEBUG_H
#define LIBKGAPI2_DEBUG_H

#include <QDebug>

#include "kgapicore_export.h"

KGAPICORE_EXPORT QDebug KGAPIDebugRawData();


KGAPICORE_EXPORT inline QDebug KGAPIDebug()
{
    return qDebug();
}

KGAPICORE_EXPORT inline QDebug KGAPIWarning()
{
    return qWarning();
}

#endif // LIBKGAPI2_DEBUG_H
