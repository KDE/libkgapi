/*
    Akonadi Google - Calendar Resource
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

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QtCore/QVariantMap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <KDE/KCalCore/Alarm>
#include <KDE/KCalCore/Attendee>
#include <KDE/KCalCore/Event>
#include <KDE/KCalCore/Person>
#include <KDE/KCalCore/Recurrence>
#include <KDE/KCalCore/RecurrenceRule>
#include <KDE/KCalCore/ICalFormat>

#include "eventjob.h"

EventJob::EventJob(const QString& accessToken, const QString& calendarId, const QString& eventId):
  m_accessToken(accessToken),
  m_calendarId(calendarId),
  m_eventId(eventId)
{
}


void EventJob::start()
{
  requestData(QUrl("https://www.google.com/calendar/feeds/"+m_calendarId+"/private/full/"+m_eventId+"?alt=jsonc"));
}
  
  
void EventJob::requestData(const QUrl &url)
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest request;
  QUrl requestUrl = url;

  if (!requestUrl.hasQueryItem("alt"))
    requestUrl.addQueryItem("alt", "jsonc");
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(requestFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  request.setUrl(requestUrl);
  request.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  request.setRawHeader("GData-Version", "2.0");
  
  nam->get(request);
}

void EventJob::requestFinished(QNetworkReply *reply)
{
  if (reply->error()) {
    setError(reply->error());
    setErrorText(reply->errorString());
    emitResult();
    return;
  }
  
  /* Google may sometimes return 301 Redirect with a new URL where the object is located. */
  if (reply->header(QNetworkRequest::LocationHeader).isValid()) {
    requestData(reply->header(QNetworkRequest::LocationHeader).toUrl());
    return;
  }    
  
  QJson::Parser parser;
  bool ok;
  QVariantMap data;
  
  data = parser.parse(reply->readAll(), &ok).toMap();
  
  if (!ok) {
    setError(1);
    setErrorText("Invalid response format");
    emitResult();
    return;
  }
  
  m_event = JSONToKCal(data["data"].toMap());
  
  emitResult();
}



