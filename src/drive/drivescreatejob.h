/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEDRIVESCREATEJOB_H
#define KGAPI2_DRIVEDRIVESCREATEJOB_H

#include "createjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT DrivesCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:
    DrivesCreateJob(const QString &requestId,
                            const DrivesPtr &drives,
                            const AccountPtr &account, QObject *parent = nullptr);
    DrivesCreateJob(const QString &requestId,
                            const DrivesList &drives,
                            const AccountPtr &account, QObject *parent = nullptr);
    ~DrivesCreateJob() override;

    /**
     * @brief Returns the requestId used in this create request.
     */
    QString requestId() const;

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

#endif // KGAPI2_DRIVEDRIVESCREATEJOB_H
