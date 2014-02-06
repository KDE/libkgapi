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

#include "job.h"
#include "job_p.h"

#include "debug.h"
#include "authjob.h"

#include <KDE/KLocalizedString>

#include <qjson/parser.h>

using namespace KGAPI2;


Job::Private::Private(Job *parent):
    isRunning(false),
    error(KGAPI2::NoError),
    maxTimeout(0),
    q(parent)
{
}

void Job::Private::init()
{
    QTimer::singleShot(0, q, SLOT(_k_doStart()));

    accessManager = new KIO::Integration::AccessManager(q);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)),
            q, SLOT(_k_replyReceived(QNetworkReply*)));

    dispatchTimer = new QTimer(q);
    connect(dispatchTimer, SIGNAL(timeout()),
            q, SLOT(_k_dispatchTimeout()));
}

QString Job::Private::parseErrorMessage(const QByteArray &json)
{
    QJson::Parser parser;
    bool ok;

    QVariant data = parser.parse(json, &ok);
    if (ok) {
        QString message;
        QVariantMap map = data.toMap();

        if (map.contains(QLatin1String("error"))) {
            map = map.value(QLatin1String("error")).toMap();
        }

        if (map.contains(QLatin1String("message"))) {
            message.append(map.value(QLatin1String("message")).toString());
        } else {
            message = QLatin1String(json);
        }

        return message;

    } else {
        return QLatin1String(json);
    }
}

void Job::Private::_k_doStart()
{
    isRunning = true;
    q->aboutToStart();
    q->start();
}

void Job::Private::_k_doEmitFinished()
{
    Q_EMIT q->finished(q);
}

