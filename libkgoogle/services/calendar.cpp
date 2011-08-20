/*
    <one line to give the program's name and a brief idea of what it does.>
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


#include "calendar.h"
#include "objects/calendar.h"
#include "objects/event.h"

#include <kcalcore/event.h>
#include <kcalcore/attendee.h>
#include <kcalcore/alarm.h>
#include <kcalcore/recurrence.h>
#include <kcalcore/recurrencerule.h>
#include <kcalcore/icalformat.h>

#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGoogle;
using namespace Service;


/********** PUBLIC JSON INTERFACE ************/

KGoogleObject* Calendar::JSONToObject(const QByteArray& jsonData)
{
  QJson::Parser parser;
  
  QVariantMap object = parser.parse(jsonData).toMap();
  
  if (object["kind"] == "calendar#calendar")
    return JSONToCalendar(object);
  else if (object["kind"] == "calendar#event")
    return JSONToEvent(object);
  else {
    /* FIXME: This is a very very wild assumption */
    return JSONToEvent(object);
  }

}

QByteArray Calendar::objectToJSON(KGoogleObject* object)
{
  QVariantMap map;
  
  if (dynamic_cast<const Object::Calendar*>(object)) {
    map = calendarToJSON(object);
  } else if (dynamic_cast<const Object::Event*>(object)) {
    map = eventToJSON(object);
  }

  QJson::Serializer serializer;
  return serializer.serialize(map);
}

QList< KGoogleObject* > Calendar::parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData)
{
  QJson::Parser parser;
  
  QVariantMap map = parser.parse(jsonFeed).toMap();
  QVariantMap data = map["data"].toMap();
  
  QList< KGoogleObject* > list;
  
  if (data["kind"] == "calendar#calendarFeed") {
    list = parseCalendarJSONFeed(data["items"].toList());
  } else if (data["kind"] == "calendar#eventFeed") {
    list = parseEventJSONFeed(data["items"].toList());
  }
    
  if (feedData) {
    feedData->itemsPerPage = data["itemsPerPage"].toInt();
    feedData->startIndex = data["startIndex"].toInt();
    feedData->totalResults = data["totalResults"].toInt();
    feedData->nextLink = data["nextLink"].toUrl();
  }
  
  return list;
}


/************* PUBLIC XML INTERFACE ***********/

KGoogleObject* Calendar::XMLToObject(const QByteArray& xmlData)
{
  Q_UNUSED (xmlData);
  
  return 0;
}

QByteArray Calendar::objectToXML(KGoogleObject* object)
{
  Q_UNUSED (object);
  
  return QByteArray();
}

QList< KGoogleObject* > Calendar::parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData)
{
  Q_UNUSED (xmlFeed);
  Q_UNUSED (feedData);
  
  return QList< KGoogleObject* >();
}


/************* URLS **************/

QString Calendar::scopeUrl() 
{ 
  return "https://www.google.com/calendar/feeds/"; 
}

QString Calendar::fetchAllUrl() 
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  return "https://www.google.com/calendar/feeds/%1/%2/full?alt=jsonc"; 
}

QString Calendar::fetchUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  // %3 = event/calendar ID
  return "https://www.google.com/calendar/feeds/%1/%2/full/%3?alt=jsonc";
}

QString Calendar::createUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  return "https://www.google.com/calendar/feeds/%1/%2/full?alt=jsonc";
}

QString Calendar::updateUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  // %3 = event/calendar ID
  return "https://www.google.com/calendar/feeds/%1/%2/full/%3?alt=jsonc";
}

 QString Calendar::removeUrl() 
{ 
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  // %3 = event/calendar ID
  return "https://www.google.com/calendar/feeds/%1/%2/full/%3"; 
}

const QString Calendar::protocolVersion()
{
  return "2";
}

bool Calendar::supportsJSONRead(QString* urlParam)
{
  if (urlParam)
    *urlParam = "jsonc";
  
  return true;
}

