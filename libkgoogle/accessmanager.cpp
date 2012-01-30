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
#include <qglobal.h>
#include <qregexp.h>
#include <qdatetime.h>

#include <kdebug.h>
#include <klocalizedstring.h>
#include <kdeversion.h>
#include <ksystemtimezone.h>

int debugArea() { static int s_area = KDebug::registerArea("libkgoogle"); return s_area; }

using namespace KGoogle;

#define RequestAttribute QNetworkRequest::User

AccessManager::AccessManager():
  m_nam(new KIO::Integration::AccessManager(this)),
  m_cacheSemaphore(new QSemaphore(1))
{
  connect(m_nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(nam_replyReceived(QNetworkReply*)));
  connect(Auth::instance(), SIGNAL(error(KGoogle::Error,QString)),
	  this, SIGNAL(error(KGoogle::Error,QString)));
}

AccessManager::~AccessManager()
{
  delete m_nam;
  delete m_cacheSemaphore;
}

void AccessManager::nam_replyReceived(QNetworkReply* reply)
{
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
    emit error(KGoogle::InvalidResponse, i18n("No valid reply received"));
    return;
  }

  switch (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
    case KGoogle::OK:		/** << OK status (fetched, updated, removed) */
    case KGoogle::Created: 	/** << OK status (created) */
    case KGoogle::NoContent:	/** << OK status (removed task using Tasks API) */
      break;

    case KGoogle::TemporarilyMoved:  /** << Temporarily moved - Google provides a new URL where to send the request */
      kDebug() << "Google says: Temporarily moved to " << reply->header(QNetworkRequest::LocationHeader).toUrl();
      request->setUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
      nam_sendRequest(request);
      return;

    case KGoogle::BadRequest: /** << Bad request - malformed data, API changed, something went wrong... */
      kWarning() << "Bad request, Google replied '" << reply->readAll() << "'";
      emit error(KGoogle::BadRequest, i18n("Bad request."));
      return;

    case KGoogle::Unauthorized: /** << Unauthorized - Access token has expired, request a new token */
      /* Lock the service, add request to cache and request new tokens. */
      if (m_cacheSemaphore->available() == 1) {
	m_cacheSemaphore->acquire();
        m_cache << request;

        KGoogle::Auth *auth = KGoogle::Auth::instance();
        connect(auth, SIGNAL(error(KGoogle::Error,QString)),
                this, SIGNAL(error(KGoogle::Error,QString)));
        auth->authenticate(request->account(), true);
      }
      /* Don't emit error here, user should not know that we need to re-negotiate tokens again. */
      return;

    default: /** Something went wrong, there's nothing we can do about it */
      kWarning() << "Unknown error" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
		 << ", Google replied '" << rawData << "'";
      emit error(KGoogle::UnknownError, i18n("Unknown error.\n\nGoogle replied '%1'", QString(rawData)));
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

  emit replyReceived(greply);

  /* Re-send the request on a new URL */
  if (new_request.isValid()) {
    request->setUrl(new_request);
    nam_sendRequest(request);
  } else {
    emit requestFinished(request);
  }

  delete service;
}

void KGoogle::AccessManager::nam_sendRequest(KGoogle::Request* request)
{
  QNetworkRequest nr;

  kDebug() << "Sending request to " << request->url();

#ifdef DEBUG_RAWDATA
  kDebug() << request->requestData();
#endif

  int type = QMetaType::type(qPrintable(request->serviceName()));
  KGoogle::Service *service = static_cast<KGoogle::Service*>(QMetaType::construct(type));
  if (!service) {
    kWarning() << "Failed to resolve service " << request->serviceName();
    emit error(KGoogle::UnknownService, i18n("Invalid request, service %1 is not registered.", request->serviceName()));
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
      m_nam->get(nr);
      break;

    case KGoogle::Request::Fetch:
      m_nam->get(nr);
      break;

    case KGoogle::Request::Create:
      nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
      m_nam->post(nr, request->requestData());
      break;

    case KGoogle::Request::Update:
      nr.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());
      nr.setRawHeader("If-Match", "*");
      m_nam->put(nr, request->requestData());
      break;

    case KGoogle::Request::Remove:
      nr.setRawHeader("If-Match", "*");
      m_nam->deleteResource(nr);
      break;
  }
}


void KGoogle::AccessManager::submitCache()
{
  while (!m_cache.isEmpty() && m_cacheSemaphore->available())
    nam_sendRequest(m_cache.takeFirst());
}


void KGoogle::AccessManager::sendRequest(KGoogle::Request *request)
{
  /* Queue to cache */
  m_cache.append(request);

  if (m_cacheSemaphore->available())
    submitCache();
}

void KGoogle::AccessManager::priv_error (const Error errCode, const QString &msg)
{
  emit error(errCode, msg);
}


