/*
    libKGoogle - Services - Calendar
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
#include "accessmanager.h"
#include "objects/calendar.h"
#include "objects/event.h"

#ifdef WITH_KCAL
#include <kcal/event.h>
#include <kcal/attendee.h>
#include <kcal/alarm.h>
#include <kcal/recurrence.h>
#include <kcal/recurrencerule.h>
#include <kcal/icalformat.h>
#else
#include <kcalcore/event.h>
#include <kcalcore/attendee.h>
#include <kcalcore/alarm.h>
#include <kcalcore/recurrence.h>
#include <kcalcore/recurrencerule.h>
#include <kcalcore/icalformat.h>
#endif

#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <kcalcore/alarm.h>
#include <ksystemtimezone.h>

using namespace KGoogle;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif


/********** PUBLIC JSON INTERFACE ************/

KGoogle::Object* Services::Calendar::JSONToObject(const QByteArray& jsonData)
{
  QJson::Parser parser;

  QVariantMap object = parser.parse(jsonData).toMap();
  QVariantMap data = object["data"].toMap();

  if (object["kind"] == "calendar#calendar")
    return JSONToCalendar(data);
  else if (object["kind"] == "calendar#event")
    return JSONToEvent(data);
  else {
    /* FIXME: This is a very very wild assumption */
    return JSONToEvent(data);
  }

}

QByteArray Services::Calendar::objectToJSON(KGoogle::Object* object)
{
  QVariantMap map;

  if (dynamic_cast< const Objects::Calendar* >(object)) {
    map = calendarToJSON(object);
  } else if (dynamic_cast< const Objects::Event* >(object)) {
    map = eventToJSON(object);
  }

  QJson::Serializer serializer;
  return serializer.serialize(map);
}

QList< KGoogle::Object* > Services::Calendar::parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData)
{
  QJson::Parser parser;

  QVariantMap map = parser.parse(jsonFeed).toMap();
  QVariantMap data = map["data"].toMap();

  QList< KGoogle::Object* > list;

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

KGoogle::Object* Services::Calendar::XMLToObject(const QByteArray& xmlData)
{
  Q_UNUSED (xmlData);

  return 0;
}

QByteArray Services::Calendar::objectToXML(KGoogle::Object* object)
{
  Q_UNUSED (object);

  return QByteArray();
}

QList< KGoogle::Object* > Services::Calendar::parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData)
{
  Q_UNUSED (xmlFeed);
  Q_UNUSED (feedData);

  return QList< KGoogle::Object* >();
}


/************* URLS **************/

QString Services::Calendar::scopeUrl()
{
  return "https://www.google.com/calendar/feeds/";
}

QString Services::Calendar::fetchAllUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  return "https://www.google.com/calendar/feeds/%1/%2/full?alt=jsonc";
}

QString Services::Calendar::fetchUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  // %3 = event/calendar ID
  return "https://www.google.com/calendar/feeds/%1/%2/full/%3?alt=jsonc";
}

QString Services::Calendar::createUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  return "https://www.google.com/calendar/feeds/%1/%2/full?alt=jsonc";
}

QString Services::Calendar::updateUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  // %3 = event/calendar ID
  return "https://www.google.com/calendar/feeds/%1/%2/full/%3?alt=jsonc";
}

 QString Services::Calendar::removeUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = visibility (allcalendars for list of calendars, private for list of events)
  // %3 = event/calendar ID
  return "https://www.google.com/calendar/feeds/%1/%2/full/%3";
}

const QString Services::Calendar::protocolVersion()
{
  return "2";
}

bool Services::Calendar::supportsJSONRead(QString* urlParam)
{
  if (urlParam)
    *urlParam = "jsonc";

  return true;
}

bool Services::Calendar::supportsJSONWrite(QString* urlParam)
{
  if (urlParam)
    *urlParam = "jsonc";

  return true;
}




/******** PRIVATE METHODS ************/
KGoogle::Object* Services::Calendar::JSONToCalendar(const QVariantMap& calendar)
{
  Objects::Calendar *object = new Objects::Calendar();

  QString id = calendar["id"].toString().replace("%40", "@");
  id.remove("http://www.google.com/calendar/feeds/default/calendars/"); // Remove the URL prefix

  object->setId(id);
  object->setEtag(calendar["etag"].toString());
  object->setTitle(calendar["title"].toString());
  object->setColor(calendar["color"].toString());
  object->setDetails(calendar["details"].toString());
  object->setEditable(calendar["editable"].toBool());
  object->setCreated(AccessManager::RFC3339StringToDate(calendar["created"].toString()));
  object->setUpdated(AccessManager::RFC3339StringToDate(calendar["created"].toString()));
 
  return dynamic_cast< KGoogle::Object* >(object);
}

QVariantMap Services::Calendar::calendarToJSON(KGoogle::Object* calendar)
{
  QVariantMap output, entry;
  Objects::Calendar *object = static_cast< Objects::Calendar* >(calendar);

  entry["id"] = "http://www.google.com/calendar/feeds/default/calendars/" + calendar->id(); 
  entry["title"] = object->title();
  entry["color"] = object->color();
  entry["details"] = object->details();
  entry["editable"] = object->editable();

  output["apiVersion"] = "2.3";
  output["data"] = entry;

  return output;
}