bool Calendar::supportsJSONWrite(QString* urlParam)
{
  if (urlParam)
    *urlParam = "jsonc";
  
  return true;
}




/******** PRIVATE METHODS ************/
KGoogleObject* Calendar::JSONToCalendar(const QVariantMap& calendar)
{
  Object::Calendar *object = new Object::Calendar();
  
  QString id = calendar["id"].toString().replace("%40", "@");
  id.remove("http://www.google.com/calendar/feeds/default/calendars/"); // Remove the URL prefix
    
  object->setId(id);
  object->setEtag(calendar["etag"].toString());
  object->setTitle(calendar["title"].toString());
  object->setColor(calendar["color"].toString());
  object->setDetails(calendar["details"].toString());
  object->setEditable(calendar["editable"].toBool());
  object->setCreated(KDateTime::fromString(calendar["created"].toString(), KDateTime::RFC3339Date));
  object->setUpdated(KDateTime::fromString(calendar["updated"].toString(), KDateTime::RFC3339Date));
 
  return dynamic_cast< KGoogleObject* >(object);
}

QVariantMap Calendar::calendarToJSON(KGoogleObject* calendar)
{
  QVariantMap output, entry;
  Object::Calendar *object = static_cast< Object::Calendar* >(calendar);
  
  entry["id"] = "http://www.google.com/calendar/feeds/default/calendars/" + calendar->id(); 
  entry["title"] = object->title();
  entry["color"] = object->color();
  entry["details"] = object->details();
  entry["editable"] = object->editable();
  
  output["apiVersion"] = "2.3";
  output["data"] = entry;
  
  return output;
}

QList< KGoogleObject* > Calendar::parseCalendarJSONFeed(const QVariantList& feed)
{
  QList< KGoogleObject* > output;
 
  foreach (QVariant i, feed) {
    output.append(JSONToCalendar(i.toMap()));
  }

  return output;
}