void Job::Private::_k_replyReceived(QNetworkReply* reply)
{
    int replyCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (replyCode == 0) {

        /* Workaround for a bug (??), when QNetworkReply does not report HTTP/1.1 401 Unauthorized
         * as an error. */
        if (!reply->rawHeaderList().isEmpty()) {
            QString status = QLatin1String(reply->rawHeaderList().first());
            if (status.startsWith(QLatin1String("HTTP/1.1 401")))
                replyCode = KGAPI2::Unauthorized;
        }
    }

    const QByteArray rawData = reply->readAll();

    KGAPIDebug() << "Received reply from" << reply->url();
    KGAPIDebugRawData() << rawData;

    switch (replyCode) {
        case KGAPI2::NoError:
        case KGAPI2::OK:           /** << OK status (fetched, updated, removed) */
        case KGAPI2::Created:      /** << OK status (created) */
        case KGAPI2::NoContent:    /** << OK status (removed task using Tasks API) */
            break;

        case KGAPI2::TemporarilyMoved: {  /** << Temporarily moved - Google provides a new URL where to send the request */
            KGAPIDebug() << "Google says: Temporarily moved to " << reply->header(QNetworkRequest::LocationHeader).toUrl();
            QNetworkRequest request = reply->request();
            request.setUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
            q->enqueueRequest(request);
            return;
        }

        case KGAPI2::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
            KGAPIWarning() << "Bad request, Google replied '" << rawData << "'";
            q->setError(KGAPI2::BadRequest);
            q->setErrorString(i18n("Bad request."));
            q->emitFinished();
            return;

        case KGAPI2::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
            KGAPIWarning() << "Unauthorized. Access token has expired or is invalid.";
            q->setError(KGAPI2::Unauthorized);
            q->setErrorString(i18n("Invalid authentication."));
            q->emitFinished();
            return;

        case KGAPI2::Forbidden: {
            KGAPIWarning() << "Requested resource is forbidden.";
            KGAPIDebugRawData() << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::Forbidden);
            q->setErrorString(i18n("Requested resource is forbidden.\n\nGoogle replied '%1'", msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::NotFound: {
            KGAPIWarning() << "Requested resource does not exist";
            KGAPIDebugRawData() << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::NotFound);
            q->setErrorString(i18n("Requested resource does not exist.\n\nGoogle replied '%1'", msg));
            // don't emit finished() here, we can get 404 when fetching contact photos or so,
            // in that case 404 is not fatal. Let subclass decide whether to terminate or not.
            q->handleReply(reply, rawData);

            if (requestQueue.isEmpty()) {
                q->emitFinished();
            }
            return;
        }

        case KGAPI2::Conflict: {
            KGAPIWarning() << "Conflict. Remote resource is newer then local.";
            KGAPIDebugRawData() << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::Conflict);
            q->setErrorString(i18n("Conflict. Remote resource is newer than local.\n\nGoogle replied '%1'", msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::Gone: {
            KGAPIWarning() << "Requested resource does not exist anymore.";
            KGAPIDebugRawData() << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::Gone);
            q->setErrorString(i18n("Requested resource does not exist anymore.\n\nGoogle replied '%1'", msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::InternalError: {
            KGAPIWarning() << "Internal server error.";
            KGAPIDebugRawData() << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::InternalError);
            q->setErrorString(i18n("Internal server error. Try again later.\n\nGoogle replied '%1'", msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::QuotaExceeded: {
            KGAPIWarning() << "User quota exceeded.";
            KGAPIDebugRawData() << rawData;

            // Extend the interval (if possible) and enqueue the request again
            int interval = dispatchTimer->interval() / 1000;
            if (interval == 0) {
                interval = 1;
            } else if (interval == 1) {
                interval = 2;
            } else if ((interval > maxTimeout) && (maxTimeout > 0)) {
                const QString msg = parseErrorMessage(rawData);
                q->setError(KGAPI2::QuotaExceeded);
                q->setErrorString(i18n("Maximum quota exceeded. Try again later.\n\nGoogle replied '%1'", msg));
                q->emitFinished();
                return;
            } else {
                interval = interval ^ 2;
            }
            KGAPIDebug() << "Increasing dispatch interval to" << interval * 1000 << "msecs";
            dispatchTimer->setInterval(interval * 1000);

            const QNetworkRequest request = reply->request();
            q->enqueueRequest(request);
            if (!dispatchTimer->isActive()) {
                dispatchTimer->start();
            }
            return;
        }

        default:{  /** Something went wrong, there's nothing we can do about it */
            KGAPIWarning() << "Unknown error" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            KGAPIDebugRawData() << rawData;
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::UnknownError);
            q->setErrorString(i18n("Unknown error.\n\nGoogle replied '%1'", msg));
            q->emitFinished();
            return;
        }
    }

    q->handleReply(reply, rawData);

    // handleReply has terminated the job, don't continue
    if (!q->isRunning()) {
        return;
    }

    KGAPIDebug() << requestQueue.length() << "requests in requestQueue.";
    if (requestQueue.isEmpty()) {
        q->emitFinished();
        return;
    }

    if (!dispatchTimer->isActive()) {
        dispatchTimer->start();
    }
}

void Job::Private::_k_dispatchTimeout()
{
    if (requestQueue.isEmpty()) {
        dispatchTimer->stop();
        return;
    }

    const Request r = requestQueue.dequeue();
    currentRequest = r;

    KGAPIDebug() << "Dispatching request to" << r.request.url();
    KGAPIDebugRawData() << r.rawData;

    q->dispatchRequest(accessManager, r.request, r.rawData, r.contentType);

    if (requestQueue.isEmpty()) {
        dispatchTimer->stop();
    }
}

/************************* PUBLIC **********************/

Job::Job(QObject* parent):
    QObject(parent),
    d(new Private(this))
{
    d->init();
}

Job::Job(const AccountPtr& account, QObject* parent):
    QObject(parent),
    d(new Private(this))
{
    d->account = account;

    d->init();
}


Job::~Job()
{
    delete d;
}

void Job::setError(Error error)
{
    d->error = error;
}

Error Job::error() const
{
    if (isRunning()) {
        KGAPIWarning() << "Called error() on running job, returning nothing";
        return KGAPI2::NoError;
    }

    return d->error;
}

void Job::setErrorString(const QString& errorString)
{
    d->errorString = errorString;
}

QString Job::errorString() const
{
    if (isRunning()) {
        KGAPIWarning() << "Called errorString() on running job, returning nothing";
        return QString();
    }

    return d->errorString;
}

bool Job::isRunning() const
{
    return d->isRunning;
}

int Job::maxTimeout() const
{
    return d->maxTimeout;
}

void Job::setMaxTimeout(int maxTimeout)
{
    if (isRunning()) {
        KGAPIWarning() << "Called setMaxTimeout() on running job. Ignoring.";
        return;
    }

    d->maxTimeout = maxTimeout;
}

AccountPtr Job::account() const
{
    return d->account;
}

void Job::setAccount(const AccountPtr& account)
{
    if (d->isRunning) {
        KGAPIWarning() << "Called setAccount() on running job. Ignoring.";
        return;
    }

    d->account = account;
}

void Job::restart()
{
    if (d->isRunning) {
        KGAPIWarning() << "Running job cannot be restarted.";
        return;
    }

    QTimer::singleShot(0, this, SLOT(_k_doStart()));
}

void Job::emitFinished()
{
    KGAPIDebug();
    aboutToFinish();

    d->isRunning = false;
    d->dispatchTimer->stop();
    d->requestQueue.clear();

    // Emit in next event loop iteration so that the method caller can finish
    // before user is notified
    QTimer::singleShot(0, this, SLOT(_k_doEmitFinished()));
}

void Job::emitProgress(int processed, int total)
{
    Q_EMIT progress(this, processed, total);
}

void Job::enqueueRequest(const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    if (!isRunning()) {
        KGAPIDebug() << "Can't enqueue requests when job is not running.";
        KGAPIDebug() << "Not enqueueing" << request.url();
        return;
    }

    KGAPIDebug() << "Queued" << request.url();

    Request r_;
    r_.request = request;
    r_.rawData = data;
    r_.contentType = contentType;

    d->requestQueue.enqueue(r_);

    if (!d->dispatchTimer->isActive()) {
        d->dispatchTimer->start();
    }
}

void Job::aboutToFinish()
{
}

void Job::aboutToStart()
{
    d->error = KGAPI2::NoError;
    d->errorString.clear();
    d->currentRequest.contentType.clear();
    d->currentRequest.rawData.clear();
    d->currentRequest.request = QNetworkRequest();
    d->dispatchTimer->setInterval(0);
}


#include "job.moc"