QList< KGoogle::Object* > Services::Calendar::parseCalendarJSONFeed(const QVariantList& feed)
{
  QList< KGoogle::Object* > output;

  foreach (QVariant i, feed) {
    output.append(JSONToCalendar(i.toMap()));
  }

  return output;
}

KGoogle::Object* Services::Calendar::JSONToEvent(const QVariantMap& event)
{
  Objects::Event *object = new Objects::Event();

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
  object->setCreated(AccessManager::RFC3339StringToDate(event["created"].toString()));

  /* Last updated */
  object->setLastModified(AccessManager::RFC3339StringToDate(event["updated"].toString()));

  /* Status */
  if (event["status"].toString() == "confirmed") {
    object->setStatus(Incidence::StatusConfirmed);
  } else if (event["status"].toString() == "canceled") {
    object->setStatus(Incidence::StatusCanceled);
    object->setDeleted(true);
  } else if (event["status"].toString() == "tentative") {
    object->setStatus(Incidence::StatusTentative);
  } else {
    object->setStatus(Incidence::StatusNone);
  }

  /* Transparency */
  if (event["transparency"].toString() == "transparent") {
    object->setTransparency(Event::Transparent);
  } else { /* Assume opaque as default transparency */
    object->setTransparency(Event::Opaque);
  }

  /* Location */
  object->setLocation(event["location"].toString());

  /* Attendees */
  QVariantList attendees = event["attendees"].toList();
  foreach (const QVariant &a, attendees) {
    QVariantMap att = a.toMap();
    AttendeePtr attendee(new Attendee(att["displayName"].toString(),
				      att["email"].toString()));
    if (att["status"].toString() == "accepted")
      attendee->setStatus(Attendee::Accepted);
    else if (att["status"].toString() == "invited")
      attendee->setStatus(Attendee::None);
    else if (att["status"].toString() == "declined")
      attendee->setStatus(Attendee::Declined);
    else if (att["status"].toString() == "tentative")
      attendee->setStatus(Attendee::Tentative);
    else
      attendee->setStatus(Attendee::None);

    if (att["type"] == "required")
      attendee->setRole(Attendee::ReqParticipant);
    else if (att["type"] == "optional")
      attendee->setRole(Attendee::OptParticipant);
    /* No further roles supported by Google */

    /* Google sends organizer as one of attendees with rel="organizer" attribute */
    if (att["rel"] == "organizer") {
      PersonPtr person(new Person(att["displayName"].toString(),
				  att["email"].toString()));
#ifdef WITH_KCAL
      object->setOrganizer(*person);
#else
      object->setOrganizer(person);
#endif
    }

    object->addAttendee(attendee, true);
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
      dtEnd = KDateTime::fromString(when["end"].toString(), "%Y-%m-%d").addDays(-1);
    } else {
      dtStart = AccessManager::RFC3339StringToDate(when["start"].toString());
      dtEnd = AccessManager::RFC3339StringToDate(when["end"].toString());

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
      bool allday;
      KDateTime dt = parseRecurrenceDT(rec.mid(8), &allday);
      object->setDtStart(dt);
      object->setAllDay(allday);
    } else  if (rec.left(5) == "DTEND") {
      bool allday;
      KDateTime dt = parseRecurrenceDT(rec.mid(6), &allday);
      object->setDtEnd(dt);
      object->setAllDay(allday);
    } else if (rec.left(5) == "RRULE") {
      ICalFormat format;
      RecurrenceRule *recurrenceRule = object->recurrence()->defaultRRule(true);
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
    AlarmPtr alarm(new Alarm(object));
    alarm->setTime(object->dtStart());

    if (reminder["method"].toString() == "alert")
      alarm->setType(Alarm::Display);
    else if (reminder["method"].toString() == "email")
      alarm->setType(Alarm::Email);
    else {
      continue;
    }

    if (!reminder["minutes"].toString().isEmpty()) {
      /* Start offset is in seconds and must be negative, so that the alarm trigger times
       * is BEFORE the event start. When alarm should trigger AFTER the event start, 
       * it is specified as "absoluteTime" */
      alarm->setStartOffset(Duration(reminder["minutes"].toInt()*(-60)));
    } else if (!reminder["absoluteTime"].toString().isEmpty()) {
      alarm->setTime(KDateTime::fromString(reminder["absoluteTime"].toString(), "%Y-%m-%dT%H:%M:%S"));
    } else {
      continue;
    }
    alarm->setEnabled(true);
    object->addAlarm(alarm);
  }

  /* Extended properties */
  QVariantList extendedProperties = event["extendedProperties"].toList();
  foreach (const QVariant &p, extendedProperties) {
    QVariantMap property = p.toMap();
    if (property["name"].toString() == "categories") {
	object->setCategories(property["value"].toString());
    }
  }
  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */

  return dynamic_cast< KGoogle::Object* >(object);
}

