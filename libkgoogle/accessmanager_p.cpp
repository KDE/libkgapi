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

#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qbytearray.h>
#include <qurl.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <kdebug.h>

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
  connect(Auth::instance(), SIGNAL(authenticated(KGoogle::Account*)),
          this, SLOT(submitCache()));
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
  foreach (QString str, reply->rawHeaderList()) {
    headers << str + ": " + reply->rawHeader(str.toLatin1());
  }
  kDebug() << headers;
  kDebug() << rawData;
#endif

  KGoogle::Request *request = reply->request().attribute(RequestAttribute).value<KGoogle::Request*>();
  if (!request) {
    emit q->error(KGoogle::InvalidResponse, i18n("No valid reply received"));
    return;
  }

  switch (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
    case KGoogle::OK:           /** << OK status (fetched, updated, removed) */
    case KGoogle::Created:      /** << OK status (created) */
    case KGoogle::NoContent:    /** << OK status (removed task using Tasks API) */
      break;

    case KGoogle::TemporarilyMoved:  /** << Temporarily moved - Google provides a new URL where to send the request */
      kDebug() << "Google says: Temporarily moved to " << reply->header(QNetworkRequest::LocationHeader).toUrl();
      request->setUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
      nam_sendRequest(request);
      return;

    case KGoogle::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
      kWarning() << "Bad request, Google replied '" << rawData << "'";
      emit q->error(KGoogle::BadRequest, i18n("Bad request."));
      return;

    case KGoogle::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
      /* Lock the service, add request to cache and request new tokens. */
      if (cacheSemaphore->available() == 1) {
        cacheSemaphore->acquire();
        cache << request;

        int type = QMetaType::type(qPrintable(request->serviceName()));
        KGoogle::Service *service = static_cast<KGoogle::Service*>(QMetaType::construct(type));
        QUrl scope = service->scopeUrl();

        if (!request->account()->scopes().contains(scope))
          request->account()->addScope(scope);

        KGoogle::Auth *auth = KGoogle::Auth::instance();
        auth->authenticate(request->account(), true);
      }
      /* Don't emit error here, user should not know that we need to re-negotiate tokens again. */
      return;

    default: /** Something went wrong, there's nothing we can do about it */
      kWarning() << "Unknown error" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << ", Google replied '" << rawData << "'";
      emit q->error(KGoogle::UnknownError, i18n("Unknown error.\n\nGoogle replied '%1'", QString(rawData)));
      return;
  }

  QList< KGoogle::Object* > replyData;

  int type = QMetaType::type(qPrintable(request->serviceName()));
  KGoogle::Service *service = static_cast<KGoogle::Service*>(QMetaType::construct(type));

  switch (request->requestType()) {
    /* For fetch-all request parse the XML/JSON reply and split it to individual
     * <entry>/entry blocks which then convert to QList of KGoogleObjects */
    case KGoogle::Request::FetchAll: {
      KGoogle::FeedData* feedData = new KGoogle::FeedData;

      if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json") ||
          reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/plain")) {

        replyData = service->parseJSONFeed(rawData, feedData);

      } else if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/atom+xml") ||
                 reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/xml")) {

        replyData = service->parseXMLFeed(rawData, feedData);

      } else {
        kDebug() << "Unknown reply content type!";
      }

      if (feedData->nextLink.isValid()) {
        QUrl url(feedData->nextLink);
        new_request.setUrl(request->url().toString());
        new_request.removeAllQueryItems("start-index");
        new_request.removeAllQueryItems("max-results");
        new_request.addQueryItem("start-index", url.queryItemValue("start-index"));
        new_request.addQueryItem("max-results", url.queryItemValue("max-results"));

        delete feedData;

        break;
      }
    } break;

    case KGoogle::Request::Fetch:
    case KGoogle::Request::Create:
    case KGoogle::Request::Update: {
      if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json") ||
          reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/plain")) {

        replyData.append(service->JSONToObject(rawData));

      } else if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/atom+xml") ||
                 reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text/xml")) {

        replyData.append(service->XMLToObject(rawData));

      }
    } break;

    case KGoogle::Request::Remove:
      break;
  }

  KGoogle::Reply *greply;
  greply = new KGoogle::Reply(request->requestType(),
                              (KGoogle::Error) reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                              request->serviceName(), replyData, request, rawData);

  emit q->replyReceived(greply);

  /* Re-send the request on a new URL */
  if (new_request.isValid()) {
    request->setUrl(new_request);
    nam_sendRequest(request);
  } else {
    emit q->requestFinished(request);
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

  nr.setRawHeader("Authorization","Bearer " + request->account()->accessToken().toLatin1());
  nr.setRawHeader("GData-Version", service->protocolVersion().toLatin1());
  nr.setUrl(request->url());
  nr.setAttribute(QNetworkRequest::User, QVariant::fromValue(request));

#ifdef DEBUG_RAWDATA
  QStringList headers;
  foreach (QString str, nr.rawHeaderList()) {
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
  }
}


void AccessManagerPrivate::submitCache()
{
  while (!cache.isEmpty() && cacheSemaphore->available())
    nam_sendRequest(cache.takeFirst());
}
