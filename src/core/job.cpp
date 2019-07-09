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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "job.h"
#include "job_p.h"
#include "account.h"
#include "networkaccessmanagerfactory_p.h"
#include "../debug.h"
#include "authjob.h"
#include "utils.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QTextStream>
#include <QFile>
#include <QUrlQuery>

using namespace KGAPI2;

FileLogger *FileLogger::sInstance = nullptr;

FileLogger::FileLogger()
{
    if (!qEnvironmentVariableIsSet("KGAPI_SESSION_LOGFILE")) {
        return;
    }

    QString filename = QString::fromLocal8Bit(qgetenv("KGAPI_SESSION_LOGFILE"))
        + QLatin1Char('.')
        + QString::number(QCoreApplication::applicationPid());
    mFile.reset(new QFile(filename));
    if (!mFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCWarning(KGAPIDebug) << "Failed to open logging file" << filename << ":" << mFile->errorString();
        mFile.reset();
    }
}

FileLogger::~FileLogger() {}

FileLogger *FileLogger::self()
{
    if (!sInstance) {
        sInstance = new FileLogger();
    }
    return sInstance;
}

void FileLogger::logRequest(const QNetworkRequest &request, const QByteArray &rawData)
{
    if (!mFile) {
        return;
    }

    QTextStream stream(mFile.data());
    stream << "C: " << request.url().toDisplayString() << "\n";
    const auto headers = request.rawHeaderList();
    for (const auto &header : headers) {
        stream << "   " << header << ": " << request.rawHeader(header) << "\n";
    }
    stream << "   " << rawData << "\n\n";
    mFile->flush();
}

void FileLogger::logReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    if (!mFile) {
        return;
    }

    QTextStream stream(mFile.data());
    stream << "S: " << reply->url().toDisplayString() << "\n";
    const auto headers = reply->rawHeaderList();
    for (const auto &header : headers) {
        stream << "   " << header << ": " << reply->rawHeader(header) << "\n";
    }
    stream << "   " << rawData << "\n\n";
    mFile->flush();
}



Job::Private::Private(Job *parent):
    isRunning(false),
    error(KGAPI2::NoError),
    accessManager(nullptr),
    maxTimeout(0),
    prettyPrint(false),
    q(parent)
{
}

void Job::Private::init()
{
    QTimer::singleShot(0, q, [this]() { _k_doStart(); });

    accessManager = NetworkAccessManagerFactory::instance()->networkAccessManager(q);
    connect(accessManager, &QNetworkAccessManager::finished,
            q, [this](QNetworkReply *reply) { _k_replyReceived(reply); });

    dispatchTimer = new QTimer(q);
    connect(dispatchTimer, &QTimer::timeout,
            q, [this]() { _k_dispatchTimeout(); });
}

