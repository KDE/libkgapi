/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "calendarfetchjob.h"
#include "calendarservice.h"
#include "account.h"
#include "calendar.h"
#include "../debug.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN CalendarFetchJob::Private
{
public:
    QString calendarId;
};

CalendarFetchJob::CalendarFetchJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private())
{
}

CalendarFetchJob::CalendarFetchJob(const QString& calendarId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private())
{
    d->calendarId = calendarId;
}

CalendarFetchJob::~CalendarFetchJob() = default;

void CalendarFetchJob::start()
{
    QUrl url;
    if (d->calendarId.isEmpty()) {
        url = CalendarService::fetchCalendarsUrl();
    } else {
        url = CalendarService::fetchCalendarUrl(d->calendarId);
    }
    const auto request = CalendarService::prepareRequest(url);
    enqueueRequest(request);
}

ObjectsList CalendarFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->calendarId.isEmpty()) {
            items =  CalendarService::parseCalendarJSONFeed(rawData, feedData);
        } else {
            items << CalendarService::JSONToCalendar(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const auto request = CalendarService::prepareRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


