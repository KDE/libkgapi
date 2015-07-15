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

#ifndef KGAPI2_DRIVECHILDREFERENCECREATEJOB_H
#define KGAPI2_DRIVECHILDREFERENCECREATEJOB_H

#include "createjob.h"
#include "kgapidrive_export.h"

#include <QtCore/QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT ChildReferenceCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:
    explicit ChildReferenceCreateJob(const QString &folderId,
                                     const QString &childId,
                                     const AccountPtr &account,
                                     QObject *parent = 0);
    explicit ChildReferenceCreateJob(const QString &folderId,
                                     const QStringList &childrenIds,
                                     const AccountPtr &account,
                                     QObject *parent = 0);
    explicit ChildReferenceCreateJob(const QString &folderId,
                                     const ChildReferencePtr &reference,
                                     const AccountPtr &account,
                                     QObject *parent = 0);
    explicit ChildReferenceCreateJob(const QString &folderId,
                                     const ChildReferencesList &references,
                                     const AccountPtr &account,
                                     QObject *parent = 0);
    virtual ~ChildReferenceCreateJob();

  protected:
    virtual void start();
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray &rawData);

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVECHILDREFERENCECREATEJOB_H
