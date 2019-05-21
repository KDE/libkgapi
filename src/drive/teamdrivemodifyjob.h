/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2019  David Barchiesi <david@barchie.si>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef KGAPI2_DRIVETEAMDRIVEMODIFYJOB_H
#define KGAPI2_DRIVETEAMDRIVEMODIFYJOB_H

#include "modifyjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT TeamdriveModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:
    explicit TeamdriveModifyJob(const TeamdrivePtr &teamdrive,
                                 const AccountPtr &account, QObject *parent = nullptr);
    explicit TeamdriveModifyJob(const TeamdrivesList &teamdrives,
                                 const AccountPtr &account, QObject *parent = nullptr);
    ~TeamdriveModifyJob() override;

  protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData) override;

  private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVETEAMDRIVEMODIFYJOB_H