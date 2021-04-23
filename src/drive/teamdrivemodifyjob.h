/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#pragma once

#include "modifyjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_DEPRECATED_EXPORT TeamdriveModifyJob : public KGAPI2::ModifyJob
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

