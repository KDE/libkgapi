/*
    libKGoogle - AccessManager
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "accessmanager_p.h"
#include "accessmanager.h"
#include "auth.h"
#include "request.h"
#include "reply.h"
#include "service.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QByteArray>
#include <QtCore/QUrl>
#include <QBuffer>
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <KDE/KDebug>

#define RequestAttribute QNetworkRequest::User

extern int debugArea();

using namespace KGoogle;

AccessManagerPrivate::AccessManagerPrivate(AccessManager* const parent):
    QObject(),
    nam(new KIO::Integration::AccessManager(this)),
    cacheSemaphore(new QSemaphore(1)),
    q_ptr(parent)
{
    connect(nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(nam_replyReceived(QNetworkReply*)));
    connect(Auth::instance(), SIGNAL(authenticated(KGoogle::Account::Ptr&)),
            this, SLOT(authenticated()));
}

AccessManagerPrivate::~AccessManagerPrivate()
{
    if (nam)
        delete nam;

    if (cacheSemaphore)
        delete cacheSemaphore;
}

void AccessManagerPrivate::nam_replyReceived(QNetworkReply* reply)
{
    Q_Q(AccessManager);

    QUrl new_request;

    QByteArray rawData = reply->readAll();

#ifdef DEBUG_RAWDATA
    QStringList headers;
    foreach(QString str, reply->rawHeaderList()) {
        headers << str + ": " + reply->rawHeader(str.toLatin1());
    }
    kDebug() << headers;
    kDebug() << rawData;
#endif

    int processedItems = -1;
    int totalItems = -1;

    KGoogle::Request *request = reply->request().attribute(RequestAttribute).value<KGoogle::Request*>();
    if (!request) {
        emit q->error(KGoogle::InvalidResponse, i18n("No valid reply received"));
        return;
    }

    int replyCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (replyCode == 0) {

        /* Workaround for a bug (??), when QNetworkReply does not report HTTP/1.1 401 Unauthorized
         * as an error. */
        if (!reply->rawHeaderList().isEmpty()) {
            QString status = reply->rawHeaderList().first();
            if (status.startsWith("HTTP/1.1 401"))
                replyCode = KGoogle::Unauthorized;
        }
    }

    switch (replyCode) {
    case KGoogle::OK:           /** << OK status (fetched, updated, removed) */
    case KGoogle::Created:      /** << OK status (created) */
    case KGoogle::NoContent:    /** << OK status (removed task using Tasks API) */
        break;

    case KGoogle::TemporarilyMoved:  /** << Temporarily moved - Google provides a new URL where to send the request */
        kDebug() << "Google says: Temporarily moved to " << reply->header(QNetworkRequest::LocationHeader).toUrl();
        request->setRealUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
        nam_sendRequest(request);
        return;

    case KGoogle::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
        kWarning() << "Bad request, Google replied '" << rawData << "'";
        emit q->error(KGoogle::BadRequest, i18n("Bad request."));
        return;

    case KGoogle::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
        /* Lock the service, add request to cache and request new tokens. */
        cache.enqueue(request);
        if (cacheSemaphore->tryAcquire()) {
            Account::Ptr account = request->account();
            if (account.isNull())
                return;

            QString accName = account->accountName();

            QList< QUrl> scopes = account->scopes();
            bool scopesChanged = false;

            /* Go through the entire cache, check if there are another
             * requests with the same account waiting, but for different 
             * services. */
            foreach (KGoogle::Request *r, cache) {
                if (r->account()->accountName() == accName) {

                    int type = QMetaType::type(qPrintable(r->serviceName()));
                    KGoogle::Service *service = static_cast<KGoogle::Service*>(QMetaType::construct(type));

                    if (!scopes.contains(service->scopeUrl())) {
                        scopes << service->scopeUrl();
                        scopesChanged = true;
                    }
                }
            }

            /* Authenticate the account not just for this, but for all other
             * services for which the account has pending requests. 
             * This is purely for user's comfort. It ensures that there won't be
             * a new dialog for each service the account has pending requests.
             * Obviously this works only for requests that are already in the cache.
             */
            if (scopesChanged) {
                account->setScopes(scopes);
            }

            KGoogle::Auth *auth = KGoogle::Auth::instance();
            auth->authenticate(account, true);
        }
        /* Don't emit error here, user should not know that we need to re-negotiate tokens again. */
        return;

    case KGoogle::Forbidden:
        kWarning() << "Requested resource is forbidden.";
        kWarning() << rawData;
        emit q->error(KGoogle::Forbidden, i18n("Requested resource is forbidden.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::Forbidden, i18n("Requested resource is forbidden.\n\nGoogle replied '%1'", QString(rawData)));
        return;

    case KGoogle::NotFound:
        kWarning() << "Requested resource does not exist";
        kWarning() << rawData;
        emit q->error(KGoogle::NotFound, i18n("Requested resource does not exist.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::NotFound, i18n("Requested resource does not exist.\n\nGoogle replied '%1'", QString(rawData)));
        return;

    case KGoogle::Conflict:
        kWarning() << "Conflict. Remote resource is newer then local.";
        kWarning() << rawData;
        emit q->error(KGoogle::Conflict, i18n("Conflict. Remote resource is newer then local.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::Conflict, i18n("Conflict. Remote resource is newer then local.\n\nGoogle replied '%1'", QString(rawData)));
        return;

    case KGoogle::Gone:
        kWarning() << "Requested resource does not exist anymore.";
        kWarning() << rawData;
        emit q->error(KGoogle::Gone, i18n("Requested resource does not exist anymore.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::Gone, i18n("Requested resource does not exist anymore.\n\nGoogle replied '%1'", QString(rawData)));
        return;

    case KGoogle::InternalError:
        kWarning() << "Internal server error.";
        kWarning() << rawData;
        emit q->error(KGoogle::InternalError, i18n("Internal server error. Try again laster.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::InternalError, i18n("Internal server error. Try again laster.\n\nGoogle replied '%1'", QString(rawData)));
        return;

    case KGoogle::QuotaExceeded:
        kWarning() << "User quota exceeded.";
        kWarning() << rawData;
        emit q->error(KGoogle::QuotaExceeded, i18n("User quota exceeded. Try again later.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::QuotaExceeded, i18n("User quota exceeded. Try again later.\n\nGoogle replied '%1'", QString(rawData)));
        return;

    default: /** Something went wrong, there's nothing we can do about it */
        kWarning() << "Unknown error" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                   << ", Google replied '" << rawData << "'";
        emit q->error(KGoogle::UnknownError, i18n("Unknown error.\n\nGoogle replied '%1'", QString(rawData)));
        emit request->error(KGoogle::UnknownError, QString(rawData));
        return;
    }

    QList< KGoogle::Object* > replyData;

    int type = QMetaType::type(qPrintable(request->serviceName()));
    KGoogle::Service *service = static_cast<KGoogle::Service*>(QMetaType::construct(type));

    switch (request->requestType()) {
        /* For fetch-all request parse the XML/JSON reply and split it to individual
         * <entry>/entry blocks which then convert to QList of KGoogleObjects */
    case KGoogle::Request::FetchAll: {
        KGoogle::FeedData feedData;
        feedData.requestUrl = reply->request().url();

        if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json") ||
                reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/plain")) {

            replyData = service->parseJSONFeed(rawData, feedData);

        } else if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/atom+xml") ||
                   reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/xml")) {

            replyData = service->parseXMLFeed(rawData, feedData);

        } else {
            kDebug() << "Unknown reply content type!";
        }

        processedItems = feedData.startIndex;
        totalItems = feedData.totalResults;

        if (feedData.nextPageUrl.isValid()) {
            new_request = feedData.nextPageUrl;
        }
    } break;

    case KGoogle::Request::Fetch:
    case KGoogle::Request::Create:
    case KGoogle::Request::Update:
    case KGoogle::Request::Patch: {
        if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json") ||
                reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/plain")) {

            replyData.append(service->JSONToObject(rawData));

        } else if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/atom+xml") ||
                   reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/xml")) {

            replyData.append(service->XMLToObject(rawData));

        }
    } break;

    case KGoogle::Request::Remove:
    case KGoogle::Request::Move:
        break;
    }

    KGoogle::Reply *greply;
    greply = new KGoogle::Reply(request->requestType(),
                                (KGoogle::Error) reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                                request->serviceName(), replyData, request, rawData);

    emit q->replyReceived(greply);

    /* Re-send the request on a new URL */
    if (new_request.isValid()) {
        request->setRealUrl(new_request);
        nam_sendRequest(request);

        if ((processedItems > -1) && (totalItems > -1)) {
            emit q->requestProgress(request, processedItems, totalItems);
        }

    } else {
        emit q->requestFinished(request);

        /* Send next request from the cache */
        submitCache();
    }

    delete service;
}

void AccessManagerPrivate::nam_sendRequest(KGoogle::Request* request)
{
    Q_Q(AccessManager);

    QNetworkRequest nr;

    kDebug() << "Sending request to " << request->url();

#ifdef DEBUG_RAWDATA
    kDebug() << request->requestData();
#endif

    int type = QMetaType::type(qPrintable(request->serviceName()));
    KGoogle::Service *service = static_cast<KGoogle::Service*>(QMetaType::construct(type));
    if (!service) {
        kWarning() << "Failed to resolve service " << request->serviceName();
        emit q->error(KGoogle::UnknownService, i18n("Invalid request, service %1 is not registered.", request->serviceName()));
        return;
    }

    nr.setRawHeader("Authorization", "Bearer " + request->account()->accessToken().toLatin1());
    nr.setRawHeader("GData-Version", service->protocolVersion().toLatin1());
    nr.setUrl(request->realUrl());
    nr.setAttribute(QNetworkRequest::User, QVariant::fromValue(request));

#ifdef DEBUG_RAWDATA
    QStringList headers;
    foreach(QString str, nr.rawHeaderList()) {
        headers << str + ": " + nr.rawHeader(str.toLatin1());
    }
    kDebug() << headers;
#endif

    delete service;

    switch (request->requestType()) {
    case KGoogle::Request::FetchAll:
        nam->get(nr);
        break;

    case KGoogle::Request::Fetch:
        nam->get(nr);
        break;

    case KGoogle::Request::Create:
    case KGoogle::Request::Move:
        nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
        nam->post(nr, request->requestData());
        break;

    case KGoogle::Request::Update:
        nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
        nr.setRawHeader("If-Match", "*");
        nam->put(nr, request->requestData());
        break;

    case KGoogle::Request::Remove:
        nr.setRawHeader("If-Match", "*");
        nam->deleteResource(nr);
        break;

    case KGoogle::Request::Patch: {
        QBuffer *buffer = new QBuffer(this);
        buffer->setData(request->requestData());
        buffer->open(QIODevice::ReadOnly);
        nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
        QNetworkReply *reply = nam->sendCustomRequest(nr, "PATCH", buffer);
        connect(reply, SIGNAL(finished()), buffer, SLOT(deleteLater()));
        } break;
    }
}

void AccessManagerPrivate::authenticated()
{
    cacheSemaphore->release();

    submitCache();
}


void AccessManagerPrivate::submitCache()
{
    kDebug() << "Cache contains" << cache.size() << "requests";
    while (!cache.isEmpty() && cacheSemaphore->available())
        nam_sendRequest(cache.dequeue());
}
