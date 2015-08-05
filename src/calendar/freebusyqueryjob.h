/*
 * Copyright 2015  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KGAPI2_CALENDAR_FREEBUSYQUERYJOB_H
#define KGAPI2_CALENDAR_FREEBUSYQUERYJOB_H

#include "fetchjob.h"
#include "kgapicalendar_export.h"

#include <QVector>
#include <QPair>
#include <QDateTime>

namespace KGAPI2 {

class KGAPICALENDAR_EXPORT FreeBusyQueryJob : public KGAPI2::FetchJob
{
    Q_OBJECT
public:
    struct BusyRange {
        QDateTime busyStart;
        QDateTime busyEnd;
    };
    typedef QVector<BusyRange> BusyRangeList;

    explicit FreeBusyQueryJob(const QString &id, 
                              const QDateTime &timeMin,
                              const QDateTime &timeMax,
                              const AccountPtr &account,
                              QObject* parent = Q_NULLPTR);
    ~FreeBusyQueryJob();

    QString id() const;
    QDateTime timeMin() const;
    QDateTime timeMax() const;

    BusyRangeList busy() const;

protected:
    void start() Q_DECL_OVERRIDE;
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType) Q_DECL_OVERRIDE;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) Q_DECL_OVERRIDE;

private:
    class Private;
    Private * const d;
    friend class Private;

};

}

#endif // KGAPI2_CALENDAR_FREEBUSYQUERYJOB_H
