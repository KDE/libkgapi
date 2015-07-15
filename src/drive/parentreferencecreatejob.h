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

#ifndef KGAPI2_DRIVEPARENTREFERENCECREATEJOB_H
#define KGAPI2_DRIVEPARENTREFERENCECREATEJOB_H

#include "createjob.h"
#include "kgapidrive_export.h"

#include <QtCore/QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT ParentReferenceCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:
    explicit ParentReferenceCreateJob(const QString &fileId,
                                      const QString &parentId,
                                      const AccountPtr &account,
                                      QObject *parent = 0);
    explicit ParentReferenceCreateJob(const QString &fileId,
                                      const QStringList &parentsIds,
                                      const AccountPtr &account,
                                      QObject *parent = 0);
    explicit ParentReferenceCreateJob(const QString &fileId,
                                      const ParentReferencePtr &reference,
                                      const AccountPtr &account,
                                      QObject *parent = 0);
    explicit ParentReferenceCreateJob(const QString &fileId,
                                      const ParentReferencesList &references,
                                      const AccountPtr &account,
                                      QObject *parent = 0);
    virtual ~ParentReferenceCreateJob();

  protected:
    virtual void start();
    virtual KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData);

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEPARENTREFERENCECREATEJOB_H
