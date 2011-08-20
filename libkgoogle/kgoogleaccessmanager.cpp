/*
    libKGoogle - KGoogleAccessManager
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

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


#include "kgoogleaccessmanager.h"
#include "kgoogleauth.h"
#include "kgooglerequest.h"
#include "kgooglereply.h"
#include "kgoogleservice.h"

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qbytearray.h>
#include <qurl.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qglobal.h>

#include <kdebug.h>


using namespace KGoogle;

#define RequestAttribute QNetworkRequest::User

KGoogleAccessManager::KGoogleAccessManager(KGoogle::KGoogleAuth *googleAuth):
  m_auth(googleAuth),
  m_nam(new QNetworkAccessManager),
  m_namLocked(false)
{
  connect(m_nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(nam_replyReceived(QNetworkReply*)));
  
  connect(m_auth, SIGNAL(tokensRecevied(QString,QString)),
	  this, SLOT(newTokensReceived()));
}

KGoogleAccessManager::~KGoogleAccessManager()
{
  delete m_nam;
}

void KGoogleAccessManager::newTokensReceived()
{
  m_namLocked = false;

  submitCache();
}


void KGoogleAccessManager::nam_replyReceived(QNetworkReply* reply)
{
  QUrl new_request;  
  
  KGoogleRequest *request = reply->request().attribute(RequestAttribute).value<KGoogleRequest*>();
  if (!request)
    return;
  
  switch (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
    case KGoogleReply::OK:	/** << OK status (fetched, updated, removed) */
    case KGoogleReply::Created: /** << OK status (created) */
      break;

    case KGoogleReply::TemporarilyMoved:  /** << Temporarily moved - Google provides a new URL where to send the request */
      request->setUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
      nam_sendRequest(request);
      break;
      
    case KGoogleReply::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
      kWarning() << "Bad request, Google replied '" << reply->readAll() << "'";
      return;
    
    case KGoogleReply::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
      /* Lock the service, add request to cache and request new tokens. */
      if (!m_namLocked) {
	m_namLocked = true;
	m_cache << request;
	m_auth->refreshToken();
      }
      return;
      
    default: /** Something went wrong, there's nothing we can do about it */
      kWarning() << "Unknown error, Google replied '" << reply->readAll() << "'";
      return;
  }

  QByteArray rawData = reply->readAll();
  QList<KGoogleObject*> replyData;
  
  int type = QMetaType::type(qPrintable(request->serviceName()));
  KGoogleService *service = static_cast<KGoogleService*>(QMetaType::construct(type));
  
  switch (request->requestType()) {
    /* For fetch-all request parse the XML/JSON reply and split it to individual
     * <entry>/entry blocks which then convert to QList of KGoogleObjects */
    case KGoogleRequest::FetchAll: {
      FeedData *feedData = new FeedData;

      if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json")) {

	replyData = service->parseJSONFeed(rawData, feedData);

      } else if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/atom+xml")) {

	replyData = service->parseXMLFeed(rawData, feedData);
	
      }

      if (feedData->nextLink.isValid()) {
	QUrl url(feedData->nextLink);
	new_request.setUrl(request->url().toString());
	new_request.removeAllQueryItems("start-index");
	new_request.removeAllQueryItems("max-results");
	new_request.addQueryItem("start-index", url.queryItemValue("start-index"));
	new_request.addQueryItem("max-results", url.queryItemValue("max-results"));
	break;
      }
    } break;
      
    case KGoogleRequest::Fetch:
    case KGoogleRequest::Create:
    case KGoogleRequest::Update: {
      if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/json")) {
	
	replyData.append(service->JSONToObject(rawData));
	
      } else if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("application/atom+xml")) {
	
	replyData.append(service->XMLToObject(rawData));

      }
    } break;
    
    case KGoogleRequest::Remove:
      break;
  }

  KGoogleReply *greply = new KGoogleReply(request->requestType(),
					  (KGoogle::KGoogleReply::ErrorCode)reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
					  request->serviceName(),
					  replyData,
					  request);
  
  emit replyReceived(greply);
  
  /* Re-send the request on a new URL */
  if (new_request.isValid()) {
    request->setUrl(new_request);
    nam_sendRequest(request);
  }
  
  delete service;  
}

void KGoogleAccessManager::nam_sendRequest(KGoogleRequest* request)
{
  QNetworkRequest nr;
 
  int type = QMetaType::type(qPrintable(request->serviceName()));
  KGoogleService *service = static_cast<KGoogleService*>(QMetaType::construct(type));
  if (!service) {
    kWarning() << "Failed to resolve service " << request->serviceName();
    return;
  }

  nr.setRawHeader("Authorization","OAuth " + m_auth->accessToken().toLatin1());
  nr.setRawHeader("GData-Version", service->protocolVersion().toLatin1());
  nr.setUrl(request->url());
  nr.setAttribute(QNetworkRequest::User, QVariant::fromValue(request));
  
  delete service;
  
  switch (request->requestType()) {
    case KGoogleRequest::FetchAll:
      m_nam->get(nr);
      break;
    
    case KGoogleRequest::Fetch:
      m_nam->get(nr);
      break;
    
    case KGoogleRequest::Create:
      nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
      m_nam->post(nr, request->requestData());
      break;
    
    case KGoogleRequest::Update:
      nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
      nr.setRawHeader("If-Match", "*");
      m_nam->put(nr, request->requestData());
      break;
    
    case KGoogleRequest::Remove:
      nr.setRawHeader("If-Match", "*");
      m_nam->deleteResource(nr);
      break;
  }
}


void KGoogleAccessManager::submitCache()
{
  while (!m_cache.isEmpty() && !m_namLocked)
    nam_sendRequest(m_cache.takeFirst());
}


void KGoogleAccessManager::sendRequest(KGoogleRequest *request)
{
  /* Queue to cache */
  m_cache.append(request);
  
  if (!m_namLocked)
    submitCache();
}