QVariantMap Services::Calendar::eventToJSON(KGoogle::Object* event)
{
  Objects::Event *object = static_cast<Objects::Event*>(event);
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
  data["created"] = AccessManager::dateToRFC3339String(object->created());

  /* Last updated */
  data["updated"] = AccessManager::dateToRFC3339String(object->lastModified());

  /* Status */
  if (object->status() == Incidence::StatusConfirmed)
    data["status"] = "confirmed";
  else if (object->status() == Incidence::StatusCanceled)
    data["status"] = "canceled";
  else if (object->status() == Incidence::StatusTentative)
    data["status"] = "tentative";

  /* Transparency */
  if (object->transparency() == Event::Transparent)
    data["transparency"] = "transparent";
  else
    data["transparency"] = "opaque";

  /* Location */
  data["location"] = object->location();

  /* Attendees */
  QVariantList atts;
  foreach (const AttendeePtr attee, object->attendees()) {
    QVariantMap att;

    att["displayName"] = attee->name();
    att["email"] = attee->email();

    if (attee->status() == Attendee::Accepted)
      att["status"] = "accepted";
    else if (attee->status() == Attendee::None)
      att["status"] = "invited";
    else if (attee->status() == Attendee::Declined)
      att["status"] = "declined";
    else if (attee->status() == Attendee::Tentative)
      att["status"] == "tentative";

    if (attee->role() == Attendee::ReqParticipant)
      att["type"] = "required";
    else if (attee->role() == Attendee::OptParticipant)
      att["type"] = "optional";
    /* No further roles supported by Google */

    atts.append(att);
  }

  /* Organizer. Google expects organizator as one of attendees. */
  QVariantMap org;
#ifdef WITH_KCAL
  PersonPtr organizer = new Person(object->organizer());
#else
  PersonPtr organizer = object->organizer();
#endif
  org["displayName"] = organizer->name();
  org["email"] = organizer->email();
  org["rel"] = "organizer";
  atts.append(org);

  data["attendees"] = atts;


  /* Start, end, reminders */
  QVariantMap when;

  /* Reminders */
  QVariantList reminders;
  foreach (AlarmPtr a, object->alarms()) {
    QVariantMap alarm;
    if (a->type() == Alarm::Display)
      alarm["method"] = "alert";
    else if (a->type() == Alarm::Email)
      alarm["method"] = "email";

    if (a->startOffset().asSeconds() < 0)
      alarm["minutes"] = a->startOffset().asSeconds()/(-60);
    else
      alarm["absoluteTime"] = a->time().toString(KDateTime::RFCDate);

    reminders.append(alarm);
  }

  /* Recurrence */
  ICalFormat format;
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
  foreach (RecurrenceRule *rrule, object->recurrence()->rRules())
    recStr += "\r\n" + format.toString(rrule);

  if (object->recurrence()->rRules().length() > 0) {
    data["recurrence"] = recStr;

    /* For recurrent events, reminders must be set outside "gd:when"
     * elements. */
    if (reminders.length() > 0)
      data["reminders"] = reminders;

  } else {

    /* When no recurrence is set, we can set dtStart and dtEnd */    
    if (object->allDay()) {
      when["start"] = object->dtStart().toString("%Y-%m-%d");
      when["end"] = object->dtEnd().addDays(1).toString("%Y-%m-%d");
    } else {
      when["start"] = AccessManager::dateToRFC3339String(object->dtStart());
      when["end"] = AccessManager::dateToRFC3339String(object->dtEnd());
    }
    when["reminders"] = reminders;
    data["when"] = QVariantList() << when;
  }


  /* Store categories */
  if (!object->categories().isEmpty()) {
    QVariantMap categories;
    categories["name"] = "categories";
    categories["value"] = object->categoriesStr();
    data["extendedProperties"] = QVariantList() << categories;
  }

  /* TODO: Implement support for additional features:
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   */

  output["apiVersion"] = "2.3";
  output["data"] = data;

  return output;
}

QList< KGoogle::Object* > Services::Calendar::parseEventJSONFeed(const QVariantList& feed)
{
  QList< KGoogle::Object* > output;

  foreach (QVariant i, feed) {
    output.append(JSONToEvent(i.toMap()));
  }

  return output;
}

KDateTime Services::Calendar::parseRecurrenceDT(const QString& dt, bool *allDay)
{
    KDateTime out;
    KTimeZone tz;
    QString s, tzid;
    int p;

    p = dt.indexOf(";");
    s = dt.mid(p + 1);

    if (s.startsWith("TZID=")) {
	int i;

	s.remove("TZID=");
	i = s.indexOf(":");

	tz = KSystemTimeZones::zone(s.left(i));
	s = s.mid(i + 1);
    } else {
	tz = KSystemTimeZones::local();
    }

    if (s.length() == 8) {
      out.fromString(s, KDateTime::ISODate);
      if (allDay) 
	  *allDay = true;
    } else {
      out = KDateTime::fromString(s, KDateTime::ISODate);
      out.setTimeSpec(tz);
      if (allDay)
	  *allDay = false;
    }

    return out;
}