QString Job::Private::parseErrorMessage(const QByteArray &json)
{
    QJsonDocument document = QJsonDocument::fromJson(json);
    if (!document.isNull()) {
        QVariantMap map = document.toVariant().toMap();
        QString message;

        if (map.contains(QStringLiteral("error"))) {
            map = map.value(QStringLiteral("error")).toMap();
        }

        if (map.contains(QStringLiteral("message"))) {
            message.append(map.value(QStringLiteral("message")).toString());
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

    qCDebug(KGAPIDebug) << "Received reply from" << reply->url();
    qCDebug(KGAPIDebug) << "Status code: " << replyCode;
    FileLogger::self()->logReply(reply, rawData);

    switch (replyCode) {
        case KGAPI2::NoError:
        case KGAPI2::OK:           /** << OK status (fetched, updated, removed) */
        case KGAPI2::Created:      /** << OK status (created) */
        case KGAPI2::NoContent:    /** << OK status (removed task using Tasks API) */
            break;

        case KGAPI2::TemporarilyMoved: {  /** << Temporarily moved - Google provides a new URL where to send the request */
            qCDebug(KGAPIDebug) << "Google says: Temporarily moved to " << reply->header(QNetworkRequest::LocationHeader).toUrl();
            QNetworkRequest request = currentRequest.request;
            request.setUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
            q->enqueueRequest(request, currentRequest.rawData, currentRequest.contentType);
            return;
        }

        case KGAPI2::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
            qCWarning(KGAPIDebug) << "Bad request, Google replied '" << rawData << "'";
            q->setError(KGAPI2::BadRequest);
            q->setErrorString(tr("Bad request."));
            q->emitFinished();
            return;

        case KGAPI2::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
            qCWarning(KGAPIDebug) << "Unauthorized. Access token has expired or is invalid.";
            q->setError(KGAPI2::Unauthorized);
            q->setErrorString(tr("Invalid authentication."));
            q->emitFinished();
            return;

        case KGAPI2::Forbidden: {
            qCWarning(KGAPIDebug) << "Requested resource is forbidden.";
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::Forbidden);
            q->setErrorString(tr("Requested resource is forbidden.\n\nGoogle replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::NotFound: {
            qCWarning(KGAPIDebug) << "Requested resource does not exist";
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::NotFound);
            q->setErrorString(tr("Requested resource does not exist.\n\nGoogle replied '%1'").arg(msg));
            // don't emit finished() here, we can get 404 when fetching contact photos or so,
            // in that case 404 is not fatal. Let subclass decide whether to terminate or not.
            q->handleReply(reply, rawData);

            if (requestQueue.isEmpty()) {
                q->emitFinished();
            }
            return;
        }

        case KGAPI2::Conflict: {
            qCWarning(KGAPIDebug) << "Conflict. Remote resource is newer then local.";
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::Conflict);
            q->setErrorString(tr("Conflict. Remote resource is newer than local.\n\nGoogle replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::Gone: {
            qCWarning(KGAPIDebug) << "Requested resource does not exist anymore.";
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::Gone);
            q->setErrorString(tr("Requested resource does not exist anymore.\n\nGoogle replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::InternalError: {
            qCWarning(KGAPIDebug) << "Internal server error.";
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::InternalError);
            q->setErrorString(tr("Internal server error. Try again later.\n\nGoogle replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }

        case KGAPI2::QuotaExceeded: {
            qCWarning(KGAPIDebug) << "User quota exceeded.";

            // Extend the interval (if possible) and enqueue the request again
            int interval = dispatchTimer->interval() / 1000;
            if (interval == 0) {
                interval = 1;
            } else if (interval == 1) {
                interval = 2;
            } else if ((interval > maxTimeout) && (maxTimeout > 0)) {
                const QString msg = parseErrorMessage(rawData);
                q->setError(KGAPI2::QuotaExceeded);
                q->setErrorString(tr("Maximum quota exceeded. Try again later.\n\nGoogle replied '%1'").arg(msg));
                q->emitFinished();
                return;
            } else {
                interval = interval ^ 2;
            }
            qCDebug(KGAPIDebug) << "Increasing dispatch interval to" << interval * 1000 << "msecs";
            dispatchTimer->setInterval(interval * 1000);

            const QNetworkRequest request = reply->request();
            q->enqueueRequest(request);
            if (!dispatchTimer->isActive()) {
                dispatchTimer->start();
            }
            return;
        }

        default:{  /** Something went wrong, there's nothing we can do about it */
            qCWarning(KGAPIDebug) << "Unknown error" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            const QString msg = parseErrorMessage(rawData);
            q->setError(KGAPI2::UnknownError);
            q->setErrorString(tr("Unknown error.\n\nGoogle replied '%1'").arg(msg));
            q->emitFinished();
            return;
        }
    }

    q->handleReply(reply, rawData);

    // handleReply has terminated the job, don't continue
    if (!q->isRunning()) {
        return;
    }

    qCDebug(KGAPIDebug) << requestQueue.length() << "requests in requestQueue.";
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

    QNetworkRequest authorizedRequest = r.request;
    if (account) {
        authorizedRequest.setRawHeader("Authorization", "Bearer " + account->accessToken().toLatin1());
    }

    QUrl url = authorizedRequest.url();
    QUrlQuery standardParamQuery(url);
    if (!fields.isEmpty()) {
        standardParamQuery.addQueryItem(Job::StandardParams::Fields, fields.join(QStringLiteral(",")));
    }

    if (!standardParamQuery.hasQueryItem(Job::StandardParams::PrettyPrint)) {
        standardParamQuery.addQueryItem(Job::StandardParams::PrettyPrint, Utils::bool2Str(prettyPrint));
    }

    url.setQuery(standardParamQuery);
    authorizedRequest.setUrl(url);

    qCDebug(KGAPIDebug) << q << "Dispatching request to" << r.request.url();
    FileLogger::self()->logRequest(authorizedRequest, r.rawData);

    q->dispatchRequest(accessManager, authorizedRequest, r.rawData, r.contentType);

    if (requestQueue.isEmpty()) {
        dispatchTimer->stop();
    }
}

/************************* PUBLIC **********************/

const QString Job::StandardParams::PrettyPrint = QStringLiteral("prettyPrint");
const QString Job::StandardParams::Fields = QStringLiteral("fields");

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
        qCWarning(KGAPIDebug) << "Called error() on running job, returning nothing";
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
        qCWarning(KGAPIDebug) << "Called errorString() on running job, returning nothing";
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
        qCWarning(KGAPIDebug) << "Called setMaxTimeout() on running job. Ignoring.";
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
        qCWarning(KGAPIDebug) << "Called setAccount() on running job. Ignoring.";
        return;
    }

    d->account = account;
}

bool Job::prettyPrint() const
{
    return d->prettyPrint;
}

void Job::setPrettyPrint(bool prettyPrint)
{
    if (d->isRunning) {
        qCWarning(KGAPIDebug) << "Called setPrettyPrint() on running job. Ignoring.";
        return;
    }

    d->prettyPrint = prettyPrint;
}

QStringList Job::fields() const
{
    return d->fields;
}

void Job::setFields(const QStringList &fields)
{
    d->fields = fields;
}

QString Job::buildSubfields(const QString &field, const QStringList &fields) {
    return QStringLiteral("%1(%2)").arg(field).arg(fields.join(QStringLiteral(",")));
}

void Job::restart()
{
    if (d->isRunning) {
        qCWarning(KGAPIDebug) << "Running job cannot be restarted.";
        return;
    }

    QTimer::singleShot(0, this, [this]() { d->_k_doStart();});
}

void Job::emitFinished()
{
    aboutToFinish();

    d->isRunning = false;
    d->dispatchTimer->stop();
    d->requestQueue.clear();

    // Emit in next event loop iteration so that the method caller can finish
    // before user is notified
    QTimer::singleShot(0, this, [this]() { d->_k_doEmitFinished(); });
}

void Job::emitProgress(int processed, int total)
{
    Q_EMIT progress(this, processed, total);
}

void Job::enqueueRequest(const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    if (!isRunning()) {
        qCDebug(KGAPIDebug) << "Can't enqueue requests when job is not running.";
        qCDebug(KGAPIDebug) << "Not enqueueing" << request.url();
        return;
    }

    qCDebug(KGAPIDebug) << "Queued" << request.url();

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

#include "moc_job.cpp"
