/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    const QUrl url = QUrl(QStringLiteral("https://www.googleapis.com/oauth2/v1/userinfo"));
    QNetworkRequest request(url);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

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
