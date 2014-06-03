/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef KGAPI2_DRIVEFILEDELETEJOB_H
#define KGAPI2_DRIVEFILEDELETEJOB_H

#include <libkgapi2/deletejob.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QStringList>

namespace KGAPI2
{

namespace Drive
{

class LIBKGAPI2_EXPORT FileDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    explicit FileDeleteJob(const QString &fileId,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileDeleteJob(const QStringList &filesIds,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileDeleteJob(const FilePtr &file,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileDeleteJob(const FilesList &files,
                           const AccountPtr &account, QObject *parent = 0);
    virtual ~FileDeleteJob();

  protected:
    virtual void start();

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEDELETEJOB_H
