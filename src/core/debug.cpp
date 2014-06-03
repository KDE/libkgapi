/*
    Copyright 2013  Dan Vratil <dan@progdan.cz>

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

#include "debug.h"

class DebugPrivate
{
public:
    DebugPrivate()
        : debugRawData(!qgetenv("KGAPI_DEBUG_RAWDATA").isEmpty())
        , nullDebug(0)
    {
        if (debugRawData) {
            nullDebug = new QDebug(&nullString);
        }
    }

    ~DebugPrivate()
    {
        delete nullDebug;
    }


    bool debugRawData;
    QString nullString;
    QDebug *nullDebug;
};

Q_GLOBAL_STATIC(DebugPrivate, sInstance)

QDebug KGAPIDebugRawData()
{
    if (sInstance()->debugRawData) {
        return *sInstance()->nullDebug;
    } else {
        return qDebug();
    }
}