KGoogleObject* Calendar::JSONToEvent(const QVariantMap& event)
{
  Object::Event *object = new Object::Event();
  
  /* ID */
  object->setId(event["id"].toString());
  
  /* ETAG */
  object->setEtag(event["etag"].toString());
  
  /* Deleted? */
  object->setDeleted(event["deleted"].toBool());

  /* Can edit? */
  object->setReadOnly(!event["canEdit"].toBool());
  
  /* Summary */
  object->setSummary(event["title"].toString());
  
  /* Description */
  object->setDescription(event["details"].toString());
  
  /* Created */
  object->setCreated(KDateTime::fromString(event["created"].toString(), KDateTime::RFC3339Date));
  
  /* Last updated */
  object->setLastModified(KDateTime::fromString(event["updated"].toString(), KDateTime::RFC3339Date));
  
  /* Status */
  if (event["status"].toString() == "confirmed")
    object->setStatus(KCalCore::Incidence::StatusConfirmed);
  else if (event["status"].toString() == "canceled") {
    object->setStatus(KCalCore::Incidence::StatusCanceled);
    object->setDeleted(true);
  } else if (event["status"].toString() == "tentative")
    object->setStatus(KCalCore::Incidence::StatusTentative);
  else
    object->setStatus(KCalCore::Incidence::StatusNone);
  
  /* Transparency */
  if (event["transparency"].toString() == "transparent")
    object->setTransparency(KCalCore::Event::Transparent);
  else /* Assume opaque as default transparency */
    object->setTransparency(KCalCore::Event::Opaque);
  
  /* Location */
  object->setLocation(event["location"].toString());
  
  /* Attendees */
  QVariantList attendees = event["attendees"].toList();
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
      object->setOrganizer((KCalCore::Person::Ptr) person);
    }
    
    object->addAttendee((KCalCore::Attendee::Ptr)attendee, true);
  }
  
  /* Start, End, all-day event */
  QVariantList whens = event["when"].toList();
  foreach (const QVariant &w, whens) {
    QVariantMap when = w.toMap();
    KDateTime dtStart, dtEnd;
    /* When dtStart and dtEnd are only dates without times, than assume all-day event */
    if ((when["start"].toString().length() == 10) &&
       (when["end"].toString().length() == 10)) {
      object->setAllDay(true);
      dtStart = KDateTime::fromString(when["start"].toString(), "%Y-%m-%d");
      dtEnd = KDateTime::fromString(when["end"].toString(), "%Y-%m-%d");
    } else {
      dtStart = KDateTime::fromString(when["start"].toString(), KDateTime::RFC3339Date);
      dtEnd = KDateTime::fromString(when["end"].toString(), KDateTime::RFC3339Date);
      
      if (!dtStart.isLocalZone())
	dtStart = dtStart.toLocalZone();
      
      if (!dtEnd.isLocalZone())
	dtEnd = dtEnd.toLocalZone();
    }
    
    object->setDtStart(dtStart);
    object->setDtEnd(dtEnd);
  }
  
  /* Recurrence */
  QStringList recrs = event["recurrence"].toString().split("\r\n");
  foreach (const QString &rec, recrs) {
    if (rec.left(7) == "DTSTART") {
      /* Sometimes the date is YYYYMMDDTHHMMSS and sometimes there Z at the end of the date */
      int start = rec.lastIndexOf(":")+1;
      object->setDtStart(KDateTime::fromString(rec.mid(start, 15), "%Y%m%dT%H%M%S"));
    } else  if (rec.left(5) == "DTEND") {
      int start = rec.lastIndexOf(":")+1;
      object->setDtEnd(KDateTime::fromString(rec.mid(start, 15), "%Y%m%dT%H%M%S"));
    } else if (rec.left(5) == "RRULE") {
      KCalCore::ICalFormat format;
      KCalCore::RecurrenceRule *recurrenceRule = object->recurrence()->defaultRRule(true);
      format.fromString(recurrenceRule, rec.mid(6));
    } else if (rec == "BEGIN:VTIMEZONE") {
      /* TODO: Implement time-zone dependent recurrences. For now take only the "standard"
       * and leave when it comes to any other */
      break;
    }
  }
  
  /* Reminders - the recurrence and/or start/end must be set BEFORE alarms 
   * because alarms must already know dtStart! */
  QVariantList reminders = event["reminders"].toList();
  foreach (const QVariant &r, reminders) {
    QVariantMap reminder = r.toMap();
    KCalCore::Alarm *alarm = new KCalCore::Alarm(object);
    alarm->setTime(object->dtStart());
    
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
    object->addAlarm((KCalCore::Alarm::Ptr) alarm);
  }
  
  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */  
  
  return dynamic_cast< KGoogleObject* >(object);
}