KDateTime KGoogle::AccessManager::RFC3339StringToDate(const QString& datetime)
{
#if KDE_IS_VERSION(4,6,90)
  return KDateTime::fromString(datetime, KDateTime::RFC3339Date);
#else
  /* This is the original implementation of KDateTime::fromString() for RFC3339Date
   * from kdelibs/kdecore/date/kdatetime.cpp by David Jarvie.  */
  QRegExp rx(QString::fromLatin1("^(\\d{4})-(\\d\\d)-(\\d\\d)[Tt](\\d\\d):(\\d\\d):(\\d\\d)(?:\\.(\\d+))?([Zz]|([+-])(\\d\\d):(\\d\\d))$"));
  if (datetime.indexOf(rx))
    return KDateTime();
  const QStringList parts = rx.capturedTexts();
  bool ok, ok1, ok2;
  int msecs  = 0;
  bool leapSecond = false;
  int year = parts[1].toInt(&ok);
  int month = parts[2].toInt(&ok1);
  int day = parts[3].toInt(&ok2);
  if (!ok || !ok1 || !ok2)
    return KDateTime();
  QDate d(year, month, day);
  if (!d.isValid())
    return KDateTime();
  int hour = parts[4].toInt(&ok);
  int minute = parts[5].toInt(&ok1);
  int second = parts[6].toInt(&ok2);
  if (!ok || !ok1 || !ok2)
    return KDateTime();
  leapSecond = (second == 60);
  if (leapSecond)
    second = 59;   // apparently a leap second - validate below, once time zone is known
  if (!parts[7].isEmpty())
  {
    QString ms = parts[7] + QLatin1String("00");
    ms.truncate(3);
    msecs = ms.toInt(&ok);
    if (!ok)
      return KDateTime();
    if (msecs && leapSecond)
      return KDateTime();   // leap second only valid if 23:59:60.000
  }
  QTime t(hour, minute, second, msecs);
  if (!t.isValid())
    return KDateTime();
  int offset = 0;
  KDateTime::SpecType spec = (parts[8].toUpper() == QLatin1String("Z")) ? KDateTime::UTC : KDateTime::OffsetFromUTC;
  if (spec == KDateTime::OffsetFromUTC)
  {
    offset = parts[10].toInt(&ok) * 3600;
    offset += parts[11].toInt(&ok1) * 60;
    if (!ok || !ok1)
      return KDateTime();
    if (parts[9] == QLatin1String("-"))
    {
      if (!offset && leapSecond)
	return KDateTime();      // leap second only valid if known time zone
      offset = -offset;
    }
  }
  if (leapSecond)
  {
    // Validate a leap second time. Leap seconds are inserted after 23:59:59 UTC.
    // Convert the time to UTC and check that it is 00:00:00.
    if ((hour*3600 + minute*60 + 60 - offset + 86400*5) % 86400)   // (max abs(offset) is 100 hours)
      return KDateTime();    // the time isn't the last second of the day
  }

  return KDateTime(d, t, KDateTime::Spec(spec, offset));
#endif
}

QString KGoogle::AccessManager::dateToRFC3339String(const KDateTime& datetime)
{
#if KDE_IS_VERSION(4,6,90)
  return datetime.toString(KDateTime::RFC3339Date);
#else
  /* This is the original implementation of KDateTime::toString() for RFC3339Date
   * from kdelibs/kdecore/date/kdatetime.cpp by David Jarvie.  */
  QString s;
  QString result;
  const char *tzcolon = "";
  KTimeZone tz;
  char tzsign = '+';
  int offset = 0;

  result += s.sprintf("%04d-%02d-%02dT%02d:%02d:%02d",
		      datetime.date().year(), datetime.date().month(), datetime.date().day(),
                      datetime.time().hour(), datetime.time().minute(), datetime.time().second());
  int msec = datetime.time().msec();
  if (msec)
  {
    int digits = 3;
    if (!(msec % 10))
      msec /= 10, --digits;
    if (!(msec % 10))
      msec /= 10, --digits;
    result += s.sprintf(".%0*d", digits, datetime.time().msec());
  }
  if (datetime.timeType() == KDateTime::UTC)
    return result + QLatin1Char('Z');
  if (datetime.timeType() == KDateTime::ClockTime)
    tz = KSystemTimeZones::local();
  tzcolon = ":";

  // Return the string with UTC offset ±hhmm appended
  if (datetime.timeType() == KDateTime::OffsetFromUTC  ||  datetime.timeType() == KDateTime::TimeZone  ||  tz.isValid())
  {
    if (datetime.timeType() == KDateTime::TimeZone)
      offset = datetime.timeZone().offsetAtZoneTime(datetime.dateTime());
    else
      offset = tz.isValid() ? tz.offsetAtZoneTime(datetime.dateTime()) : datetime.utcOffset();
        if (offset < 0)
        {
            offset = -offset;
            tzsign = '-';
        }
    }
    offset /= 60;
    return result + s.sprintf("%c%02d%s%02d", tzsign, offset/60, tzcolon, offset%60);
#endif
}