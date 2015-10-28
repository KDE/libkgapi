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

#ifndef SERVICESTASKSTESTS_H
#define SERVICESTASKSTESTS_H

#include <qtest_kde.h>

namespace KGAPI {
namespace Objects {
class Task;
class TaskList;
}
}

class ServicesTasksTests : public QObject
{
    Q_OBJECT

  private:
    void compareTasks(const KGAPI::Objects::Task *task, const QSettings &values, bool fromSerializer = false);
    void compareTasklists(const KGAPI::Objects::TaskList *taskList, const QSettings &values, bool fromSerializer = false);

  private Q_SLOTS:
    void testTasksParser();
    void testTasklistsParser();

    void testTasksSerializer();
    void testTasklistsSerializer();

    void testUrls();
};

#endif // SERVICESTASKSTESTS_H
