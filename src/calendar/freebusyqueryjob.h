/*
 * SPDX-FileCopyrightText: 2015 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapicalendar_export.h"

#include <QDateTime>
#include <QList>
#include <QScopedPointer>

namespace KGAPI2
{

class KGAPICALENDAR_EXPORT FreeBusyQueryJob : public KGAPI2::FetchJob
{
    Q_OBJECT
public:
    struct BusyRange {
        BusyRange() = default;
        BusyRange(const QDateTime &busyStart, const QDateTime &busyEnd)
            : busyStart(busyStart)
            , busyEnd(busyEnd)
        {
        }

        bool operator==(const BusyRange &other) const
        {
            return busyStart == other.busyStart && busyEnd == other.busyEnd;
        }

        QDateTime busyStart;
        QDateTime busyEnd;
    };
    using BusyRangeList = QList<BusyRange>;

    explicit FreeBusyQueryJob(const QString &id, const QDateTime &timeMin, const QDateTime &timeMax, const AccountPtr &account, QObject *parent = nullptr);
    ~FreeBusyQueryJob() override;

    [[nodiscard]] QString id() const;
    [[nodiscard]] QDateTime timeMin() const;
    [[nodiscard]] QDateTime timeMax() const;

    [[nodiscard]] BusyRangeList busy() const;

protected:
    void start() override;
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType) override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

}
