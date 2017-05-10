/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "accountinfofetchjob.h"
#include "account.h"
#include "accountinfo.h"
#include "../debug.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN AccountInfoFetchJob::Private
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
    QNetworkRequest request(QUrl(QStringLiteral("https://www.googleapis.com/oauth2/v1/userinfo")));
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
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
