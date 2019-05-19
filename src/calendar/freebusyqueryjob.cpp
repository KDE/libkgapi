/*
 * Copyright 2015  Daniel Vrátil <dvratil@redhat.com>
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

#include "freebusyqueryjob.h"
#include "calendarservice.h"
#include "utils.h"
#include "account.h"

#include <QVariantMap>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

using namespace KGAPI2;

class Q_DECL_HIDDEN FreeBusyQueryJob::Private
{
public:
    Private(const QString &id, const QDateTime &timeMin, const QDateTime &timeMax)
        : id(id)
        , timeMin(timeMin)
        , timeMax(timeMax)
    {}

    QString id;
    QDateTime timeMin;
    QDateTime timeMax;
    FreeBusyQueryJob::BusyRangeList busy;
};


FreeBusyQueryJob::FreeBusyQueryJob(const QString &id, const QDateTime &timeMin, const QDateTime &timeMax,
                                   const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new FreeBusyQueryJob::Private(id, timeMin, timeMax))
{
}

FreeBusyQueryJob::~FreeBusyQueryJob()
{
    delete d;
}

FreeBusyQueryJob::BusyRangeList FreeBusyQueryJob::busy() const
{
    return d->busy;
}

QString FreeBusyQueryJob::id() const
{
    return d->id;
}

QDateTime FreeBusyQueryJob::timeMin() const
{
    return d->timeMin;
}

QDateTime FreeBusyQueryJob::timeMax() const
{
    return d->timeMax;
}

void FreeBusyQueryJob::start()
{
    QVariantMap requestData({
        { QStringLiteral("timeMin"), Utils::rfc3339DateToString(d->timeMin) },
        { QStringLiteral("timeMax"), Utils::rfc3339DateToString(d->timeMax) },
        { QStringLiteral("items"), 
            QVariantList({
                QVariantMap({ { QStringLiteral("id"), d->id } })
            })
        }});
    QJsonDocument document = QJsonDocument::fromVariant(requestData);
    const QByteArray json = document.toJson(QJsonDocument::Compact);

    QNetworkRequest request(CalendarService::freeBusyQueryUrl());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());
    enqueueRequest(request, json, QStringLiteral("application/json"));
}

void FreeBusyQueryJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                       const QNetworkRequest &request,
                                       const QByteArray &data,
                                       const QString &contentType)
{
    QNetworkRequest r = request;
    if (!r.hasRawHeader("Content-Type")) {
        r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    accessManager->post(r, data);
}

void FreeBusyQueryJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        const QJsonDocument document = QJsonDocument::fromJson(rawData);
        const QVariantMap data = document.toVariant().toMap();
        const QVariantMap cals = data[QStringLiteral("calendars")].toMap();
        const QVariantMap cal = cals[d->id].toMap();
        if (cal.contains(QStringLiteral("errors"))) {
            setError(KGAPI2::NotFound);
            setErrorString(tr("FreeBusy information is not available"));
        } else {
            const QVariantList busyList = cal[QStringLiteral("busy")].toList();
            for (const QVariant &busyV : busyList) {
                const QVariantMap busy = busyV.toMap();
                d->busy << BusyRange{ 
                    Utils::rfc3339DateFromString(busy[QStringLiteral("start")].toString()),
                    Utils::rfc3339DateFromString(busy[QStringLiteral("end")].toString())
                };
            }
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
}