KCalCore::Event* EventJob::JSONToKCal(QVariantMap jsonData)
{
  KCalCore::Event *event = new KCalCore::Event;
  
  /* ID */
  event->setUid(jsonData["id"].toString());

  /* Can edit? */
  event->setReadOnly(!jsonData["canEdit"].toBool());
  
  /* Summary */
  event->setSummary(jsonData["title"].toString());
  
  /* Description */
  event->setDescription(jsonData["details"].toString());
  
  /* Created */
  event->setCreated(KDateTime::fromString(jsonData["created"].toString(), "%Y-%m-%dT%H:%M:%S%:SZ"));
  
  /* Last updated */
  event->setLastModified(KDateTime::fromString(jsonData["updated"].toString(), "%Y-%m-%dT%H:%M:%S%:SZ"));
  
  /* Status */
  if (jsonData["status"].toString() == "confirmed")
    event->setStatus(KCalCore::Incidence::StatusConfirmed);
  else if (jsonData["status"].toString() == "canceled")
    event->setStatus(KCalCore::Incidence::StatusCanceled);
  else if (jsonData["status"].toString() == "tentative")
    event->setStatus(KCalCore::Incidence::StatusTentative);
  else
    event->setStatus(KCalCore::Incidence::StatusNone);
  
  /* Transparency */
  if (jsonData["transparency"].toString() == "transparent")
    event->setTransparency(KCalCore::Event::Transparent);
  else /* Assume opaque as default transparency */
    event->setTransparency(KCalCore::Event::Opaque);
  
  /* Location */
  event->setLocation(jsonData["location"].toString());
  
  /* Attendees */
  QVariantList attendees = jsonData["attendees"].toList();
  foreach (const QVariant &a, attendees) {
    QVariantMap att = a.toMap();
    KCalCore::Attendee *attendee = new KCalCore::Attendee(att["displayName"].toString(),
							  att["email"].toString());
    if (att["status"].toString() == "accepted")
      attendee->setStatus(KCalCore::Attendee::Accepted);
    else if (att["status"].toString() == "invited")
      attendee->setStatus(KCalCore::Attendee::None);
    else if (att["status"].toString() == "declined")
      attendee->setStatus(KCalCore::Attendee::Declined);
    else if (att["status"].toString() == "tentative")
      attendee->setStatus(KCalCore::Attendee::Tentative);
    else
      attendee->setStatus(KCalCore::Attendee::None);

    if (att["type"] == "required")
      attendee->setRole(KCalCore::Attendee::ReqParticipant);
    else if (att["type"] == "optional")
      attendee->setRole(KCalCore::Attendee::OptParticipant);
    /* No further roles supported by Google */
    
    /* Google sends organizer as one of attendees with rel="organizer" attribute */
    if (att["rel"] == "organizer") {
      KCalCore::Person *person = new KCalCore::Person(att["displayName"].toString(),
						      att["email"].toString());
      event->setOrganizer((KCalCore::Person::Ptr) person);
    }
    
    event->addAttendee((KCalCore::Attendee::Ptr)attendee, true);
  }
  
  /* Start, End, all-day event */
  QVariantList whens = jsonData["when"].toList();
  foreach (const QVariant &w, whens) {
    QVariantMap when = w.toMap();
    KDateTime dtStart, dtEnd;
    /* When dtStart and dtEnd are only dates without times, than assume all-day event */
    if ((when["start"].toString().length() == 10) &&
       (when["end"].toString().length() == 10)) {
      event->setAllDay(true);
      dtStart = KDateTime::fromString(when["start"].toString(), "%Y-%m-%d");
      dtEnd = KDateTime::fromString(when["end"].toString(), "%Y-%m-%d");
      dtEnd = dtEnd.addDays(-1); /* Google says one-day all-day event = 2011-05-20 to 2011-05-21, 
			          * KDE says one-day all-day event = 2011-05-20 to 2011-05-20 */
    } else {
      if (when["start"].toString().right(1) == "Z") /* 2011-05-20T12:00:00.000Z */
	dtStart = KDateTime::fromString(when["start"].toString().left(19), "%Y-%m-%dT%H:%M:%S");
      else if (when["start"].toString().contains("+")) /* 2011-05-20T12:00;00.000+02:00 */
	dtStart = KDateTime::fromString(when["start"].toString().left(29), "%Y-%m-%dT%H:%M:%S%:s%:z");
	
      if (when["end"].toString().right(1) == "Z") /* 2011-05-20T12:00:00.000Z */
	dtEnd = KDateTime::fromString(when["end"].toString().left(19), "%Y-%m-%dT%H:%M:%S");
      else if (when["end"].toString().contains("+")) /* 2011-05-20T12:00;00.000+02:00 */
	dtEnd = KDateTime::fromString(when["end"].toString().left(29), "%Y-%m-%dT%H:%M:%S%:s%:z");
    }
    
    event->setDtStart(dtStart);
    event->setDtEnd(dtEnd);
  }
  
  /* Recurrence */
  QStringList recurrences = jsonData["recurrence"].toString().split("\r\n");
  foreach (const QString &recurrence, recurrences) {
    if (recurrence.left(7) == "DTSTART") {
      /* Sometimes the date is YYYYMMDDTHHMMSS and sometimes there Z at the end of the date */
      int start = recurrence.lastIndexOf(":")+1;
      event->setDtStart(KDateTime::fromString(recurrence.mid(start, 15), "%Y%m%dT%H%M%S"));
    } else  if (recurrence.left(5) == "DTEND") {
      int start = recurrence.lastIndexOf(":")+1;
      event->setDtEnd(KDateTime::fromString(recurrence.mid(start, 15), "%Y%m%dT%H%M%S"));
      /* Minus one day: Google says that all-day event is from 2011-05-20 to 2011-05-21, but
       * KDE says that all-day event is from 2011-05-20 to 2011-05-20. */
      event->setDtEnd(event->dtEnd().addDays(-1));
    } else if (recurrence.left(5) == "RRULE") {
      KCalCore::ICalFormat format;
      KCalCore::RecurrenceRule *recurrenceRule = event->recurrence()->defaultRRule(true);
      format.fromString(recurrenceRule, recurrence.mid(6));
    } else if (recurrence == "BEGIN:VTIMEZONE") {
      /* TODO: Implement time-zone dependent recurrences. For now take only the "standard"
       * and leave when it comes to any other */
      break;
    }
  }
  
  /* Reminders - the recurrence and/or start/end must be set BEFORE alarms 
   * because alarms must already know dtStart! */
  QVariantList reminders = jsonData["reminders"].toList();
  foreach (const QVariant &r, reminders) {
    QVariantMap reminder = r.toMap();
    KCalCore::Alarm *alarm = new KCalCore::Alarm(event);
    alarm->setTime(event->dtStart());
    
    if (reminder["method"].toString() == "alert")
      alarm->setType(KCalCore::Alarm::Display);
    else if (reminder["method"].toString() == "email")
      alarm->setType(KCalCore::Alarm::Email);
    else {
      delete alarm;
      continue;
    }
    
    if (!reminder["minutes"].toString().isEmpty()) {
      /* Start offset is in seconds and must be negative, so that the alarm trigger times
       * is BEFORE the event start. When alarm should trigger AFTER the event start, 
       * it is specified as "absoluteTime" */
      alarm->setStartOffset(KCalCore::Duration(reminder["minutes"].toInt()*(-60)));
    } else if (!reminder["absoluteTime"].toString().isEmpty()) {
      alarm->setTime(KDateTime::fromString(reminder["absoluteTime"].toString(), "%Y-%m-%dT%H:%M:%S"));
    } else {
      delete alarm;
      continue;
    }

    alarm->setEnabled(true);
    event->addAlarm((KCalCore::Alarm::Ptr) alarm);
  }
  
  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */
  
  return event;
}

QVariantMap EventJob::KCalToJSON(KCalCore::Event event)
{
  /* TODO: Implement me! */
  
  Q_UNUSED(event)
  
  return QVariantMap();
}


