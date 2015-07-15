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

#include "accountinfofetchjob.h"
#include "account.h"
#include "accountinfo.h"
#include "../debug.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class AccountInfoFetchJob::Private
{
};

AccountInfoFetchJob::AccountInfoFetchJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private)
{
}

AccountInfoFetchJob::~AccountInfoFetchJob()
{
    delete d;
}

void AccountInfoFetchJob::start()
{
    QNetworkRequest request(QUrl(QLatin1String("https://www.googleapis.com/oauth2/v1/userinfo")));
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    enqueueRequest(request);
}

ObjectsList AccountInfoFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        items << AccountInfo::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    return items;
}

#include "moc_accountinfofetchjob.cpp"