QVariantMap Calendar::eventToJSON(KGoogleObject* event)
{
  Object::Event *object = static_cast<Object::Event*>(event);
  QVariantMap output, data;
  
  /* ID */
  data["id"] = object->id();
  
  /* Deleted? */
  data["deleted"] = object->deleted();
  
  /* Can edit? */
  data["canEdit"] = object->isReadOnly();
   
  /* Summary */
  data["title"] = object->summary();
  
  /* Description */
  data["details"] = object->description();
  
  /* Created */
  data["created"] = object->created().toString(KDateTime::RFC3339Date);
  
  /* Last updated */
  data["updated"] = object->lastModified().toString(KDateTime::RFC3339Date);
  
  /* Status */
  if (object->status() == KCalCore::Incidence::StatusConfirmed)
    data["status"] = "confirmed";
  else if (object->status() == KCalCore::Incidence::StatusCanceled)
    data["status"] = "canceled";
  else if (object->status() == KCalCore::Incidence::StatusTentative)
    data["status"] = "tentative";

  /* Transparency */
  if (object->transparency() == KCalCore::Event::Transparent)
    data["transparency"] = "transparent";
  else
    data["transparency"] = "opaque";
  
  /* Location */
  data["location"] = object->location();
  
  /* Attendees */
  QVariantList atts;
  foreach (const KCalCore::Attendee::Ptr attee, object->attendees()) {
    QVariantMap att;
    
    att["displayName"] = attee->name();
    att["email"] = attee->email();
    
    if (attee->status() == KCalCore::Attendee::Accepted)
      att["status"] = "accepted";
    else if (attee->status() == KCalCore::Attendee::None)
      att["status"] = "invited";
    else if (attee->status() == KCalCore::Attendee::Declined)
      att["status"] = "declined";
    else if (attee->status() == KCalCore::Attendee::Tentative)
      att["status"] == "tentative";
    
    if (attee->role() == KCalCore::Attendee::ReqParticipant)
      att["type"] = "required";
    else if (attee->role() == KCalCore::Attendee::OptParticipant)
      att["type"] = "optional";
    /* No further roles supported by Google */

    atts.append(att);
  }
  
  /* Organizer. Google expects organizator as one of attendees. */
  QVariantMap org;
  org["displayName"] = object->organizer()->name();
  org["email"] = object->organizer()->email();
  org["rel"] = "organizer";
  atts.append(org);
  
  data["attendees"] = atts;

  /* Start, end */
  QVariantList whens;
  QVariantMap when;
  if (object->allDay()) {
    when["start"] = object->dtStart().toString("%Y-%m-%d");
    when["end"] = object->dtEnd().addDays(1).toString("%Y-%m-%d");
  } else {
    when["start"] = object->dtStart().toString(KDateTime::RFC3339Date);
    when["end"] = object->dtEnd().toString(KDateTime::RFC3339Date);
  }

  /* Reminders (subpart of "WHEN") */
  QVariantList als;
  foreach (KCalCore::Alarm::Ptr a, object->alarms()) {
    QVariantMap alarm;
    if (a->type() == KCalCore::Alarm::Display)
      alarm["method"] = "alert";
    else if (a->type() == KCalCore::Alarm::Email)
      alarm["method"] = "email";
    
    if (a->startOffset().asSeconds() < 0)
      alarm["minutes"] = a->startOffset().asSeconds()/(-60);
    else
      alarm["absoluteTime"] = a->time().toString(KDateTime::RFCDate);
    
    als.append(alarm);
  }
  when["reminders"] = als;
  whens.append(when);
  data["when"] = whens;
  
  /* Recurrence */
  KCalCore::ICalFormat format;
  QString recStr;
  QString start, end;
  if (object->allDay()) {
    start = "VALUE=DATE:" + object->dtStart().toUtc().toString("%Y%m%d");
    end = "VALUE=DATE:" + object->dtEnd().toUtc().toString("%Y%m%d");
  } else {
    start = "TZID=" + object->dtStart().timeZone().name() + ":" + object->dtStart().toLocalZone().toString("%Y%m%dT%H%M%S");
    end = "TZID=" + object->dtEnd().timeZone().name() + ":" + object->dtEnd().toLocalZone().toString("%Y%m%dT%H%M%S");
  }
  recStr = "DTSTART;" + start + "\r\n" +
	   "DTEND;" + end;
  foreach (KCalCore::RecurrenceRule *rrule, object->recurrence()->rRules())
    recStr += "\r\n" + format.toString(rrule);
  if (object->recurrence()->rRules().length() > 0) {
    data["recurrence"] = recStr;
    data["when"].toMap().remove("start"); /* Can't have 'recurrence' and 'when' together */
    data["when"].toMap().remove("end"); /* Can't have 'recurrence' and 'when' together */
  }
  
  
  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */
  
  output["apiVersion"] = "2.3";
  output["data"] = data;
  
  return output;
}

QList< KGoogleObject* > Calendar::parseEventJSONFeed(const QVariantList& feed)
{
  QList< KGoogleObject* > output;

  foreach (QVariant i, feed) {
    output.append(JSONToEvent(i.toMap()));
  }

  return output;
}